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

#include "scene.hpp"

namespace como {

/***
 * 1. Construction.
 ***/

/*
        // Users sharing this scene.
        UsersMap users_;

        ClientPrimitivesManagerPtr primitivesManager_;

         * \brief Drawables mmanager. This object acts as a container and an
         * interface to all the drawables present in the scene.
         *
        DrawablesManagerPtr drawablesManager_;

        MaterialsManagerPtr materialsManager_;

        LightsManagerPtr lightsManager_;

        OpenGLPtr openGL_;

        // Local user's ID.
        UserID localUserID_;
        DrawableIndex localUserNextDrawableIndex_;

        shared_ptr< QOpenGLContext > oglContext_;

        // Interface with the server.
        ServerInterfacePtr server_;

        // Lines VAO, VBO and offsets.
        GLuint linesVAO;
        GLuint linesVBO;
        GLuint linesBufferOffsets[N_LINES_BUFFER_OFFSETS];

        GLint uniformColorLocation;
        GLint uniformLightingEnabledLocation;
        */

Scene::Scene( const char* host, const char* port, const char* userName, LogPtr log ) :
    BasicScene( log ),
    localUserID_( 1 ), // Will be updated to its final value in Scene::connect().
    localUserNextDrawableIndex_( 1 ),
    uniformColorLocation( -1 ),
    uniformLightingEnabledLocation( -1 )
{
    try {
        UserAcceptancePacket userAcceptancePacket;

        initScene( "scene" );

        server_ = ServerInterfacePtr( new ServerInterface( host, port, userName, getTempDirPath(), log, userAcceptancePacket ) );

        initManagers( userAcceptancePacket );

        initOpenGL();

        initLinesBuffer();

        // Set the background color.
        setBackgroundColor( 0.9f, 0.9f, 0.9f, 0.9f );

        OpenGL::checkStatus( "Scene - constructor\n" );
    }catch( std::exception& ex ){
        throw;
    }
}


/***
 * 2. Destruction
 ***/

Scene::~Scene()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &linesVBO );
    glDeleteVertexArrays( 1, &linesVAO );

    // Primitives manager must be destroyed before Server.
    primitivesManager_.reset();
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

    openGL_ = OpenGLPtr( new OpenGL );
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

    log_->debug( "uniformLightingEnabledLocation: ", uniformLightingEnabledLocation, "\n" );

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


void Scene::initManagers( const UserAcceptancePacket& userAcceptancePacket )
{
    try{
        // Initialize the scene.
        //initScene( "scene" );

        log_->debug( "TEMP DIR: [", getTempDirPath(), "]\n" );

        // Create an interface to the server.
        //server_ = ServerInterfacePtr( new ServerInterface( log_, getTempDirPath() ) );

        // Try to connect to the server. If there is any error, the method
        // ServerInterface::connect() throws an exception.
        //log_->debug( "Connecting to (", host, ":", port, ") with name [", userName, "]...\n" );
        //userAcceptancePacket = server_->connect( host, port, userName );

        // Retrieve the local user ID.
        localUserID_ = userAcceptancePacket.getId();

        // Signal / slot: when a command is received from server, execute it on
        // the local scene.
        QObject::connect( server_.get(), &ServerInterface::commandReceived, this, &Scene::executeRemoteCommand );

        log_->debug( "Remote command execution signal connected\n" );

        // Initialize the materials manager.
        materialsManager_ = MaterialsManagerPtr( new MaterialsManager( localUserID_, server_, log_ ) );

        // Initialize the drawables manager.
        drawablesManager_ = DrawablesManagerPtr( new DrawablesManager( server_, materialsManager_, localUserID_, userAcceptancePacket.getSelectionColor(), std::string( "data/scenes/" ) + getName() + std::string( "/primitives" ), oglContext_, log_ ) );

        // Initialize the primitives manager.
        primitivesManager_ = ClientPrimitivesManagerPtr( new ClientPrimitivesManager( getDirPath(), getTempDirPath(), server_, drawablesManager_, materialsManager_, log_ ) );

        // Initialize the lights manager.
        lightsManager_ = LightsManagerPtr( new LightsManager( drawablesManager_, server_, log_ ) );


        drawablesManager_->addObserver( lightsManager_.get() );

        // Add the local user to the scene.
        addUser( std::shared_ptr< const UserConnectionCommand >( new UserConnectionCommand( userAcceptancePacket ) ) );
    }catch( std::exception& ex ){
        std::cerr << ex.what() << std::endl;
        throw;
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

LightsManagerPtr Scene::getLightsManager() const
{
    return lightsManager_;
}

ClientPrimitivesManagerPtr Scene::getPrimitivesManager() const
{
    return primitivesManager_;
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
    OpenGL::checkStatus( "Scene::setTransformGuideLine" );

    for( ; i<3; i++ ){
        guideRectsBuffer[i] = origin[i];
    }
    for( ; i<6; i++ ){
        guideRectsBuffer[i] = destiny[i-3];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );
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

    openGL_->disableTexturing();

    // Draw all the drawables.
    openGL_->enableLighting();
    drawablesManager_->drawAll( openGL_, viewProjMatrix );
    openGL_->disableLighting();

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

    OpenGL::checkStatus( "Scene::draw" );
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

            log_->debug( "USER CONNECTED(",
                         (int)( userConnected->getSelectionColor()[0].getValue() ), ", ",
                         (int)( userConnected->getSelectionColor()[1].getValue() ), ", ",
                         (int)( userConnected->getSelectionColor()[2].getValue() ), ", ",
                         (int)( userConnected->getSelectionColor()[3].getValue() ), ")\n" );

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
            drawablesManager_->executeRemoteParameterChangeCommand( dynamic_pointer_cast< const UserParameterChangeCommand >( command ) );
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
            drawablesManager_->executeRemoteDrawableCommand( dynamic_pointer_cast< const DrawableCommand>( command ) );
        break;
        case CommandTarget::SELECTION:
            drawablesManager_->executeRemoteSelectionCommand( dynamic_pointer_cast< const SelectionCommand>( command ) );
        break;
        case CommandTarget::PRIMITIVE:
            primitivesManager_->executeRemoteCommand( dynamic_pointer_cast< const PrimitiveCommand>( command ) );
        break;
        case CommandTarget::PRIMITIVE_CATEGORY:
            primitivesManager_->executeRemoteCommand( dynamic_pointer_cast< const PrimitiveCategoryCommand >( command ) );
        break;
        case CommandTarget::MATERIAL:
            materialsManager_->executeRemoteCommand( dynamic_pointer_cast< const MaterialCommand >( command ) );
        break;
        case CommandTarget::LIGHT:
            lightsManager_->executeRemoteCommand( dynamic_pointer_cast< const LightCommand >( command ) );
        break;
    }

    log_->debug( "Scene - Executing remote command(",
                 commandTargetStrings[static_cast<unsigned int>( command->getTarget() )],
                 ") ...OK\n" );
}

bool Scene::hasChangedSinceLastQuery()
{
    // FIXME: This "if( drawablesManager_)" is necessary because this method
    // is invoked since Scene is instantiated, but drawablesManager_ ISN'T
    // initialized until we connect to a server.
    if( drawablesManager_ && drawablesManager_->hasChangedSinceLastQuery() ){
        return true;
    }

    if( materialsManager_ && materialsManager_->hasChangedSinceLastQuery() ){
        return true;
    }

    if( lightsManager_ && lightsManager_->hasChangedSinceLastQuery() ){
        return true;
    }

    return false;
}

} // namespace como
