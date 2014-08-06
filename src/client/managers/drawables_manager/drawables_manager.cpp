/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "drawables_manager.hpp"

namespace como {


/***
 * 1. Construction
 ***/

DrawablesManager::DrawablesManager( ServerInterfacePtr server, const PackableColor& localSelectionBorderColor, shared_ptr< QOpenGLContext > oglContext, LogPtr log ) :
    AbstractChangeable(),
    ResourcesManager( server ),
    nonSelectedDrawables_( new DrawablesSelection( glm::vec4( 0.0f ) ) ),
    oglContext_( oglContext ),
    log_( log )
{
    glm::vec4 selectionColor = localSelectionBorderColor.toVec4();

    // Add a selection of unselected drawables to the map of selections as a
    // selection associated to NO_USER).
    drawablesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( NO_USER, nonSelectedDrawables_ ) );

    // Create an empty drawables selection for the local user.
    localDrawablesSelection_ = LocalDrawablesSelectionPtr( new LocalDrawablesSelection( localUserID(), selectionColor, server ) );

    // Insert the recently created selection to the selections map.
    drawablesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( localUserID(), localDrawablesSelection_ ) );

    // Set a default mode for displaying the edges of the meshes in this
    // selection.
    displayEdges( MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED );
}


/***
 * 3. Getters
 ***/

LocalDrawablesSelectionPtr DrawablesManager::getLocalUserSelection() const
{
    return localDrawablesSelection_;
}


DrawablesSelectionPtr DrawablesManager::getUserSelection( UserID userID ) const
{
    return drawablesSelections_.at( userID );
}


DrawablesSelectionPtr DrawablesManager::getUserSelection( UserID userID )
{
    return drawablesSelections_.at( userID );
}


glm::vec3 DrawablesManager::getPivotPoint() const
{
    return getPivotPoint( localUserID() );
}


glm::vec3 DrawablesManager::getPivotPoint( UserID userID ) const
{
    switch( getUserSelection( userID )->getPivotPointMode() ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
        case PivotPointMode::MEDIAN_POINT:
            return glm::vec3( getUserSelection( userID )->getCentroid() );
        break;
        default:
            return glm::vec3( 0.0f, 0.0f, 0.0f );
        break;
    }
}


bool DrawablesManager::existsDrawable( const ResourceID& id ) const
{
    DrawablesSelections::const_iterator it;

    for( it = drawablesSelections_.begin(); it != drawablesSelections_.end(); it++ ){
        if( it->second->existsDrawable( id ) ){
            return true;
        }
    }
    return false;
}


/***
 * 4. Setters
 ***/

void DrawablesManager::displayEdges( MeshEdgesDisplayFrequency frequency )
{
    switch( frequency ){
        case MeshEdgesDisplayFrequency::ALWAYS:
            nonSelectedDrawables_->displayEdges( true );
        break;
        case MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED:
            nonSelectedDrawables_->displayEdges( false );
        break;
    }
}


/***
 * 5. Drawables administration
 ***/

ResourceID DrawablesManager::addDrawable( DrawablePtr drawable )
{
    //takeOpenGLContext();

    return localDrawablesSelection_->addDrawable( drawable );
}

void DrawablesManager::addDrawable( UserID userID, DrawablePtr drawable, ResourceID drawableID )
{
    //takeOpenGLContext();

    getUserSelection( userID )->addDrawable( drawableID, drawable );
}


void DrawablesManager::deleteSelection()
{
    deleteSelection( localUserID() );

    // Send Command to the server.
    requestSelectionDeletion();
}


void DrawablesManager::deleteSelection( const unsigned int& userId )
{
    getUserSelection( userId )->erase();

    notifyObservers();

    //emit renderNeeded();
}


/***
 * 5. Selections management
 ***/

void DrawablesManager::addDrawablesSelection( UserID userID, const PackableColor& selectionBorderColor )
{
    drawablesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( userID, DrawablesSelectionPtr( new DrawablesSelection( selectionBorderColor.toVec4() ) ) ) );
}


/***
 * 6. Drawables (de)seletion.
 ***/

void DrawablesManager::selectDrawable( ResourceID drawableID )
{
    selectDrawable( drawableID, localUserID() );
}


void DrawablesManager::selectDrawable( ResourceID drawableID, UserID userID )
{
    // Retrieve user's selection.
    DrawablesSelection& userSelection = *( getUserSelection( userID ) );

    // Check if the desired drawable is among the non selected ones, and move
    // it to the user's selection in that case.
    nonSelectedDrawables_->moveDrawable( drawableID, userSelection );
}


void DrawablesManager::unselectAll()
{
    unselectAll( localUserID() );

    requestSelectionUnlock();
}


void DrawablesManager::unselectAll( UserID userID )
{
    DrawablesSelection& userSelection = *( getUserSelection( userID ) );

    // Move all drawables from user selection to non selected set.
    userSelection.moveAll( *nonSelectedDrawables_ );
}



ResourceID DrawablesManager::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection, glm::vec3& worldCollisionPoint )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    ResourceID closestObject;

    DrawablesSelection& userSelection = *( getUserSelection( localUserID() ) );

    r1 = glm::normalize( r1 );

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        unselectAll();
    }

    // Check if the given ray intersect any of the non selected drawables.
    if( nonSelectedDrawables_->intersect( r0, r1, closestObject, minT ) ){
        // A non selected drawable has been intersected.
        log_->debug( "Object picked\n" );

        // Request to the server the lock of the intersected drawable.
        requestResourceLock( closestObject );

        // Save the collision point (in world coordinates) for returning it to
        // caller.
        worldCollisionPoint = r0 + r1 * minT;
    }else{
        // If user dind't selected any non-selected drawable, check if he / she
        // clicked on an already selected one.
        if( userSelection.intersect( r0, r1, closestObject, minT ) ){
            log_->debug( "RETURN 0\n" );
            ////emit renderNeeded();
            return NO_RESOURCE;
        }else{
            log_->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
            unselectAll();
        }

        // Even if no object is collided, we return in "worldCollisionPoint"
        // the "collision" with the near plane.
        //worldCollisionPoint = r0;
        worldCollisionPoint = glm::vec3( 0.0f );
    }

    // TODO: Send a request to server even when trying to select another
    // user's drawables?

    return closestObject;
}


/***
 * 10. Drawing
 ***/

void DrawablesManager::drawAll( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    DrawablesSelections::const_iterator it;

    // Draw the user's selections.
    for( it = drawablesSelections_.begin(); it != drawablesSelections_.end(); it++  ){
        (it->second)->draw( openGL, viewMatrix, projectionMatrix );
    }
}


/***
 * 11. Command execution
 ***/

void DrawablesManager::executeRemoteSelectionCommand( SelectionCommandConstPtr command )
{
    const SelectionTransformationCommand* selectionTransformation = nullptr;
    std::array< float, 3 > transformationVector;

    switch( command->getType() ){
        case SelectionCommandType::SELECTION_TRANSFORMATION:
            // Cast to a SELECTION_TRANSFORMATION command.
            selectionTransformation = dynamic_cast< const SelectionTransformationCommand* >( command.get() );

            // Transform the user's selection.
            transformationVector = selectionTransformation->getTransformationVector();

            // Execute one transformation or another according to the requested
            // type.
            switch( selectionTransformation->getTransformationType() ){
                case SelectionTransformationCommandType::TRANSLATION:
                    getUserSelection( selectionTransformation->getUserID() )->translate( glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
                break;
                case SelectionTransformationCommandType::ROTATION:
                    getUserSelection( selectionTransformation->getUserID() )->rotate( selectionTransformation->getTransformationAngle(), glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
                break;
                case SelectionTransformationCommandType::SCALE:
                    getUserSelection( selectionTransformation->getUserID() )->scale( glm::vec3( transformationVector[0], transformationVector[1], transformationVector[2] ) );
                break;
            }
        break;
    }
}


void DrawablesManager::executeRemoteParameterChangeCommand( UserParameterChangeCommandConstPtr command )
{
    // Change parameter.
    switch( command->getParameterType() ){
        case ParameterType::PIVOT_POINT_MODE:
            getUserSelection( command->getUserID() )->setPivotPointMode( command->getPivotPointMode() );
        break;
    }
}


/***
 * 8. Auxiliar methods
 ***/

bool DrawablesManager::hasChangedSinceLastQuery()
{
    DrawablesSelections::iterator it;

    for( it = drawablesSelections_.begin(); it != drawablesSelections_.end(); it++ ){
        if( it->second->hasChangedSinceLastQuery() ){
            return true;
        }
    }

    return false;
}


void DrawablesManager::highlightProperty( const void *property )
{
    for( auto drawablesSelection : drawablesSelections_ ){
        drawablesSelection.second->highlighDrawableProperty( property );
    }
}

/***
 * 12. Resource management
 ***/

void DrawablesManager::lockResource( const ResourceID& resourceID, UserID userID )
{
    selectDrawable( resourceID, userID );
}

void DrawablesManager::unlockResourcesSelection( UserID userID )
{
    unselectAll( userID );
}

void DrawablesManager::deleteResourcesSelection( UserID userID )
{
    deleteSelection( userID );
}



} // namespace como
