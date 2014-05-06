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

const unsigned int TRANSFORMATION_FLOAT_PRECISION = 10000;

/***
 * 1. Construction
 ***/

DrawablesManager::DrawablesManager( ServerInterfacePtr server, UserID localUserID, const std::uint8_t* localSelectionBorderColor, std::string primitivesDirPath, shared_ptr< QOpenGLContext > oglContext, LogPtr log ) :
    Changeable( true ),
    nonSelectedDrawables_( glm::vec4( 0.0f ) ),
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

    // Create an empty drawables selection for the local user.
    localDrawablesSelection_ = LocalDrawablesSelectionPtr( new LocalDrawablesSelection( localUserID_, selectionColor, server_ ) );

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
    switch( drawablesSelections_.at( userID )->getPivotPointMode() ){
        case PivotPointMode::INDIVIDUAL_CENTROIDS:
        case PivotPointMode::MEDIAN_POINT:
            return glm::vec3( drawablesSelections_.at( userID )->getCentroid() );
        break;
        default:
            return glm::vec3( 0.0f, 0.0f, 0.0f );
        break;
    }
}


/***
 * 4. Setters
 ***/

void DrawablesManager::setPivotPointMode( PivotPointMode pivotPointMode )
{
    // Set user's pivot point mode.
    setPivotPointMode( pivotPointMode, localUserID_ );

    // Send the command to the server.
    server_->sendCommand( CommandConstPtr( new ParameterChangeCommand( localUserID_, pivotPointMode ) ) );
}


void DrawablesManager::setPivotPointMode( PivotPointMode pivotPointMode, UserID userID )
{
    getUserSelection( userID )->setPivotPointMode( pivotPointMode );
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

    drawablesSelections_.at( userID )->addDrawable( drawableID, drawable );
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

    log_->debug( "Adding primitive (", primitivePaths_.at( primitiveID ), ") to scene\n" );

    // Build the "absolute" path to the specification file of the
    // primitive used for building this mesh.
    std::string primitivePath = primitivesDirPath_ + '/' + primitivePaths_.at( primitiveID );

    // Create the mesh.
    DrawablePtr drawable = DrawablePtr( new Mesh( primitivePath.c_str(), color ) );

    // Add the mest to the scene.
    PackableDrawableID drawableID = addDrawable( drawable );

    // Send the command to the server.
    server_->sendCommand( CommandConstPtr( new MeshCreationCommand( localUserID_, drawableID, primitiveID, color ) ) );
}


// FIXME: Duplicated code.
void DrawablesManager::addMesh( UserID userID, PrimitiveID primitiveID, const std::uint8_t* color, PackableDrawableID drawableID )
{
    try {
        //takeOpenGLContext();

        log_->debug( "Adding primitive (", primitivePaths_.at( primitiveID ), ") to scene\n" );

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
    bool drawableFound = false;

    // Retrieve user's selection.
    DrawablesSelection& userSelection = *( drawablesSelections_.at( userID ) );

    // Check if the desired drawable is among the non selected ones, and move
    // it to the user's selection in that case.
    drawableFound = nonSelectedDrawables_.moveDrawable( drawableID, userSelection );

    if( !drawableFound ){
        throw std::runtime_error( "ERROR when selecting drawable. Drawable not found" );
    }

    setChanged();
}


void DrawablesManager::unselectAll()
{
    unselectAll( localUserID_ );

    // Send command to the server.
    server_->sendCommand( CommandConstPtr( new FullDeselectionCommand( localUserID_ ) ) );
}


void DrawablesManager::unselectAll( UserID userID )
{
    DrawablesSelection& userSelection = *( drawablesSelections_.at( userID ) );

    // Move all drawables from user selection to non selected set.
    userSelection.moveAll( nonSelectedDrawables_ );

    setChanged();
}



PackableDrawableID DrawablesManager::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    PackableDrawableID closestObject;

    DrawablesSelection& userSelection = *( drawablesSelections_.at( localUserID_ ) );

    r1 = glm::normalize( r1 );

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        unselectAll();
    }

    // Check if the given ray intersect any of the non selected drawables.
    if( nonSelectedDrawables_.intersect( r0, r1, closestObject, minT ) ){
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

    /*
    // Iterate over all non selected drawables and check if the given ray intersects
    // them or not. Get the closest object.
    DrawablesSelection::iterator it;
    for( it = nonSelectedDrawables_.begin(); it != nonSelectedDrawables_.end(); it++ ){
        it->second->intersects( r0, r1, t );
        if( ( t >= 0.0f ) && (t < minT ) ){
            // New closest object, get its ID and distance.
            closestObject = it->first;
            minT = t;
        }
    }

    // If user dind't selected any non-selected drawable, check if he / she
    // clicked on an already selected one.
    if( minT == MAX_T ){
        for( it = userSelection.begin(); it != userSelection.end(); it++ ){
            it->second->intersects( r0, r1, t );
            if( ( t > 0.0f ) && (t < minT ) ){
                // New closest object, get its index and distance.
                closestObject = it->first;
                minT = t;
                log_->debug( "RETURN 0\n" );
                //emit renderNeeded();
                return NULL_DRAWABLE_ID;
            }
        }
    }



    // If there were intersections, select the closest one.
    if( minT < MAX_T ){
        log_->debug( "FINAL CLOSEST OBJECT: (", closestObject.creatorID, ", ", closestObject.drawableIndex, ")\n",
                     "\t min t: ", minT, ")\n",
                     "\t min distance: ", glm::distance( glm::vec3( 0.0f, 0.0f, 0.0f ), r1 * t ), "\n" );

        // Send a DRAWABLE_SELECTION command to the server.
        server_->sendCommand( CommandConstPtr(
                                 new DrawableSelectionCommand( localUserID_,
                                                     closestObject,
                                                     addToSelection ) ) );

        // Insert the selected drawable's ID in a queue of pending selections.
        localUserPendingSelections_.push( closestObject );

        //selectDrawable( closestObject );
    }else{
        log_->debug( "NO CLOSEST OBJECT. Unselecting all\n" );
        unselectAll();
    }
    */

    setChanged();

    return closestObject;
}


/***
 * 7. Drawables selection transformations
 ***/

void DrawablesManager::translateSelection( glm::vec3 direction )
{
    CommandPtr translationCommand( new SelectionTransformationCommand( localUserID_ ) );

    // Round transformation magnitude to 3 decimal places.
    roundTransformationMagnitude( direction[0], direction[1], direction[2] );

    // Translate locally (client).
    translateSelection( direction, localUserID_ );

    // Send command to the server.
    ( dynamic_cast< SelectionTransformationCommand* >( translationCommand.get() ) )->setTranslation( &direction[0] );
    server_->sendCommand( translationCommand );
}


void DrawablesManager::translateSelection( glm::vec3 direction, UserID userID )
{
    // Get the user's selection and translate it.
    getUserSelection( userID )->translate( direction );

    // //emit a signal indicating that the DrawablesManager has been changed and so it needs
    // a render.
    ////emit renderNeeded();
}


void DrawablesManager::rotateSelection( GLfloat angle, glm::vec3 axis )
{
    CommandPtr rotationCommand( new SelectionTransformationCommand( localUserID_ ) );

    // Round transformation magnitude to 3 decimal places.
    roundTransformationMagnitude( angle, axis[0], axis[1], axis[2] );

    // Rotate locally (client).
    rotateSelection( angle, axis, localUserID_ );

    // Send command to the server.
    ( dynamic_cast< SelectionTransformationCommand* >( rotationCommand.get() ) )->setRotation( angle, &axis[0] );
    server_->sendCommand( rotationCommand );
}


void DrawablesManager::rotateSelection( GLfloat angle, glm::vec3 axis, UserID userID )
{
    // Get the user's selection and rotate it.
    getUserSelection( userID )->rotate( angle, axis );

    // //emit a signal indicating that the DrawablesManager has been changed and so it needs
    // a render.
    ////emit renderNeeded();
}


void DrawablesManager::scaleSelection( glm::vec3 scaleFactors )
{
    CommandPtr scaleCommand( new SelectionTransformationCommand( localUserID_ ) );

    // Round transformation magnitude to 3 decimal places.
    roundTransformationMagnitude( scaleFactors[0], scaleFactors[1], scaleFactors[2] );

    // Scale locally (client).
    scaleSelection( scaleFactors, localUserID_ );

    // Send command to the server.
    ( dynamic_cast< SelectionTransformationCommand* >( scaleCommand.get() ) )->setScale( &scaleFactors[0] );
    server_->sendCommand( scaleCommand );
}


void DrawablesManager::scaleSelection( glm::vec3 scaleFactors, UserID userID )
{
    // Get the user's selection and scale it.
    getUserSelection( userID )->scale( scaleFactors );

    // //emit a signal indicating that the DrawablesManager has been changed and so it needs
    // a render.
    ////emit renderNeeded();
}

/*
void DrawablesManager::rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot )
{
    DrawablesSelection::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->rotate( angle, axis, pivot );
    }
    //emit renderNeeded();
}
*/

void DrawablesManager::drawAll( const glm::mat4& viewProjMatrix ) const
{
    DrawablesSelections::const_iterator it;

    // Draw the non selected drawables.
    nonSelectedDrawables_.draw( viewProjMatrix );

    // Draw the user's selections.
    for( it = drawablesSelections_.begin(); it != drawablesSelections_.end(); it++  ){
        // FIXME: Use selection color.
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
                    translateSelection( glm::vec3( transf[0], transf[1], transf[2] ), selectionTransformation->getUserID() );
                break;
                case SelectionTransformationCommandType::ROTATION:
                    rotateSelection( selectionTransformation->getAngle(), glm::vec3( transf[0], transf[1], transf[2] ), selectionTransformation->getUserID() );
                break;
                case SelectionTransformationCommandType::SCALE:
                    scaleSelection( glm::vec3( transf[0], transf[1], transf[2] ), selectionTransformation->getUserID() );
                break;
            }
        break;
    }
}


/***
 * 8. Auxiliar methods
 ***/

void DrawablesManager::roundTransformationMagnitude( float& vx, float& vy, float& vz )
{
    // Round transformation magnitude to 3 decimal places.
    // http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
    vx = floorf( vx * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    vy = floorf( vy * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    vz = floorf( vz * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
}


void DrawablesManager::roundTransformationMagnitude( float& angle, float& vx, float& vy, float& vz )
{
    // Round transformation magnitude to 3 decimal places.
    // http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
    angle = floorf( angle * TRANSFORMATION_FLOAT_PRECISION + 0.5) / TRANSFORMATION_FLOAT_PRECISION;
    vx = floorf( vx * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    vy = floorf( vy * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
    vz = floorf( vz * TRANSFORMATION_FLOAT_PRECISION + 0.5f) / TRANSFORMATION_FLOAT_PRECISION;
}


void DrawablesManager::registerPrimitivePath( PrimitiveID primitiveID, std::string primitiveRelPath )
{
    // Create a new entry (ID, relative path) for the recently added primitive.
    primitivePaths_[primitiveID] = primitiveRelPath;
}

} // namespace como
