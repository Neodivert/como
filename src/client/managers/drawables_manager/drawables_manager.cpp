/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
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

DrawablesManager::DrawablesManager( ServerInterfacePtr server, UserID localUserID, const std::uint8_t* localSelectionBorderColor, std::string primitivesDirPath, shared_ptr< QOpenGLContext > oglContext, LogPtr log ) :
    AbstractChangeable(),
    nonSelectedDrawables_( new DrawablesSelection( glm::vec4( 0.0f ) ) ),
    server_( server ),
    localUserID_( localUserID ),
    primitivesDirPath_( primitivesDirPath ),
    oglContext_( oglContext ),
    log_( log )
{
    glm::vec4 selectionColor(
                    localSelectionBorderColor[0],
                    localSelectionBorderColor[1],
                    localSelectionBorderColor[2],
                    localSelectionBorderColor[3]
                );

    // Add a selection of unselected drawables to the map of selections as a
    // selection associated to NO_USER).
    drawablesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( NO_USER, nonSelectedDrawables_ ) );

    // Create an empty drawables selection for the local user.
    localDrawablesSelection_ = LocalDrawablesSelectionPtr( new LocalDrawablesSelection( localUserID_, selectionColor, server ) );

    // Insert the recently created selection to the selections map.
    drawablesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( localUserID_, localDrawablesSelection_ ) );
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


/***
 * 5. Drawables administration
 ***/

PackableDrawableID DrawablesManager::addDrawable( DrawablePtr drawable )
{
    //takeOpenGLContext();

    return localDrawablesSelection_->addDrawable( drawable );
}


void DrawablesManager::addDrawable( UserID userID, DrawablePtr drawable, PackableDrawableID drawableID )
{
    //takeOpenGLContext();

    getUserSelection( userID )->addDrawable( drawableID, drawable );
}


/*
void DrawablesManager::addMesh( PrimitiveID primitiveID, QColor color )
{
    std::uint8_t color8[4];
    int r, g, b;

    // Get the RGB components (int) of the color.
    color.getRgb( &r, &g, &b );

    // Turn the previous RGB components into a UINT8 vector.
    color8[0] = static_cast< std::uint8_t >( r );
    color8[1] = static_cast< std::uint8_t >( g );
    color8[2] = static_cast< std::uint8_t >( b );
    color8[3] = 255;

    // Add the primitive to the scene.
    addMesh( primitiveID, color8 );
}
*/

// FIXME: Duplicated code.
void DrawablesManager::addMesh( PrimitiveID primitiveID, const std::uint8_t* color )
{
    // FIXME: Is this necessary?
    //takeOpenGLContext();
    //oglContext_->makeCurrent( this );

    // Build the "absolute" path to the specification file of the
    // primitive used for building this mesh.
    std::string primitivePath = primitivesDirPath_ + '/' + primitivePaths_.at( primitiveID );

    // Create the mesh.
    DrawablePtr drawable = DrawablePtr( new Mesh( primitivePath.c_str(), color ) );

    // Add the mest to the scene.
    PackableDrawableID drawableID = addDrawable( drawable );

    // Send the command to the server.
    server_->sendCommand( CommandConstPtr( new PrimitiveMeshCreationCommand( drawableID, primitiveID, color ) ) );
}


// FIXME: Duplicated code.
void DrawablesManager::addMesh( UserID userID, PrimitiveID primitiveID, const std::uint8_t* color, PackableDrawableID drawableID )
{
    try {
        //takeOpenGLContext();

        // Build the "absolute" path to the specification file of the
        // primitive used for building this mesh.
        std::string primitivePath = primitivesDirPath_ + '/' + primitivePaths_.at( primitiveID );

        // Create the mesh.
        DrawablePtr drawable = DrawablePtr( new Mesh( primitivePath.c_str(), color ) );

        // Add the mesh to the scene.
        addDrawable( userID, drawable, drawableID );
    }catch( std::exception& ex ){
        std::cerr << ex.what() << std::endl;
        throw;
    }
}


void DrawablesManager::addDirectionalLight( PackableDrawableID lightID, glm::vec3 lightColor )
{
    addDrawable( lightID.creatorID.getValue(), DrawablePtr( new DirectionalLight( lightColor ) ), lightID );
}


void DrawablesManager::deleteSelection()
{
    deleteSelection( localUserID_ );

    // Send Command to the server.
    CommandPtr deleteSelectionCommand( new SelectionDeletionCommand( localUserID_ ) );
    server_->sendCommand( deleteSelectionCommand );
}


void DrawablesManager::deleteSelection( const unsigned int& userId )
{
    getUserSelection( userId )->clear();

    //emit renderNeeded();
}


/***
 * 5. Selections management
 ***/

void DrawablesManager::addDrawablesSelection( UserID userID, const std::uint8_t* selectionBorderColor )
{
    glm::vec4 floatSelectionBorderColor(
                selectionBorderColor[0],
                selectionBorderColor[1],
                selectionBorderColor[2],
                selectionBorderColor[3]
            );

    drawablesSelections_.insert( std::pair< UserID, DrawablesSelectionPtr >( userID, DrawablesSelectionPtr( new DrawablesSelection( floatSelectionBorderColor ) ) ) );
}


/***
 * 6. Drawables (de)seletion.
 ***/

void DrawablesManager::selectDrawable( PackableDrawableID drawableID )
{
    selectDrawable( drawableID, localUserID_ );
}


void DrawablesManager::selectDrawable( PackableDrawableID drawableID, UserID userID )
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
    server_->sendCommand( CommandConstPtr( new FullDeselectionCommand( localUserID_ ) ) );
}


void DrawablesManager::unselectAll( UserID userID )
{
    DrawablesSelection& userSelection = *( getUserSelection( userID ) );

    // Move all drawables from user selection to non selected set.
    userSelection.moveAll( *nonSelectedDrawables_ );
}



PackableDrawableID DrawablesManager::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    PackableDrawableID closestObject;

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
        server_->sendCommand( CommandConstPtr(
                                 new DrawableSelectionCommand( localUserID_,
                                                     closestObject,
                                                     addToSelection ) ) );

        // Insert the selected drawable's ID in a queue of pending selections.
        localUserPendingSelections_.push( closestObject );
    }else{
        // If user dind't selected any non-selected drawable, check if he / she
        // clicked on an already selected one.
        if( userSelection.intersect( r0, r1, closestObject, minT ) ){
            log_->debug( "RETURN 0\n" );
            ////emit renderNeeded();
            return NULL_DRAWABLE_ID;
        }else{
            log_->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
            unselectAll();
        }
    }

    return closestObject;
}


/***
 * 10. Drawing
 ***/

void DrawablesManager::drawAll( const glm::mat4& viewProjMatrix ) const
{
    DrawablesSelections::const_iterator it;

    // Draw the user's selections.
    for( it = drawablesSelections_.begin(); it != drawablesSelections_.end(); it++  ){
        (it->second)->draw( viewProjMatrix );
    }
}


/***
 * 11. Command execution
 ***/

void DrawablesManager::executeRemoteSelectionCommand( SelectionCommandConstPtr command )
{
    const SelectionResponseCommand* selectionResponse = nullptr;
    const SelectionTransformationCommand* selectionTransformation = nullptr;
    const float* transf = nullptr;
    bool selectionConfirmed;
    unsigned int i;

    PackableDrawableID pendingSelection;

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
                    //selectDrawable( selectDrawable->getDrawableID() );
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
            transf = selectionTransformation->getTransformationMagnitude();

            // Execute one transformation or another according to the requested
            // type.
            switch( selectionTransformation->getTransformationType() ){
                case SelectionTransformationCommandType::TRANSLATION:
                    getUserSelection( selectionTransformation->getUserID() )->translate( glm::vec3( transf[0], transf[1], transf[2] ) );
                break;
                case SelectionTransformationCommandType::ROTATION:
                    getUserSelection( selectionTransformation->getUserID() )->rotate( selectionTransformation->getAngle(), glm::vec3( transf[0], transf[1], transf[2] ) );
                break;
                case SelectionTransformationCommandType::SCALE:
                    getUserSelection( selectionTransformation->getUserID() )->scale( glm::vec3( transf[0], transf[1], transf[2] ) );
                break;
            }
        break;

        case SelectionCommandType::MESH_COLOR_CHANGE:
            getUserSelection( command->getUserID() )->setMeshColor( dynamic_cast< const MeshColorChangeCommand* >( command.get() )->getMeshColor() );
        break;
    }
}


void DrawablesManager::executeRemoteParameterChangeCommand( ParameterChangeCommandConstPtr command )
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

void DrawablesManager::registerPrimitivePath( PrimitiveID primitiveID, std::string primitiveRelPath )
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

} // namespace como
