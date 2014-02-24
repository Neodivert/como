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

#include "scene.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

Scene::Scene( LogPtr log ) :
    log_( log ),
    localUserID_( 1 ), // Will be updated to its final value in Scene::connect().
    localUserNextDrawableIndex_( 1 ),
    server_( log_ )
{
    initOpenGL();

    initLinesBuffer();

    // Set the default contour color for those drawable that are
    // not selected by any user.
    defaultContourColor[0] = 0.0f;
    defaultContourColor[1] = 0.0f;
    defaultContourColor[2] = 0.0f;
    defaultContourColor[3] = 0.0f;

    // Set the background color.
    setBackgroundColor( 0.9f, 0.9f, 0.9f, 0.9f );

    // FIXME: Study why this is necessary.
    qRegisterMetaType< CommandConstPtr >();
    qRegisterMetaType< CommandConstPtr >( "CommandConstPtr" );

    // Signal / slot: when a command is received from server, execute it on
    // the local scene.
    QObject::connect( &server_, &ServerInterface::commandReceived, this, &Scene::executeRemoteCommand );

    checkOpenGL( "Scene - constructor\n" );
}


Scene::~Scene()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &linesVBO );
    glDeleteVertexArrays( 1, &linesVAO );
}


void Scene::initOpenGL()
{
    // Create a surface format for OpenGL 4.2 Core.
    // http://stackoverflow.com/questions/11000014/cant-set-desired-opengl-version-in-qglwidget
    QSurfaceFormat format;
    format.setMajorVersion( 4 );
    format.setMinorVersion( 2 );
    format.setProfile( QSurfaceFormat::CoreProfile );
    format.setRenderableType( QSurfaceFormat::OpenGL );

    // Initialize this offscreen surface with previous format.
    setFormat( format );
    destroy();
    create();

    // Create an empty OpenGL context and make it use this surface's format.
    oglContext_ = shared_ptr< QOpenGLContext >( new QOpenGLContext );
    oglContext_->setFormat( format );

    // Initialize the OpenGL context and make it the current one.
    oglContext_->create();
    oglContext_->makeCurrent( this );

    // Obtain a functions object and resolve all entry points
    QAbstractOpenGLFunctions* oglFunctions = oglContext_->versionFunctions();
    if ( !oglFunctions ) {
        qWarning( "Could not obtain OpenGL versions object" );
        exit( 1 );
    }
    oglFunctions->initializeOpenGLFunctions();

    // Load shaders
    msl::ShaderLoader* shaderLoader = msl::ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    shaderLoader->destroy();

    // Set clear color.
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    // Set OpenGL depth test.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );
}


void Scene::initLinesBuffer()
{
    GLint currentShaderProgram;
    GLint vPosition;

    const GLfloat x0 = -0.5f;
    const GLfloat y0 = -0.5f;
    const GLfloat z0 = 0.5f;

    const GLfloat x1 = 0.5f;
    const GLfloat y1 = 0.5f;
    const GLfloat z1 = -0.5f;

    GLfloat linesData[] =
    {
        // World X axis
        x0, y0, z0,
        x1, y0, z0,

        // World Y axis
        x0, y0, z0,
        x0, y1, z0,

        // World Z axis
        x0, y0, z0,
        x0, y0, z1,

        // X guide axis
        -100.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 0.0f,

        // Y guide axis
        0.0f, -100.0f, 0.0f,
        0.0f, 100.0f, 0.0f,

        // Z guide axis
        0.0f, 0.0f, -100.0f,
        0.0f, 0.0f, 100.0f,

        // Auxiliar line for rotations and scales
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    const int LINES_BUFFER_SIZE = 42 * sizeof( GLfloat );

    linesBufferOffsets[WORLD_AXIS] = 0;
    linesBufferOffsets[GUIDE_AXIS] = 6;
    linesBufferOffsets[TRANSFORM_GUIDE_LINE] = 12;

    // Get the position of the vertex shader variable "vPosition"
    // for the current shader program.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    vPosition = glGetAttribLocation( currentShaderProgram, "vPosition" );
    if( vPosition == GL_INVALID_OPERATION ){
        log_->error( "Error getting layout of \"position\"\n" );
    }
    // Get location of uniform shader variable "color".
    uniformColorLocation = glGetUniformLocation( currentShaderProgram, "color" );

    // Set a VBO for the world axis rects.
    glGenBuffers( 1, &linesVBO );
    glBindBuffer( GL_ARRAY_BUFFER, linesVBO );
    glBufferData( GL_ARRAY_BUFFER, LINES_BUFFER_SIZE, linesData, GL_DYNAMIC_DRAW );

    // Set a VAO for the world axis rects.
    glGenVertexArrays( 1, &linesVAO );
    glBindVertexArray( linesVAO );

    // By using the previous "vPosition" position, specify the location and data format of
    // the array of vertex positions.
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( vPosition );
}


/***
 * 2. Users administration
 ***/

void Scene::removeUser( UserID userID )
{
    // Unselect all drawables selected by user.
    unselectAll( userID );

    // Remove user from scene.
    if( users_.erase( userID ) ){
        emit userDisconnected( userID );
    }
}


/***
 * 3. Getters
 ***/

glm::vec3 Scene::getPivotPoint() const
{
    return getPivotPoint( localUserID_ );
}


glm::vec3 Scene::getPivotPoint( UserID userID ) const
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


shared_ptr< QOpenGLContext > Scene::getOpenGLContext() const
{
    return oglContext_;
}


DrawablesSelection* Scene::getUserSelection()
{
    return getUserSelection( localUserID_ );
}


DrawablesSelection* Scene::getUserSelection( UserID userID )
{
    return &( users_.at( userID )->selection );
}


const DrawablesSelection* Scene::getUserSelection() const
{
    return getUserSelection( localUserID_ );
}


const DrawablesSelection* Scene::getUserSelection( UserID userID ) const
{
    return &( users_.at( userID )->selection );
}



/***
 * 4. Setters
 ***/

void Scene::setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a )
{
    glClearColor( r, g, b, a );
}


void Scene::setTransformGuideLine( glm::vec3 origin, glm::vec3 destiny )
{
    GLfloat* guideRectsBuffer = nullptr;
    unsigned int i = 0;

    glBindBuffer( GL_ARRAY_BUFFER, linesVBO );
    guideRectsBuffer = (GLfloat *)glMapBufferRange( GL_ARRAY_BUFFER, linesBufferOffsets[TRANSFORM_GUIDE_LINE]*3*sizeof( GLfloat ), 6*sizeof( GLfloat ), GL_MAP_WRITE_BIT );
    checkOpenGL( "Scene::setTransformGuideLine" );

    for( ; i<3; i++ ){
        guideRectsBuffer[i] = origin[i];
    }
    for( ; i<6; i++ ){
        guideRectsBuffer[i] = destiny[i-3];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
}


void Scene::setPivotPointMode( PivotPointMode pivotPointMode )
{
    // Set user's pivot point mode.
    setPivotPointMode( pivotPointMode, localUserID_ );

    // Send the command to the server.
    server_.sendCommand( CommandConstPtr( new ParameterChangeCommand( localUserID_, pivotPointMode ) ) );
}


void Scene::setPivotPointMode( PivotPointMode pivotPointMode, UserID userID )
{
    getUserSelection( userID )->setPivotPointMode( pivotPointMode );
}


/***
 * 5. Drawables administration
 ***/

void Scene::addDrawable( DrawablePtr drawable, DrawableID drawableID )
{
    takeOpenGLContext();

    nonSelectedDrawables.addDrawable( drawableID, drawable );

    emit renderNeeded();
}


void Scene::takeOpenGLContext()
{
    oglContext_->makeCurrent( this );
}


void Scene::addCube( QColor color )
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

    // Add the cube to the scene.
    addCube( color8 );
}


void Scene::addCube( const std::uint8_t* color )
{
    DrawableID drawableID;

    // Give a unique ID to the new drawable (bind it to the local user).
    drawableID.creatorID = localUserID_;
    drawableID.drawableIndex = localUserNextDrawableIndex_;

    // Increment the index for the next local user's drawable.
    localUserNextDrawableIndex_++;

    // Create the cube and add it to the scene.
    addCube( color, drawableID );

    // Send the command to the server.
    server_.sendCommand( CommandConstPtr( new CubeCreationCommand( localUserID_, drawableID, color ) ) );
}


void Scene::addCube( const std::uint8_t* color, DrawableID drawableID )
{
    try {
        takeOpenGLContext();

        // Create the cube.
        DrawablePtr drawable = DrawablePtr( new Cube( color ) );

        // Add the cube to the scene.
        addDrawable( drawable, drawableID );
    }catch( std::exception& ex ){
        std::cerr << ex.what() << std::endl;
        throw;
    }
}


/*
void Scene::addDrawable( DrawableType drawableType )
{
    DrawableID drawableID;

    // Give a unique ID to the new drawable (bind it to the local user).
    drawableID.creatorID = localUserID_;
    drawableID.drawableIndex = localUserNextDrawableIndex_;

    // Increment the index for the next local user's drawable.
    localUserNextDrawableIndex_++;

    // Add the drawable to the scene.
    addDrawable( drawableType, drawableID );

    switch( drawableType ){
        case DrawableType::CUBE:
            server_.sendCommand( CommandConstPtr( new CubeCreationCommand ) );
        break;
    }
}

void Scene::addDrawable( DrawableType drawableType, DrawableID drawableID )
{
    DrawablePtr drawable;
    Cube* cube;

    switch( drawableType ){
        case DrawableType::CUBE:
            cube = new Cube;
            drawable = DrawablePtr( cube );
        break;
    }

    addDrawable( drawable, drawableID );
}
*/

/***
 * 6. Drawables selection
 ***/

void Scene::selectDrawable( DrawableID drawableID )
{
    selectDrawable( drawableID, localUserID_ );
}

void Scene::selectDrawable( DrawableID drawableID, UserID userID )
{
    bool drawableFound = false;
    UsersMap::iterator currentUser;

    // Retrieve user's selection.
    DrawablesSelection& userSelection = users_.at( userID )->selection;

    // Check if the desired drawable is among the non selected ones, and move
    // it to the user's selection in that case.
    drawableFound = nonSelectedDrawables.moveDrawable( drawableID, userSelection );


    // If not found, search the desired drawable among the user's selections.
    if( !drawableFound ){
        currentUser = users_.begin();
        while( !drawableFound && ( currentUser != users_.end()) ){
            drawableFound = currentUser->second->selection.moveDrawable( drawableID, userSelection );
        }
    }

    if( !drawableFound ){
        throw std::runtime_error( "ERROR when selecting drawable. Drawable not found" );
    }

    emit renderNeeded();
}

/*
void Scene::selectAll()
{
    selectedDrawables.splice( selectedDrawables.end(), nonSelectedDrawables );

    updateSelectionCentroid();

    emit renderNeeded();
}
*/

void Scene::unselectAll()
{
    unselectAll( localUserID_ );

    // Send command to the server.
    server_.sendCommand( CommandConstPtr( new FullDeselectionCommand( localUserID_ ) ) );
}


void Scene::unselectAll( const unsigned int& userId )
{
    DrawablesSelection& userSelection = users_.at( userId )->selection;

    // Move all drawables from user selection to non selected set.
    userSelection.moveAll( nonSelectedDrawables );

    emit renderNeeded();
}



DrawableID Scene::selectDrawableByRayPicking( glm::vec3 r0, glm::vec3 r1, bool addToSelection )
{
    const float MAX_T = 9999999.0f;
    float minT = MAX_T;
    DrawableID closestObject;

    DrawablesSelection& userSelection = users_.at( localUserID_ )->selection;

    r1 = glm::normalize( r1 );

    // Does the user want to keep the actual set of selected objects and simply add
    // a new one? If that's NOT the case, we need to clear the set of selected drawables
    // first.
    if( !addToSelection ){
        unselectAll();
    }


    // Check if the given ray intersect any of the non selected drawables.
    if( nonSelectedDrawables.intersect( r0, r1, closestObject, minT ) ){
        // A non selected drawable has been intersected.
        log_->debug( "Object picked\n" );

        // Send a DRAWABLE_SELECTION command to the server.
        server_.sendCommand( CommandConstPtr(
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
            emit renderNeeded();
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
    for( it = nonSelectedDrawables.begin(); it != nonSelectedDrawables.end(); it++ ){
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
                emit renderNeeded();
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
        server_.sendCommand( CommandConstPtr(
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

    emit renderNeeded();

    return closestObject;
}


/***
 * 7. Transformations
 ***/

void Scene::translateSelection( glm::vec3 direction )
{
    CommandPtr translationCommand( new SelectionTransformationCommand( localUserID_ ) );

    // Round transformation magnitude to 3 decimal places.
    roundTransformationMagnitude( direction[0], direction[1], direction[2] );

    // Translate locally (client).
    translateSelection( direction, localUserID_ );

    // Send command to the server.
    ( dynamic_cast< SelectionTransformationCommand* >( translationCommand.get() ) )->setTranslation( &direction[0] );
    server_.sendCommand( translationCommand );
}


void Scene::translateSelection( glm::vec3 direction, UserID userID )
{
    // Get the user's selection and translate it.
    getUserSelection( userID )->translate( direction );

    // Emit a signal indicating that the scene has been changed and so it needs
    // a render.
    emit renderNeeded();
}


void Scene::rotateSelection( GLfloat angle, glm::vec3 axis )
{
    CommandPtr rotationCommand( new SelectionTransformationCommand( localUserID_ ) );

    // Round transformation magnitude to 3 decimal places.
    roundTransformationMagnitude( angle, axis[0], axis[1], axis[2] );

    // Rotate locally (client).
    rotateSelection( angle, axis, localUserID_ );

    // Send command to the server.
    ( dynamic_cast< SelectionTransformationCommand* >( rotationCommand.get() ) )->setRotation( angle, &axis[0] );
    server_.sendCommand( rotationCommand );
}


void Scene::rotateSelection( GLfloat angle, glm::vec3 axis, UserID userID )
{
    // Get the user's selection and rotate it.
    getUserSelection( userID )->rotate( angle, axis );

    // Emit a signal indicating that the scene has been changed and so it needs
    // a render.
    emit renderNeeded();
}


void Scene::scaleSelection( glm::vec3 scaleFactors )
{
    CommandPtr scaleCommand( new SelectionTransformationCommand( localUserID_ ) );

    // Round transformation magnitude to 3 decimal places.
    roundTransformationMagnitude( scaleFactors[0], scaleFactors[1], scaleFactors[2] );

    // Scale locally (client).
    scaleSelection( scaleFactors, localUserID_ );

    // Send command to the server.
    ( dynamic_cast< SelectionTransformationCommand* >( scaleCommand.get() ) )->setScale( &scaleFactors[0] );
    server_.sendCommand( scaleCommand );
}


void Scene::scaleSelection( glm::vec3 scaleFactors, UserID userID )
{
    // Get the user's selection and scale it.
    getUserSelection( userID )->scale( scaleFactors );

    // Emit a signal indicating that the scene has been changed and so it needs
    // a render.
    emit renderNeeded();
}

/*
void Scene::rotateSelection( const GLfloat& angle, const glm::vec3& axis, const glm::vec3& pivot )
{
    DrawablesSelection::iterator it = selectedDrawables.begin();

    for( ; it != selectedDrawables.end(); it++ )
    {
        (*it)->rotate( angle, axis, pivot );
    }
    emit renderNeeded();
}
*/

void Scene::deleteSelection()
{
    deleteSelection( localUserID_ );

    // Send Command to the server.
    CommandPtr deleteSelectionCommand( new SelectionDeletionCommand( localUserID_ ) );
    server_.sendCommand( deleteSelectionCommand );
}


void Scene::deleteSelection( const unsigned int& userId )
{   
    getUserSelection( userId )->clear();

    emit renderNeeded();
}


/***
 * 8. Drawing
 ***/

void Scene::draw( const int& drawGuideRect ) const
{
    GLfloat WHITE_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    UsersMap::const_iterator usersIterator = users_.begin();

    // Draw the non selected drawables.
    nonSelectedDrawables.draw( defaultContourColor );

    // Draw the user's selections.
    for( ; usersIterator != users_.end(); usersIterator++  ){
        (usersIterator->second)->selection.draw( (usersIterator->second)->color );
    }

    // Draw a guide rect if asked.
    if( drawGuideRect != -1 ){
        // Change painting color to white.
        glUniform4fv( uniformColorLocation, 1, WHITE_COLOR );

        // Bind guide rects' VAO and VBO as the active ones.
        glBindVertexArray( linesVAO );
        glBindBuffer( GL_ARRAY_BUFFER, linesVBO );

        // Draw the guide rect.
        glDrawArrays( GL_LINES, linesBufferOffsets[GUIDE_AXIS] + (drawGuideRect << 1), 2 );
    }

    checkOpenGL( "Scene::draw" );
}


void Scene::drawWorldAxis() const
{
    GLfloat worldAxisColors[3][4] =
    {
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f }
    };

    // Bind world axis rects' VAO and VBO as the active ones.
    glBindVertexArray( linesVAO );
    glBindBuffer( GL_ARRAY_BUFFER, linesVBO );

    // Draw each world axis with its corresponding color.
    for( unsigned int i=0; i<3; i++ ){
        glUniform4fv( uniformColorLocation, 1, worldAxisColors[i] );
        glDrawArrays( GL_LINES, linesBufferOffsets[WORLD_AXIS] + (i << 1), 2 );
    }
}


void Scene::drawTransformGuideLine() const
{
    const GLfloat lineColor[4] =
    {
        0.0f, 1.0f, 0.0f, 1.0f
    };

    // Bind selection centroid VAO and VBO as the active ones.
    glBindVertexArray( linesVAO );
    glBindBuffer( GL_ARRAY_BUFFER, linesVBO );

    // Set selection centroid color.
    glUniform4fv( uniformColorLocation, 1, lineColor );

    // Draw selection centroid point.
    // TODO: The range of point sizes are implementation-dependent. Also I have to
    // check wheter point size mode is enabled or not.
    glDisable( GL_DEPTH_TEST );
    glPointSize( 3.0f );
    glDrawArrays( GL_POINTS, linesBufferOffsets[TRANSFORM_GUIDE_LINE], 1 );
    glPointSize( 1.0f );
    glDrawArrays( GL_LINES, linesBufferOffsets[TRANSFORM_GUIDE_LINE], 2 );
    glEnable( GL_DEPTH_TEST );
}


void Scene::emitRenderNeeded()
{
    emit renderNeeded();
}


/***
 * 11. Slots
 ***/

void Scene::executeRemoteUserCommand( UserCommandConstPtr command )
{
    const UserConnectionCommand* userConnected = nullptr;
    const ParameterChangeCommand* changeParameter = nullptr;

    switch( ( dynamic_pointer_cast< const UserCommand >( command ) )->getType() ){
        case UserCommandType::USER_CONNECTION:
            // Cast to an USER_CONNECTION command.
            userConnected = dynamic_cast< const UserConnectionCommand* >( command.get() );

            // Add user to the scene.
            addUser( std::shared_ptr< const UserConnectionCommand>( new UserConnectionCommand( *userConnected ) ) );
        break;

        case UserCommandType::USER_DISCONNECTION:
            // Remove user from the scene.
            removeUser( command->getUserID() );
        break;

        case UserCommandType::PARAMETER_CHANGE:
            // Cast to a PARAMETER_CHANGE command.
            changeParameter = dynamic_cast< const ParameterChangeCommand* >( command.get() );

            // Change parameter.
            switch( changeParameter->getParameterType() ){
                case ParameterType::PIVOT_POINT_MODE:
                    setPivotPointMode( changeParameter->getPivotPointMode(), changeParameter->getUserID() );
                break;
            }
        break;
    }
}


void Scene::executeRemoteDrawableCommand( DrawableCommandConstPtr command )
{
    const CubeCreationCommand* createCube = nullptr;
    const DrawableSelectionCommand* selectDrawable = nullptr;

    switch( command->getType() ){
        case DrawableCommandType::CUBE_CREATION:
            // Cast to a CUBE_CREATION command.
            createCube = dynamic_cast< const CubeCreationCommand* >( command.get() );

            // Add cube to the scene.
            addCube( createCube->getColor(), createCube->getDrawableID() );
        break;

        case DrawableCommandType::DRAWABLE_SELECTION:
            // Cast to a DRAWABLE_SELECTION command.
            selectDrawable = dynamic_cast< const DrawableSelectionCommand* >( command.get() );

            // Select drawable.
            this->selectDrawable( selectDrawable->getDrawableID(), selectDrawable->getUserID() );
        break;
    }
}


void Scene::executeRemoteSelectionCommand( SelectionCommandConstPtr command )
{
    const SelectionResponseCommand* selectionResponse = nullptr;
    const SelectionTransformationCommand* selectionTransformation = nullptr;
    const float* transf = nullptr;
    bool selectionConfirmed;
    unsigned int i;

    DrawableID pendingSelection;

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
                    this->selectDrawable( pendingSelection );
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

void Scene::executeRemoteCommand( CommandConstPtr command )
{
    log_->debug( "Scene - Executing remote command(",
                 commandTargetStrings[static_cast<unsigned int>( command->getTarget() )],
                 ") ...\n" );

    switch( command->getTarget() ){
        case CommandTarget::USER:
            executeRemoteUserCommand( dynamic_pointer_cast< const UserCommand >( command ) );
        break;
        case CommandTarget::DRAWABLE:
            executeRemoteDrawableCommand( dynamic_pointer_cast< const DrawableCommand>( command ) );
        break;
        case CommandTarget::SELECTION:
            executeRemoteSelectionCommand( dynamic_pointer_cast< const SelectionCommand>( command ) );
        break;
    }

    log_->debug( "Scene - Executing remote command(",
                 commandTargetStrings[static_cast<unsigned int>( command->getTarget() )],
                 ") ...OK\n" );
}


/***
 * 12. Auxiliar methods
 ***/

void Scene::roundTransformationMagnitude( float& vx, float& vy, float& vz )
{
    // Round transformation magnitude to 3 decimal places.
    // http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
    vx = floorf( vx * 1000 + 0.5f) / 1000;
    vy = floorf( vy * 1000 + 0.5f) / 1000;
    vz = floorf( vz * 1000 + 0.5f) / 1000;
}


void Scene::roundTransformationMagnitude( float& angle, float& vx, float& vy, float& vz )
{
    // Round transformation magnitude to 3 decimal places.
    // http://stackoverflow.com/questions/1343890/rounding-number-to-2-decimal-places-in-c
    angle = floorf( angle * 1000 + 0.5) / 1000;
    vx = floorf( vx * 1000 + 0.5f) / 1000;
    vy = floorf( vy * 1000 + 0.5f) / 1000;
    vz = floorf( vz * 1000 + 0.5f) / 1000;
}


} // namespace como
