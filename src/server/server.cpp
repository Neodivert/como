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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "server.hpp"

namespace como {


/***
 * 1. Construction
 ***/

Server::Server( unsigned int port_, unsigned int maxSessions, const char* sceneName, unsigned int nThreads ) :
    // Initialize the server parameters.
    io_service_( std::shared_ptr< boost::asio::io_service >( new boost::asio::io_service ) ),
    acceptor_( *io_service_ ),
    work_( *io_service_ ),
    N_THREADS( nThreads ),
    MAX_SESSIONS( maxSessions ),
    newSocket_( *io_service_ ),
    newId_( 1 ),
    port_( port_ )
{
    unsigned int i;

    // Set the scene name.
    strncpy( sceneName_, sceneName, NAME_SIZE );

    // Create the threads pool.
    for( i=0; i<N_THREADS; i++ ){
        threads_.create_thread( boost::bind( &Server::workerThread, this ) );
    }

    // Initialize the commands historic.
    commandsHistoric_ = CommandsHistoricPtr( new CommandsHistoric( std::bind( &Server::broadcast, this ) ) );

    // Initialize the log.
    log_ = LogPtr( new Log );

    // Debug information.
    log_->debug( "Scene [", sceneName_, "] created\n" );
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

            // std::cout << "0x" << std::hex << color << std::dec << std::endl;
            freeUserColors_.push( color );
        }
        // std::cout << "----" << std::endl;
    }
}


/***
 * 3. Main loop
 ***/

void Server::run()
{
    // Create a TCP resolver and a query for getting the endpoint the server
    // must listen to.

    try{
        log_->debug( "Press any key to exit\n" );

        // Create the primitives directory for the current scene.
        createScenePrimitivesDirectory();

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
        userAcceptedPacket.setSceneName( sceneName_ );

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
                        userColor
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
                                 SceneUpdatePacketConstPtr sceneUpdate )
{
    const CommandsList* commands = nullptr;
    CommandsList::const_iterator commandsIterator;


    mutex_.lock();
    if( errorCode ){
        log_->error( "ERROR reading SCENE_UPDATE packet from [", users_.at(userID)->getName(), "] : ", errorCode.message(), "\n" );
        deleteUser( userID );

        mutex_.unlock();
    }else{
        // Get the commands from the packet.
        commands = sceneUpdate->getCommands();

        log_->debug( "SCENE_UPDATE received from [", users_.at( userID )->getName(), "] with (", commands->size(), ") commands\n" );

        // Add the commands to the historic.
        for( commandsIterator = commands->begin(); commandsIterator != commands->end(); commandsIterator++ ){
            processSceneCommand( *commandsIterator );
        }

        mutex_.unlock();

        //broadcastCallback_();
        users_.at( userID )->readSceneUpdatePacket();
    }
}

void Server::processSceneCommand( CommandConstPtr sceneCommand )
{
    const CubeCreationCommand* createCube = nullptr;
    const PrimitiveCreationCommand* primitiveCreationCommand = nullptr;
    const DrawableSelectionCommand* selectDrawable = nullptr;

    switch( sceneCommand->getTarget() ){
        case CommandTarget::USER:
        break;
        case CommandTarget::DRAWABLE:
            switch( ( dynamic_cast< const DrawableCommand* >( sceneCommand.get() ) )->getType() ){
                case DrawableCommandType::CUBE_CREATION:
                    // CUBE_CREATION command received, cast its pointer.
                    createCube = dynamic_cast< const CubeCreationCommand* >( sceneCommand.get() );

                    // Add a node to the Drawable Owners map for the recently added
                    // cube. Mark it with a 0 (no owner).
                    drawableOwners_[createCube->getDrawableID()] = 0;

                    log_->debug( "Cube added! (", (int)( createCube->getDrawableID().creatorID.getValue() ), ", ", (int)( createCube->getDrawableID().drawableIndex.getValue() ), ")\n" );
                break;
                case DrawableCommandType::DRAWABLE_SELECTION:
                    // DRAWABLE_SELECTION command received, cast its pointer.
                    selectDrawable = dynamic_cast< const DrawableSelectionCommand* >( sceneCommand.get() );

                    // Give an affirmative response to the user's selection if the
                    // desired drawable isn't selected by anyone (User ID = 0).
                    users_.at( sceneCommand->getUserID() );
                    log_->debug( "Selecting drawable (", (int)( selectDrawable->getDrawableID().creatorID.getValue() ), ", ", (int)( selectDrawable->getDrawableID().drawableIndex.getValue() ), ")\n" );
                    users_.at( sceneCommand->getUserID() )->addSelectionResponse( drawableOwners_.at( selectDrawable->getDrawableID() ) == 0 );
                break;
            }
        break;
        case CommandTarget::SELECTION:
            if( ( dynamic_cast< const SelectionCommand* >( sceneCommand.get() ) )->getType() == SelectionCommandType::FULL_DESELECTION ){
                // Unselect all.
                unselectAll( sceneCommand->getUserID() );
            }
        break;
        case CommandTarget::PRIMITIVE:
            if( ( dynamic_cast< const PrimitiveCommand* >( sceneCommand.get() ) )->getType() == PrimitiveCommandType::PRIMITIVE_CREATION ){
                // PRIMITIVE_CREATION command received, cast its pointer.
                primitiveCreationCommand = dynamic_cast< const PrimitiveCreationCommand* >( sceneCommand.get() );

                // TODO: Complete, Save new primitive.
                log_->debug( "Primitive received [", primitiveCreationCommand->getFile()->getFilePath()->getValue(), "]\n" );
            }
        break;
    }

    // Add the command to the historic.
    commandsHistoric_->addCommand( sceneCommand );
}


/***
 * 6. Commands historic management.
 ***/

void Server::addCommand( CommandConstPtr sceneCommand )
{
    // Add the command to the historic.
    commandsHistoric_->addCommand( sceneCommand );
}


/***
 * 7. Auxiliar methods
 ***/

void Server::createScenePrimitivesDirectory()
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
    sprintf( consoleCommand, "cp -RT \"%s\" \"%s\"", LOCAL_PRIMITIVES_DIR, scenePrimitivesDirectory );
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

    initializePrimitives( scenePrimitivesDirectory );
}


void Server::initializePrimitives( const char* primitivesDir )
{
    PrimitiveID primitiveID = 0;
    const char* fileName = nullptr;
    const boost::filesystem::recursive_directory_iterator endIterator;
    boost::filesystem::recursive_directory_iterator fileIterator( primitivesDir );

    log_->debug( "Adding primitives to scene [", primitivesDir, "] ...\n" );

    for( ; fileIterator != endIterator; fileIterator++ ){
        if( boost::filesystem::is_regular_file( *fileIterator ) ){
            fileName = fileIterator->path().string().c_str();

            log_->debug( "\tAdding primitive [", fileName, "] to scene ...\n" );
            addCommand( CommandConstPtr( new PrimitiveCreationCommand( fileName, 0, primitiveID ) ) );
            log_->debug( "\tAdding primitive [", fileName, "] to scene ...OK\n" );

            primitiveID++;
        }
    }

    log_->debug( "Adding primitives to scene [", primitivesDir, "] ...\n" );
}


void Server::deleteUser( UserID id )
{
    log_->debug( "Server::deleteUser(", id, ")\n" );

    // Return user's color to free colors container.
    freeUserColors_.push( users_.at( id )->getColor() );

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


void Server::unselectAll( UserID userID )
{
    DrawableOwners::iterator drawableOwner;

    // Iterate over the map of drawable owners and change those entries
    // associated with the current user to zero (no user).
    for( drawableOwner = drawableOwners_.begin(); drawableOwner != drawableOwners_.end(); drawableOwner++ ){
        if( drawableOwner->second == userID ){
            drawableOwner->second = 0;
        }
    }
}


void Server::deleteSelection( UserID userID )
{
    DrawableOwners::iterator drawable;

    // Iterate over the map of drawable owners and remove those entries
    // associated with the current user to zero (no user).
    for( drawable = drawableOwners_.begin(); drawable != drawableOwners_.end(); drawable++ ){
        if( drawable->second == userID ){
            drawableOwners_.erase( drawable );
        }
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
        if( !strcmp( newName, user->second->getName() ) ){
            return true;
        }
    }

    return false;
}

} // namespace como
