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
    BasicScene( log )
{
    try {
        UserAcceptancePacket userAcceptancePacket;

        initScene( "scene" );

        server_ = ServerInterfacePtr( new ServerInterface( host, port, userName, getTempDirPath(), log, userAcceptancePacket ) );

        // Signal / slot: when a command is received from server, execute it on
        // the local scene.
        qRegisterMetaType< std::shared_ptr< const Command > >( "std::shared_ptr< const Command >" );
        QObject::connect( server_.get(), &ServerInterface::commandReceived, this, &Scene::executeRemoteCommand );
        log_->debug( "Remote command execution signal connected\n" );

        initOpenGL();


        linesRenderer_ =
                std::unique_ptr<AuxiliarLinesRenderer>(
                    new AuxiliarLinesRenderer( *openGL_,
                                               userAcceptancePacket.getSelectionColor() ) );

        initManagers( userAcceptancePacket );

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
    // Primitives manager must be destroyed before Server.
    primitivesManager_.reset();
}


/***
 * 3. Getters
 ***/

std::shared_ptr< QOpenGLContext > Scene::getOpenGLContext() const
{
    LOCK
    return oglContext_;
}


UsersManagerPtr Scene::getUsersManager() const
{
    LOCK
    return usersManager_;
}


MeshesManagerPtr Scene::getMeshesManager() const
{
    LOCK
    return entitiesManager_->getMeshesManager();
}


MaterialsManagerPtr Scene::getMaterialsManager() const
{
    LOCK
    return materialsManager_;
}


LightsManagerPtr Scene::getLightsManager() const
{
    LOCK
    return entitiesManager_->getLightsManager();
}


ClientPrimitivesManagerPtr Scene::getPrimitivesManager() const
{
    LOCK
    return primitivesManager_;
}


EntitiesManagerPtr Scene::getEntitiesManager() const
{
    LOCK
    return entitiesManager_;
}


SystemPrimitivesFactoryPtr Scene::getSystemPrimitivesFactory() const
{
    LOCK
    return systemPrimitivesFactory_  ;
}


TextureWallsManager *Scene::getTextureWallsManager() const
{
    LOCK
    return textureWallsManager_.get();
}


TexturesManager *Scene::getTexturesManager() const
{
    LOCK
    return texturesManager_.get();
}


OpenGLPtr Scene::getOpenGL() const
{
    LOCK
    return openGL_;
}


AuxiliarLinesRenderer *Scene::linesRenderer() const
{
    LOCK
    return linesRenderer_.get();
}


/***
 * 4. Setters
 ***/

void Scene::setBackgroundColor( const GLfloat& r, const GLfloat& g, const GLfloat &b, const GLfloat &a ) const
{
    glClearColor( r, g, b, a );
}


/***
 * 5. Drawing
 ***/

void Scene::draw( const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix ) const
{
    LOCK

    // Set the background color.
    setBackgroundColor( 0.9f, 0.9f, 0.9f, 1.0f );

    // Draw all the entities.
    entitiesManager_->drawAll( openGL_, viewMatrix, projectionMatrix );
}


/***
 * 6. Main loop
 ***/

void Scene::run()
{
    server_->run();
}


/***
 * 7. Updating (Observer pattern)
 ***/

void Scene::update()
{
    LOCK
    notifyObservers();
}


/***
 * 9. Slots
 ***/

void Scene::executeRemoteCommand( std::shared_ptr< const Command > command )
{
    LOCK

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
        }break;
        case CommandTarget::RESOURCES_SELECTION:{
            entitiesManager_->executeResourcesSelectionCommand( dynamic_cast< const ResourcesSelectionCommand& >( *command ) );
        }break;
        case CommandTarget::GEOMETRIC_PRIMITIVE:{
            systemPrimitivesFactory_->executeRemoteCommand( dynamic_cast< const SystemPrimitiveCommand& >( *command ) );
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
        case CommandTarget::ENTITY:
            entitiesManager_->executeRemoteEntityCommand( dynamic_cast< const EntityCommand& >( *command ) );
        break;
    }

    log_->debug( "Scene - Executing remote command(",
                 commandTargetStrings[static_cast<unsigned int>( command->getTarget() )],
                 ") ...OK\n" );
}


/***
 * 10. Initialization
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
    oglContext_ = std::shared_ptr< QOpenGLContext >( new QOpenGLContext );
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
        systemPrimitivesFactory_ = SystemPrimitivesFactoryPtr( new SystemPrimitivesFactory( server_, entitiesManager_->getMeshesManager(), materialsManager_, textureWallsManager_.get() ) );

        // Initialize the primitives manager.
        primitivesManager_ = ClientPrimitivesManagerPtr( new ClientPrimitivesManager( getDirPath(), getTempDirPath(), server_, entitiesManager_->getMeshesManager(), log_ ) );

    }catch( std::exception& ex ){
        log_->error( ex.what(), "\n" );
        throw;
    }
}

} // namespace como
