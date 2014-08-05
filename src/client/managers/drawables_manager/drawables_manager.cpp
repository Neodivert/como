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

DrawablesManager::DrawablesManager( ServerInterfacePtr server, UserID localUserID, const PackableColor& localSelectionBorderColor, std::string primitivesDirPath, shared_ptr< QOpenGLContext > oglContext, LogPtr log ) :
    AbstractChangeable(),
    ResourcesManager( server ),
    nonSelectedDrawables_( new DrawablesSelection( glm::vec4( 0.0f ) ) ),
    localUserID_( localUserID ),
    primitivesDirPath_( primitivesDirPath ),
    oglContext_( oglContext ),
    log_( log ),
    newMeshesDisplayVertexNormals_( false )
{
    glm::vec4 selectionColor = localSelectionBorderColor.toVec4();

    // Add a selection of unselected drawables to the map of selections as a
    // selection associated to NO_USER).
    drawablesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( NO_USER, nonSelectedDrawables_ ) );

    // Create an empty drawables selection for the local user.
    localDrawablesSelection_ = LocalDrawablesSelectionPtr( new LocalDrawablesSelection( localUserID_, selectionColor, server ) );

    // Insert the recently created selection to the selections map.
    drawablesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( localUserID_, localDrawablesSelection_ ) );

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
    return getPivotPoint( localUserID_ );
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


ElementsMeetingCondition DrawablesManager::displaysVertexNormals() const
{
    const ElementsMeetingCondition firstSelectionValue =
            drawablesSelections_.begin()->second->meshes()->displaysVertexNormals();

    if( firstSelectionValue == ElementsMeetingCondition::SOME ){
        return ElementsMeetingCondition::SOME;
    }

    for( auto drawablesSelection : drawablesSelections_ ){
        if( drawablesSelection.second->meshes()->displaysVertexNormals() != firstSelectionValue ){
            return ElementsMeetingCondition::SOME;
        }
    }

    return firstSelectionValue;
}

unsigned int DrawablesManager::getTotalMeshes() const
{
    unsigned int totalMeshes = 0;

    for( auto drawablesSelection : drawablesSelections_ ){
        totalMeshes += drawablesSelection.second->meshes()->size();
    }

    return totalMeshes;
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


void DrawablesManager::displayVertexNormals( bool display )
{
    newMeshesDisplayVertexNormals_ = display;

    for( auto drawablesSelection : drawablesSelections_ ){
        drawablesSelection.second->displayVertexNormals( display );
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


ResourceID DrawablesManager::createMesh( MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials )
{
    DrawablePtr mesh( new Mesh( vertexData, oglData, polygonsGroups, materials, newMeshesDisplayVertexNormals_ ) );

    return addDrawable( mesh );
}


void DrawablesManager::createMesh( ResourceID meshID, MeshVertexData vertexData, MeshOpenGLData oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials )
{
    DrawablePtr mesh( new Mesh( vertexData, oglData, polygonsGroups, materials, newMeshesDisplayVertexNormals_ ) );

    addDrawable( meshID.getCreatorID(), mesh, meshID );
}


void DrawablesManager::deleteSelection()
{
    deleteSelection( localUserID_ );

    // Send Command to the server.
    CommandPtr deleteSelectionCommand( new SelectionDeletionCommand( localUserID_ ) );
    sendCommandToServer( deleteSelectionCommand );
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
    selectDrawable( drawableID, localUserID_ );
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
    unselectAll( localUserID_ );

    // Send command to the server.
    sendCommandToServer( CommandConstPtr( new FullDeselectionCommand( localUserID_ ) ) );
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

    DrawablesSelection& userSelection = *( getUserSelection( localUserID_ ) );

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

        // Send a DRAWABLE_SELECTION command to the server.
        sendCommandToServer( CommandConstPtr(
                                 new DrawableSelectionCommand( localUserID_,
                                                     closestObject,
                                                     addToSelection ) ) );

        // Insert the selected drawable's ID in a queue of pending selections.
        localUserPendingSelections_.push( closestObject );

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

void DrawablesManager::executeRemoteDrawableCommand( DrawableCommandConstPtr command )
{
    const DrawableSelectionCommand* selectDrawable = nullptr;

    switch( command->getType() ){
        case DrawableCommandType::DRAWABLE_SELECTION:
            // Cast to a DRAWABLE_SELECTION command.
            selectDrawable = dynamic_cast< const DrawableSelectionCommand* >( command.get() );

            // Select drawable.
            this->selectDrawable( selectDrawable->getResourceID(), selectDrawable->getUserID() );
        break;
    }
}

void DrawablesManager::executeRemoteSelectionCommand( SelectionCommandConstPtr command )
{
    const SelectionResponseCommand* selectionResponse = nullptr;
    const SelectionTransformationCommand* selectionTransformation = nullptr;
    std::array< float, 3 > transformationVector;

    bool selectionConfirmed;
    unsigned int i;

    ResourceID pendingSelection;

    switch( command->getType() ){
        case SelectionCommandType::SELECTION_DELETION:
            // Delete user selection.
            deleteSelection( command->getUserID() );
        break;

        case SelectionCommandType::SELECTION_RESPONSE:
            // Cast to a SELECTION_RESPONSE command.
            selectionResponse = dynamic_cast< const SelectionResponseCommand* >( command.get() );

            for( i = 0; i < selectionResponse->getNSelections(); i++ ){
                selectionConfirmed = selectionResponse->getSelectionConfirmed() & (1 << i);
                if( selectionConfirmed ){
                    pendingSelection = localUserPendingSelections_.front();
                    selectDrawable( pendingSelection );
                    //selectDrawable( selectDrawable->getResourceID() );
                }
                localUserPendingSelections_.pop();
            }
        break;

        case SelectionCommandType::FULL_DESELECTION:
            // Unselect all.
            unselectAll( command->getUserID() );
        break;

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

void DrawablesManager::registerPrimitivePath( ResourceID primitiveID, std::string primitiveRelPath )
{
    // Create a new entry (ID, relative path) for the recently added primitive.
    primitivePaths_[primitiveID] = primitiveRelPath;
}


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

} // namespace como
