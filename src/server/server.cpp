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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "server.hpp"
#include <memory>

namespace como {

/***
 * 1. Construction
 ***/

Server::Server( unsigned int port_, unsigned int maxSessions, const char* sceneName, const char* sceneFilePath, unsigned int nThreads ) :
    // Initialize the server parameters.
    resourceIDsGenerator_( new ResourceIDsGenerator( NO_USER ) ),
    log_( new Log ),
    io_service_( std::shared_ptr< boost::asio::io_service >( new boost::asio::io_service ) ),
    acceptor_( *io_service_ ),
    work_( *io_service_ ),
    N_THREADS( nThreads ),
    MAX_SESSIONS( maxSessions ),
    newSocket_( *io_service_ ),
    newId_( 1 ),
    port_( port_ ),
    resourcesOwnershipManager_( users_, log_ ),
    lightsManager_( 4, &resourcesOwnershipManager_ ),
    commandsHistoric_( new CommandsHistoric( std::bind( &Server::broadcast, this ) ) ),
    scene_( sceneName, commandsHistoric_, log_, sceneFilePath )
{
    unsigned int i;

    // Create the threads pool.
    for( i=0; i<N_THREADS; i++ ){
        threads_.create_thread( boost::bind( &Server::workerThread, this ) );
    }
}


// TODO: Generate more colors?
void Server::initUserColors()
{
    std::uint32_t mask = 0xFF;
    std::uint32_t color = 0;

    // Clear the container of free user colors.
    std::queue< std::uint32_t > empty;
    std::swap( freeUserColors_, empty );

    unsigned int partition;
    unsigned int partitionElement;

    //const unsigned int MAX_USERS = 32;
    const unsigned int MAX_USER_COLORS = 24;
    const unsigned int PARTITION_SIZE = 8;
    const unsigned int N_PARTITIONS = MAX_USER_COLORS / PARTITION_SIZE;

    for( partition = 0; partition < N_PARTITIONS; partition++ ){
        mask = 0xFF - partition * 0x55;
        for( partitionElement = 1; partitionElement < PARTITION_SIZE; partitionElement++ ){
            color = 0;
            if( partitionElement & 0x1 ){
                color |= mask;
            }
            if( partitionElement & 0x2 ){
                color |= (mask << 8);
            }
            if( partitionElement & 0x4 ){
                color |= (mask << 16);
            }

            freeUserColors_.push( color );
        }
    }
}


/***
 * 2. Destruction
 ***/

Server::~Server()
{
    // Primitives manager must be destroyed before Server.
    primitivesManager_.reset();
}


/***
 * 3. Main loop
 ***/

void Server::run()
{
    const ResourceID DIRECTIONAL_LIGHT_ID = resourceIDsGenerator_->generateResourceIDs( 1 );
    const ResourceID CAMERA_ID = resourceIDsGenerator_->generateResourceIDs( 1 );

    // Create a TCP resolver and a query for getting the endpoint the server
    // must listen to.

    try{
        log_->debug( "Press any key to exit\n" );

        // Create and initialize the primitives directory for the current
        // scene.
        io_service_->post( [this](){
            primitivesManager_ = std::unique_ptr< ServerPrimitivesManager >( new ServerPrimitivesManager( scene_.getDirPath(),
                                                                                                          scene_.getTempDirPath(),
                                                                                                          commandsHistoric_,
                                                                                                          log_,
                                                                                                          resourceIDsGenerator_ ) );
        });

        // Create a directional light with with no owner and synchronize it in
        // the commands historic.
        std::uint8_t lightColor[4] = { 255, 255, 255, 255 };
        lightsManager_.requestDirectionalLightCreation( DIRECTIONAL_LIGHT_ID );
        addCommand( CommandConstPtr( new DirectionalLightCreationCommand( NO_USER, DIRECTIONAL_LIGHT_ID, lightColor ) ) );

        // Create a camera with no owner and syncrhonize it in the commands
        // historic.
        const glm::vec3 cameraCenter( 0.0f, 0.0f, 0.0f );
        const glm::vec3 cameraEye( 0.5f, 0.5f, 0.0f );
        const glm::vec3 cameraUp( -0.5f, 0.5f, 0.0f );
        processSceneCommand(
                    CameraCreationCommand(
                        CAMERA_ID,
                        cameraCenter,
                        cameraEye,
                        cameraUp ) );

        // Initialize the container of free user colors.
        initUserColors();

        // Open the acceptor.
        openAcceptor();

        // Wait for new connections.
        listen();

        // User only needs to press any key to stop the server.
        std::cin.get();

        // User pressed a key; disconnect server.
        disconnect();

    }catch( std::exception& ex ){
        log_->error( "Exception: ", ex.what(), "\n" );
    }
}

// TODO: Call this method when exceptions are thrown.
// TODO: Make use of errorCode?
void Server::disconnect()
{
    boost::system::error_code errorCode;

    // Stop the I/O processing.
    io_service_->stop();

    // Close the server's socket used for accepting new connections.
    if( newSocket_.is_open() ){
        newSocket_.shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
        newSocket_.close( errorCode );
    }

    // Free the server's TCP acceptor.
    acceptor_.close( errorCode );

    // Wait for the server's threads to finish.
    threads_.join_all();
}


void Server::broadcast()
{
    log_->debug( "Server - broadcasting\n" );
    UsersMap::iterator user;

    for( user = users_.begin(); user != users_.end(); user++ ){
        user->second->requestUpdate();
    }
}



/***
 * 4. Listeners
 ***/

void Server::listen()
{
    log_->debug( "Listening on port (", port_, ")\n" );

    // Wait for a new user connection.
    acceptor_.async_accept( newSocket_, boost::bind( &Server::onAccept, this, _1 ) );
}


/***
 * 5. Handlers
 ***/

void Server::onAccept( const boost::system::error_code& errorCode )
{
    boost::system::error_code closingErrorCode;

    como::NewUserPacket newUserPacket;
    como::UserAcceptancePacket userAcceptedPacket;
    char buffer[128];

    std::uint32_t userColor = 0;

    if( errorCode ){
        log_->error( "[", boost::this_thread::get_id(), "]: ERROR(", errorCode.message(), ")\n" );
    }else{
        // Connection established. Wait synchronously for a NEW_USER package.
        log_->debug( "New user (", boost::this_thread::get_id(), "): Connecting!\n" );
        newUserPacket.recv( newSocket_ );
        log_->debug( "User [", newUserPacket.getName(), "] (", boost::this_thread::get_id(), "): Connected!\n" );

        /*** Prepare an USER_ACCEPTED package in respond to the previous NEW_USER one ***/

        // Assign a id to the new user.
        userAcceptedPacket.setId( newId_ );

        // Check if the new user's name is already in use in the server. If
        // yes, concatenate the string (<id>) to it.
        if( !nameInUse( newUserPacket.getName() ) ){
            userAcceptedPacket.setName( newUserPacket.getName() );
        }else{
            sprintf( buffer, "%s (%d)", newUserPacket.getName(), newId_ );
            userAcceptedPacket.setName( buffer );
        }

        // Send the scene name within the UserAcceptancePacket.
        userAcceptedPacket.setSceneName( scene_.getName().c_str() );

        // Get a color from the queue of free colors and assign it to the new
        // user.
        userColor = freeUserColors_.front();
        freeUserColors_.pop();

        userAcceptedPacket.setSelectionColor( ( userColor & 0xFF0000 ) >> 16,
                                              ( userColor & 0xFF00 ) >> 8,
                                              userColor & 0xFF,
                                              0xFF );

        // Pack the network package and send it synchronously to the client.
        userAcceptedPacket.send( newSocket_ );

        // Add the new user to the users map.
        users_[newId_] =
                std::make_shared<PublicUser>(
                        newId_,
                        userAcceptedPacket.getName(),
                        io_service_,
                        std::move( newSocket_ ),
                        std::bind( &Server::processSceneUpdatePacket, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ),
                        std::bind( &Server::deleteUser, this, std::placeholders::_1 ),
                        commandsHistoric_,
                        log_,
                        userColor,
                        scene_.getTempDirPath()
                    );

        // Add an USER_CONNECTION scene command to the server historic.
        addCommand( CommandConstPtr( new UserConnectionCommand( userAcceptedPacket ) ) );

        // Increment the "new id" for the next user.
        newId_++;

        if( users_.size() < MAX_SESSIONS ){
            // There is room for more users, wait for a new connection.
            listen();
        }else{
            // There isn't room for more users, close the acceptor.
            acceptor_.close( closingErrorCode );
            //acceptor_.cancel();

            log_->debug( "Server is full (MAX_SESSIONS: ", MAX_SESSIONS, ")\n" );
        }
    }
}


void Server::processSceneUpdatePacket( const boost::system::error_code& errorCode,
                                 UserID userID,
                                 const SceneUpdatePacket& sceneUpdate )
{
    const CommandsList* commands = nullptr;

    mutex_.lock();
    if( errorCode ){
        log_->error( "ERROR reading SCENE_UPDATE packet from [", users_.at(userID)->getName(), "] : ", errorCode.message(), "\n" );
        deleteUser( userID );

        mutex_.unlock();
    }else{
        // Get the commands from the packet.
        commands = sceneUpdate.getCommands();

        log_->debug( "SCENE_UPDATE received from [", users_.at( userID )->getName(), "] with (", commands->size(), ") commands\n" );

        // Process and add the commands to the historic.
        for( const auto& command : *commands ){
            processSceneCommand( *command );
        }

        mutex_.unlock();

        //broadcastCallback_();
        users_.at( userID )->readSceneUpdatePacket();
    }
}

void Server::processSceneCommand( const Command& sceneCommand )
{
    // TODO: Move this code to Scene class.
    switch( sceneCommand.getTarget() ){
        case CommandTarget::PRIMITIVE:{
            const PrimitiveCommand& primitiveCommand = dynamic_cast< const PrimitiveCommand& >( sceneCommand );

            switch( primitiveCommand.getType() ){
                case PrimitiveCommandType::PRIMITIVE_CREATION:{
                    // PRIMITIVE_CREATION command received, cast its pointer.
                    const PrimitiveCreationCommand& primitiveCreationCommand =
                            dynamic_cast< const PrimitiveCreationCommand& >( sceneCommand );

                    // TODO: Complete, Save new primitive (Move it from temp to category directory).
                    log_->debug( "Primitive received [", primitiveCreationCommand.getPrimitiveInfo().name, "]\n" );
                }break;
                case PrimitiveCommandType::PRIMITIVE_INSTANTIATION:{
                    const PrimitiveInstantiationCommand& primitiveCommand =
                            dynamic_cast< const PrimitiveInstantiationCommand& >( sceneCommand );

                    // Add a node to the Drawable Owners map for the recently added
                    // drawable. Mark it with a 0 (no owner).
                    resourcesOwnershipManager_.registerResource( primitiveCommand.getMeshID(), primitiveCommand.getUserID() );

                    log_->debug( "Mesh added! (", (int)( primitiveCommand.getMeshID().getCreatorID() ),
                                 ", ", (int)( primitiveCommand.getMeshID().getResourceIndex() ), "\n" );
                }break;
            }
        }break;
        case CommandTarget::LIGHT:{
            const LightCommand& lightCommand = dynamic_cast< const LightCommand& >( sceneCommand );

            if( lightCommand.getType() == LightCommandType::LIGHT_CREATION ){
                // Request the creation of the light to the lights manager.
                if( lightsManager_.requestDirectionalLightCreation( lightCommand.getResourceID() ) ){
                    users_.at( lightCommand.getUserID() )->addResponseCommand(
                                CommandConstPtr(
                                    new LightCreationResponseCommand( lightCommand.getResourceID(),
                                                                      true
                                                                      ) ) );
                }else{
                    users_.at( lightCommand.getUserID() )->addResponseCommand(
                                CommandConstPtr(
                                    new LightCreationResponseCommand( lightCommand.getResourceID(),
                                                                      false
                                                                      ) ) );
                    // If the request was denied, return from this method so
                    // the creation command received from user isn't added to
                    // the commands historic.
                    return;
                }
            }
        }break;
        case CommandTarget::RESOURCE:{
            const ResourceCommand& resourceCommand = dynamic_cast< const ResourceCommand& >( sceneCommand );
            resourcesOwnershipManager_.executeResourceCommand( resourceCommand );
        }break;
        case CommandTarget::RESOURCES_SELECTION:{
            const ResourcesSelectionCommand& resourcesSelectionCommand =
                    dynamic_cast< const ResourcesSelectionCommand& >( sceneCommand );
            resourcesOwnershipManager_.executeResourcesSelectionCommand( resourcesSelectionCommand );
        }break;
        case CommandTarget::GEOMETRIC_PRIMITIVE:{
            const GeometricPrimitiveCommand& geometricPrimitiveCommand =
                    dynamic_cast< const GeometricPrimitiveCommand& >( sceneCommand );

            if( ( geometricPrimitiveCommand.getType() == GeometricPrimitiveCommandType::CUBE_CREATION ) ||
                ( geometricPrimitiveCommand.getType() == GeometricPrimitiveCommandType::CONE_CREATION ) ||
                ( geometricPrimitiveCommand.getType() == GeometricPrimitiveCommandType::CYLINDER_CREATION ) ||
                ( geometricPrimitiveCommand.getType() == GeometricPrimitiveCommandType::SPHERE_CREATION ) ){
                // Add a node to the Drawable Owners map for the recently added
                // drawable. Mark it with a 0 (no owner).
                resourcesOwnershipManager_.registerResource( geometricPrimitiveCommand.getMeshID(),
                                                             geometricPrimitiveCommand.getUserID() );

                log_->debug( "Geometric primitive added! (", geometricPrimitiveCommand.getMeshID(), "\n" );
            }
        }break;
        case CommandTarget::CAMERA:{
            const CameraCommand& cameraCommand =
                    dynamic_cast< const CameraCommand& >( sceneCommand );

            if( cameraCommand.getType() == CameraCommandType::CAMERA_CREATION ){
                resourcesOwnershipManager_.registerResource( cameraCommand.cameraID(),
                                                             cameraCommand.getUserID(),
                                                             false );
            }
        }break;
        default:
        break;
    }

    // This includes inserting the command into the historic.
    scene_.processCommand( sceneCommand );
}


/***
 * 6. Commands historic management.
 ***/

void Server::addCommand( CommandConstPtr sceneCommand )
{
    // Add the command to the historic.
    commandsHistoric_->addCommand( std::move( sceneCommand ) );
}


/***
 * 7. Auxiliar methods
 ***/


void Server::deleteUser( UserID id )
{
    log_->debug( "Server::deleteUser(", id, ")\n" );

    // Return user's color to free colors container.
    freeUserColors_.push( users_.at( id )->getColor() );

    // Unlock its current selection.
    resourcesOwnershipManager_.removeUser( id );

    // Delete the requested user.
    users_.erase( id );

    // Add a SceneCommand to the historic informing about the user
    // disconnection.
    addCommand( CommandConstPtr( new UserDisconnectionCommand( id ) ) );

    if( users_.size() == (MAX_SESSIONS - 1) ){
        // If the server was full before this user got out, that means the acceptor wasn't
        // listening for new connections. Start listening now that there is room again.

        //acceptor_.open( endPoint_.protocol() );
        //acceptor_.listen( 0 );
        openAcceptor();

        // Start listening.
        // FIXME: Sometimes I get an exception "bind address already in use" when is
        // the server who closes the connections.
        listen();
    }
}


std::string Server::getCurrentDayTime() const
{
    std::time_t now = std::time( 0 );
    return std::ctime(&now);
}


void Server::workerThread()
{
    log_->debug( "[", boost::this_thread::get_id(), "] Thread Start\n" );

    while( true )
    {
        try
        {
            boost::system::error_code ec;
            io_service_->run( ec );
            if( ec )
            {
                log_->error( "[", boost::this_thread::get_id(), "] Error: ", ec.message(), "\n" );
            }
            break;
        }
        catch( std::exception & ex )
        {
            log_->error( "[", boost::this_thread::get_id(), "] Exception: ", ex.what(), "\n" );
        }
    }

    log_->debug( "[", boost::this_thread::get_id(), "] Thread finish\n" );
}


void Server::openAcceptor()
{
    // Set an endpoint for given server TCP port.
    boost::asio::ip::tcp::endpoint endpoint( boost::asio::ip::tcp::v4(), port_ );

    // Open the acceptor.
    acceptor_.open( endpoint.protocol() );

    // Set "reuse_address" to true in order to avoid "Already in use" error.
    acceptor_.set_option( boost::asio::ip::tcp::acceptor::reuse_address( true ) );

    // Bind the acceptor to the previous endpoint.
    acceptor_.bind( endpoint );

    // Start listening.
    acceptor_.listen( 0 );
}


bool Server::nameInUse( const char* newName ) const
{
    UsersMap::const_iterator user;

    for( user = users_.begin(); user != users_.end(); user++ ){
        if( !strcmp( newName, user->second->getName().c_str() ) ){
            return true;
        }
    }

    return false;
}

} // namespace como
