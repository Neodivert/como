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

Scene::Scene( const char* host, const char* port, const char* userName, LogPtr log ) :
    BasicScene( log ),
    uniformColorLocation( -1 )
{
    try {
        UserAcceptancePacket userAcceptancePacket;

        initScene( "scene" );

        server_ = ServerInterfacePtr( new ServerInterface( host, port, userName, getTempDirPath(), log, userAcceptancePacket ) );

        // Signal / slot: when a command is received from server, execute it on
        // the local scene.
        // TODO: this randomly causes a command to be executed when Scene isn't
        // initialized yet.
        qRegisterMetaType< std::shared_ptr< const Command > >( "std::shared_ptr< const Command >" );
        QObject::connect( server_.get(), &ServerInterface::commandReceived, this, &Scene::executeRemoteCommand );
        log_->debug( "Remote command execution signal connected\n" );

        initOpenGL();

        initManagers( userAcceptancePacket );

        initLinesBuffer();

        // Set the background color.
        setBackgroundColor( 0.9f, 0.9f, 0.9f, 0.9f );

        OpenGL::checkStatus( "Scene - constructor\n" );

    }catch( std::exception& ){
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

    openGL_ = OpenGLPtr( new OpenGL );

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

    const GLfloat x0 = 0.0f;
    const GLfloat y0 = 0.0f;
    const GLfloat z0 = 0.0f;

    const GLfloat x1 = 0.5f;
    const GLfloat y1 = 0.5f;
    const GLfloat z1 = 0.5f;

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
        log_->debug( "TEMP DIR: [", getTempDirPath(), "]\n" );

        // Initialize the users manager with the local user.
        usersManager_ = UsersManagerPtr( new UsersManager( userAcceptancePacket ) );

        // Initialize the materials manager.
        materialsManager_ = MaterialsManagerPtr( new MaterialsManager( server_, log_ ) );
        materialsManager_->Observable::addObserver( this );

        // Initialize the textures manager.
        texturesManager_ = TexturesManagerPtr( new TexturesManager( *openGL_, server_, getDirPath(), getTempDirPath() ) );

        // Initialize the texture walls manager.
        textureWallsManager_ = TextureWallsManagerPtr( new TextureWallsManager( server_, *texturesManager_ ) );

        // Initialize the entities manager.
        entitiesManager_ = EntitiesManagerPtr( new EntitiesManager( server_, log_, openGL_.get(), usersManager_, materialsManager_, textureWallsManager_.get() ) );

        // Initialize the geometric primitives factory.
        geometricPrimitivesFactory_ = GeometricPrimitivesFactoryPtr( new GeometricPrimitivesFactory( server_, entitiesManager_->getMeshesManager(), materialsManager_, textureWallsManager_.get() ) );

        // Initialize the primitives manager.
        primitivesManager_ = ClientPrimitivesManagerPtr( new ClientPrimitivesManager( getDirPath(), getTempDirPath(), server_, entitiesManager_->getMeshesManager(), materialsManager_, log_ ) );

        localUserConnectionCommand_ = UserConnectionCommandConstPtr( new UserConnectionCommand( userAcceptancePacket ) );
    }catch( std::exception& ex ){
        std::cerr << ex.what() << std::endl;
        throw;
    }
}


/***
 * 5. Getters
 ***/

shared_ptr< QOpenGLContext > Scene::getOpenGLContext() const
{
    return oglContext_;
}


UsersManagerPtr Scene::getUsersManager() const
{
    return usersManager_;
}


MeshesManagerPtr Scene::getMeshesManager() const
{
    return entitiesManager_->getMeshesManager();
}


MaterialsManagerPtr Scene::getMaterialsManager() const
{
    return materialsManager_;
}


LightsManagerPtr Scene::getLightsManager() const
{
    return entitiesManager_->getLightsManager();
}


ClientPrimitivesManagerPtr Scene::getPrimitivesManager() const
{
    return primitivesManager_;
}


EntitiesManagerPtr Scene::getEntitiesManager() const
{
    return entitiesManager_;
}


GeometricPrimitivesFactoryPtr Scene::getGeometricPrimitivesFactory() const
{
    return geometricPrimitivesFactory_  ;
}


TextureWallsManager *Scene::getTextureWallsManager() const
{
    return textureWallsManager_.get();
}


TexturesManager *Scene::getTexturesManager() const
{
    return texturesManager_.get();
}


OpenGLPtr Scene::getOpenGL() const
{
    return openGL_;
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

void Scene::draw( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const int& drawGuideRect ) const
{
    GLfloat WHITE_COLOR[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Draw all the entities.
    entitiesManager_->drawAll( openGL_, viewMatrix, projectionMatrix );

    // Draw a guide rect if asked.
    if( drawGuideRect != -1 ){
        openGL_->setShadingMode( ShadingMode::SOLID_PLAIN );
        openGL_->setMVPMatrix( glm::mat4( 1.0f ), viewMatrix, projectionMatrix );

        // Change painting color to white.
        glUniform4fv( uniformColorLocation, 1, WHITE_COLOR );

        // Bind guide rects' VAO and VBO as the active ones.
        glBindVertexArray( linesVAO );
        glBindBuffer( GL_ARRAY_BUFFER, linesVBO );

        // Draw the guide rect.
        glDrawArrays( GL_LINES, linesBufferOffsets[GUIDE_AXIS] + (drawGuideRect << 1), 2 );
    }
}


void Scene::drawWorldAxis() const
{
    openGL_->setShadingMode( ShadingMode::SOLID_PLAIN );

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
    openGL_->setShadingMode( ShadingMode::SOLID_PLAIN );

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

void Scene::executeRemoteCommand( std::shared_ptr< const Command > command )
{
    log_->debug( "Scene - Executing remote command(",
                 commandTargetStrings[static_cast<unsigned int>( command->getTarget() )],
                 ") ...\n" );

    switch( command->getTarget() ){
        case CommandTarget::USER:
            usersManager_->executeRemoteCommand( dynamic_cast< const UserCommand& >( *command ) );
        break;
        case CommandTarget::SELECTION:
            entitiesManager_->executeRemoteSelectionCommand( dynamic_cast< const SelectionCommand& >( *command ) );
        break;
        case CommandTarget::PRIMITIVE:
            primitivesManager_->executeRemoteCommand( dynamic_cast< const PrimitiveCommand& >( *command ) );
        break;
        case CommandTarget::PRIMITIVE_CATEGORY:
            primitivesManager_->executeRemoteCommand( dynamic_cast< const PrimitiveCategoryCommand& >( *command ) );
        break;
        case CommandTarget::MATERIAL:
            materialsManager_->executeRemoteCommand( dynamic_cast< const MaterialCommand& >( *command ) );
        break;
        case CommandTarget::LIGHT:
            entitiesManager_->getLightsManager()->executeRemoteCommand( dynamic_cast< const LightCommand& >( *command ) );
        break;
        case CommandTarget::RESOURCE:{
            entitiesManager_->executeResourceCommand( dynamic_cast< const ResourceCommand& >( *command ) );
            // TODO: materialsManager_->executeResourceCommand( resourceCommand );
        }break;
        case CommandTarget::RESOURCES_SELECTION:{
            entitiesManager_->executeResourcesSelectionCommand( dynamic_cast< const ResourcesSelectionCommand& >( *command ) );
            // TODO: materialsManager_->executeResourcesSelectionCommand( selectionCommand );
        }break;
        case CommandTarget::GEOMETRIC_PRIMITIVE:{
            geometricPrimitivesFactory_->executeRemoteCommand( dynamic_cast< const GeometricPrimitiveCommand& >( *command ) );
        }break;
        case CommandTarget::TEXTURE:
            texturesManager_->executeRemoteCommand( dynamic_cast< const TextureCommand& >( *command ) );
        break;
        case CommandTarget::TEXTURE_WALL:
            textureWallsManager_->executeRemoteCommand( dynamic_cast< const TextureWallCommand& >( *command ) );
        break;
        case CommandTarget::CAMERA:
            entitiesManager_->getCamerasManager()->executeRemoteCommand( dynamic_cast< const CameraCommand& >( *command ) );
        break;
    }

    log_->debug( "Scene - Executing remote command(",
                 commandTargetStrings[static_cast<unsigned int>( command->getTarget() )],
                 ") ...OK\n" );
}


void Scene::run()
{
    // When the server was created, the local user was added to the Scene
    // but as the GUI wasn't yet initialized, the GUI users list wasn't
    // updated. We reemit here the signal for GUI updating.
    // TODO: Remove this and find a better way of initializing the users list?
    emit userConnected( localUserConnectionCommand_ );

    server_->run();
}


/***
 * 15. Updating (Observer pattern)
 ***/

void Scene::update()
{
    notifyObservers();
}

} // namespace como
