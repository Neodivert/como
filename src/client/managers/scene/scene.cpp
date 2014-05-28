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
    server_( new ServerInterface( log_ ) ),
    uniformColorLocation( -1 ),
    uniformLightingEnabledLocation( -1 )
{
    initOpenGL();

    initLinesBuffer();

    // Set the background color.
    setBackgroundColor( 0.9f, 0.9f, 0.9f, 0.9f );

    // FIXME: Study why this is necessary.
    qRegisterMetaType< CommandConstPtr >();
    qRegisterMetaType< CommandConstPtr >( "CommandConstPtr" );

    // Signal / slot: when a command is received from server, execute it on
    // the local scene.
    QObject::connect( server_.get(), &ServerInterface::commandReceived, this, &Scene::executeRemoteCommand );

    checkOpenGL( "Scene - constructor\n" );
}


Scene::~Scene()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &linesVBO );
    glDeleteVertexArrays( 1, &linesVAO );
}



/***
 * 3. Initialization
 ***/

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
    // Get location of uniform shader variables.
    uniformColorLocation = glGetUniformLocation( currentShaderProgram, "material.color" );
    uniformLightingEnabledLocation = glGetUniformLocation( currentShaderProgram, "lightingEnabled" );

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


bool Scene::connect( const char* host, const char* port, const char* userName )
{
    try{
        std::shared_ptr< const UserAcceptancePacket > userAcceptancePacket;

        // Try to connect to the server. If there is any error, the method
        // ServerInterface::connect() throws an exception.
        log_->debug( "Connecting to (", host, ":", port, ") with name [", userName, "]...\n" );
        userAcceptancePacket = server_->connect( host, port, userName );

        // Retrieve the local user ID.
        localUserID_ = userAcceptancePacket->getId();

        // Copy the scene name given by the server.
        setName( userAcceptancePacket->getSceneName() );

        // Create the scene's primitives directory.
        createScenePrimitivesDirectory();

        // Initialize the materials manager.
        materialsManager_ = MaterialsManagerPtr( new MaterialsManager( localUserID_, server_, log_ ) );

        // Initialize the drawables manager.
        drawablesManager_ = DrawablesManagerPtr( new DrawablesManager( server_, materialsManager_, localUserID_, userAcceptancePacket->getSelectionColor(), std::string( "data/primitives/scenes/" ) + sceneName_, oglContext_, log_ ) );

        // Add the local user to the scene.
        addUser( std::shared_ptr< const UserConnectionCommand >( new UserConnectionCommand( *userAcceptancePacket ) ) );

        // Add the directional light from the previous manager to the scene.
        // TODO: Remove this and sync light creation in both client and
        // server.
        // TODO: Make this constant.
        PackableDrawableID DIRECTIONAL_LIGHT_ID;
        DIRECTIONAL_LIGHT_ID.creatorID = 0;
        DIRECTIONAL_LIGHT_ID.drawableIndex = 1;
        //drawablesManager_->addDrawable( lightingManager_->getDirectionalLight(), DIRECTIONAL_LIGHT_ID );

        // Emit a signal indicating that we have connected to a scene.
        emit connectedToScene( tr( userAcceptancePacket->getSceneName() ) );

        return true;
    }catch( std::exception& ex ){
        std::cerr << ex.what() << std::endl;
        return false;
    }
}


/***
 * 4. Users administration
 ***/


void Scene::addUser( std::shared_ptr< const UserConnectionCommand > userConnectedCommand )
{
    // Create the new user from the given USER_CONNECTION command.
    UserPtr newUser( new User( userConnectedCommand->getUserID(), userConnectedCommand->getName() ) );

    // Insert the new user in the users vector.
    users_.insert( std::pair< UserID, UserPtr >( userConnectedCommand->getUserID(), newUser ) );

    // Create an empty drawables selection for the new user.
    drawablesManager_->addDrawablesSelection( userConnectedCommand->getUserID(), userConnectedCommand->getSelectionColor() );

    // Emit a UserConnectionCommand signal.
    emit userConnected( userConnectedCommand );
}


void Scene::removeUser( UserID userID )
{
    // Unselect all drawables selected by user.
    drawablesManager_->unselectAll( userID );

    // Remove user from scene.
    if( users_.erase( userID ) ){
        emit userDisconnected( userID );
    }
}




/***
 * 5. Getters
 ***/

shared_ptr< QOpenGLContext > Scene::getOpenGLContext() const
{
    return oglContext_;
}


DrawablesManagerPtr Scene::getDrawablesManager() const
{
    return drawablesManager_;
}


MaterialsManagerPtr Scene::getMaterialsManager() const
{
    return materialsManager_;
}


/***
 * 6. Setters
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


void Scene::setName( const char* sceneName )
{
    strncpy( sceneName_, sceneName, NAME_SIZE );
}

void Scene::takeOpenGLContext()
{
    oglContext_->makeCurrent( this );
}


/***
 * 10. Drawing
 ***/

void Scene::drawIfChanged( const glm::mat4& viewProjMatrix, const int& drawGuideRect )
{
    if( hasChangedSinceLastQuery() ){
        draw( viewProjMatrix, drawGuideRect );
    }
}

void Scene::draw( const glm::mat4& viewProjMatrix, const int& drawGuideRect ) const
{
    GLfloat WHITE_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Draw all the drawables.
    enableLighting();
    drawablesManager_->drawAll( viewProjMatrix );
    disableLighting();

    // Draw a guide rect if asked.
    if( drawGuideRect != -1 ){
        Mesh::sendMVPMatrixToShader( viewProjMatrix );

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
 * 13. Slots
 ***/

void Scene::executeRemoteUserCommand( UserCommandConstPtr command )
{
    const UserConnectionCommand* userConnected = nullptr;

    switch( ( dynamic_pointer_cast< const UserCommand >( command ) )->getType() ){
        case UserCommandType::USER_CONNECTION:
            // Cast to an USER_CONNECTION command.
            userConnected = dynamic_cast< const UserConnectionCommand* >( command.get() );

            std::cout << "USER CONNECTED("
                      << (int)( userConnected->getSelectionColor()[0].getValue() ) << ", "
                      << (int)( userConnected->getSelectionColor()[1].getValue() ) << ", "
                      << (int)( userConnected->getSelectionColor()[2].getValue() ) << ", "
                      << (int)( userConnected->getSelectionColor()[3].getValue() ) << ")" << std::endl;

            // Add user to the scene.
            addUser( std::shared_ptr< const UserConnectionCommand>( new UserConnectionCommand( *userConnected ) ) );
        break;

        case UserCommandType::USER_DISCONNECTION:
            // Remove user from the scene.
            removeUser( command->getUserID() );
        break;

        case UserCommandType::PARAMETER_CHANGE:
            // TODO: Change the ParameterChange hierarchy for distinguishing
            // those that affects selections from others.
            drawablesManager_->executeRemoteParameterChangeCommand( dynamic_pointer_cast< const ParameterChangeCommand >( command ) );
        break;
    }
}


void Scene::executeRemoteDrawableCommand( DrawableCommandConstPtr command )
{
    const MeshCreationCommand* meshCreationCommand = nullptr;
    const PrimitiveMeshCreationCommand* primitiveMeshCreationCommand = nullptr;
    const DrawableSelectionCommand* selectDrawable = nullptr;
    const LightCreationCommand* lightCreationCommand = nullptr;
    const DirectionalLightCreationCommand* directionalLightCreationCommand = nullptr;

    switch( command->getType() ){
        case  DrawableCommandType::MESH_CREATION:
            meshCreationCommand = dynamic_cast< const MeshCreationCommand* >( command.get() );

            switch( meshCreationCommand->getMeshType() ){
                case MeshType::PRIMITIVE_MESH:
                    // Cast to a MESH_CREATION command.
                    primitiveMeshCreationCommand = dynamic_cast< const PrimitiveMeshCreationCommand* >( meshCreationCommand );

                    std::cout << "Executing remote mesh creation command: ("
                              << primitiveMeshCreationCommand->getMeshColor()[0].getValue() << ", "
                              << primitiveMeshCreationCommand->getMeshColor()[1].getValue() << ", "
                              << primitiveMeshCreationCommand->getMeshColor()[2].getValue() << ", "
                              << primitiveMeshCreationCommand->getMeshColor()[3].getValue() << ")" << std::endl;

                    // Add mesh to the scene.
                    drawablesManager_->addMesh( primitiveMeshCreationCommand->getDrawableID().creatorID.getValue(),
                                                primitiveMeshCreationCommand->getPrimitiveID(),
                                                //primitiveMeshCreationCommand->getMeshColor(), TODO: Use Material.
                                                primitiveMeshCreationCommand->getDrawableID() );
                break;
                case MeshType::LIGHT:
                    lightCreationCommand = dynamic_cast< const LightCreationCommand* >( meshCreationCommand );

                    switch( lightCreationCommand->getLightType() ){
                        case LightType::DIRECTIONAL_LIGHT:
                            directionalLightCreationCommand = dynamic_cast< const DirectionalLightCreationCommand* >( lightCreationCommand  );

                            // Add a directional light to the scene.
                            drawablesManager_->addDirectionalLight( directionalLightCreationCommand->getDrawableID(),
                                                                    PackableColor( directionalLightCreationCommand->getLightColor() ),
                                                                    PackableColor( directionalLightCreationCommand->getMeshColor() ) );
                        break;
                    }
                break;
                default:
                    // TODO: Complete.
                break;
            }
        break;

        case DrawableCommandType::DRAWABLE_SELECTION:
            // Cast to a DRAWABLE_SELECTION command.
            selectDrawable = dynamic_cast< const DrawableSelectionCommand* >( command.get() );

            // Select drawable.
            drawablesManager_->selectDrawable( selectDrawable->getDrawableID(), selectDrawable->getUserID() );
        break;
    }
}



void Scene::executeRemotePrimitiveCommand( PrimitiveCommandConstPtr command )
{
    const PrimitiveCreationCommand * primitiveCreationCommand = nullptr;
    std::string primitiveRelPath;

    switch( command->getType() ){
        case PrimitiveCommandType::PRIMITIVE_CREATION:
            // Cast to a PRIMITIVE_SELECTION command.
            primitiveCreationCommand = dynamic_cast< const PrimitiveCreationCommand* >( command.get() );

            // Debug message.
            log_->debug( "Primitive file received: [", primitiveCreationCommand->getFile()->getFilePath()->getValue(), "]\n" );

            // Build the primitives relative path, starting from SCENES_PRIMITIVES_DIR/<scene name>/.
            primitiveRelPath = primitiveCreationCommand->getFile()->getFilePath()->getValue();
            primitiveRelPath = primitiveRelPath.substr( ( std::string( SCENES_PRIMITIVES_DIR ) + '/' + sceneName_ ).length() + 1 );

            log_->debug( "Primitive relative path: [", primitiveRelPath, "]\n" );

            // Register the new primitive.
            drawablesManager_->registerPrimitivePath( primitiveCreationCommand->getPrimitiveID(), primitiveRelPath );

            // Emit a signal indicating the primitive insertion. Include
            // primitive's name and ID in the signal.
            emit primitiveAdded( tr( primitiveRelPath.c_str() ),
                                 primitiveCreationCommand->getPrimitiveID() );
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
            drawablesManager_->executeRemoteSelectionCommand( dynamic_pointer_cast< const SelectionCommand>( command ) );
        break;
        case CommandTarget::PRIMITIVE:
            executeRemotePrimitiveCommand( dynamic_pointer_cast< const PrimitiveCommand>( command ) );
        break;
    }

    log_->debug( "Scene - Executing remote command(",
                 commandTargetStrings[static_cast<unsigned int>( command->getTarget() )],
                 ") ...OK\n" );
}


// TODO: Duplicated code in Server::createScenePrimitivesDirectory.
void Scene::createScenePrimitivesDirectory()
{
    char scenePrimitivesDirectory[128] = {0};
    char consoleCommand[256] = {0};
    int lastCommandResult = 0;

    // Build the path to the scene primitives directory.
    sprintf( scenePrimitivesDirectory, "%s/%s", SCENES_PRIMITIVES_DIR, sceneName_ );

    log_->debug( "Creating scene primitives directory [", scenePrimitivesDirectory, "] ...\n" );

    // Copy the server directory for local primitives as this scene's
    // primitives directory.
    // TODO: Use a multiplatform alternative (boost::filesystem::copy_directory
    // doesn't copy directory's contents).
    sprintf( consoleCommand, "mkdir -p \"%s\"", scenePrimitivesDirectory );
    lastCommandResult = system( consoleCommand );

    // If there was any error creating the scene primitives directory, throw
    // an exception.
    if( lastCommandResult ){
        throw std::runtime_error( std::string( "Error creating scene primitives directory [" ) +
                                  scenePrimitivesDirectory +
                                  "]"
                                  );
    }

    log_->debug( "Creating scene primitives directory [", scenePrimitivesDirectory, "] ...OK\n" );
}


bool Scene::hasChangedSinceLastQuery()
{
    // FIXME: This "if" is necessary because this method is invoked since
    // Scene is instantiated, but drawablesManager_ ISN'T initialized
    // until we connect to a server.
    if( drawablesManager_ ){
        return drawablesManager_->hasChangedSinceLastQuery();
    }
    return false;
}



void Scene::enableLighting() const
{
    glUniform1i( uniformLightingEnabledLocation, 1 );
    checkOpenGL( "Scene::enableLighting()" );
}


void Scene::disableLighting() const
{
    glUniform1i( uniformLightingEnabledLocation, 0 );
    checkOpenGL( "Scene::disableLighting()" );
}


} // namespace como
