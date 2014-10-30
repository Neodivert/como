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
    port_( port_ ),
    commandsHistoric_( new CommandsHistoric( std::bind( &Server::broadcast, this ) ) ),
    scene_( sceneName, commandsHistoric_, users_, resourceIDsGenerator_, log_, sceneFilePath )
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
 * 3. Main loop
 ***/

void Server::run()
{
    // Create a TCP resolver and a query for getting the endpoint the server
    // must listen to.

    try{
        log_->debug( "Press any key to exit\n" );

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

        // Save scene to file.
        const std::string filePath = scene_.saveToFile();
        if( filePath != "" ){
            log_->debug( "Scene file saved: [",
                         filePath,
                         "]\n" );
        }

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

    // Wait for the server's threads to finish.
    threads_.join_all();

    // Close the server's socket used for accepting new connections.
    if( newSocket_.is_open() ){
        newSocket_.shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
        newSocket_.close( errorCode );
    }

    // Free the server's TCP acceptor.
    acceptor_.close( errorCode );
}


void Server::broadcast()
{
    lock();

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
    lock();

    log_->debug( "Listening on port (", port_, ")\n" );

    // Wait for a new user connection.
    acceptor_.async_accept( newSocket_, boost::bind( &Server::onAccept, this, _1 ) );
}


/***
 * 5. Handlers
 ***/

void Server::onAccept( const boost::system::error_code& errorCode )
{
    lock();

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

        const UserID newUserID = scene_.generateUserID();

        // Assign a id to the new user.
        userAcceptedPacket.setId( newUserID );

        // Check if the new user's name is already in use in the server. If
        // yes, concatenate the string (<id>) to it.
        if( !nameInUse( newUserPacket.getName() ) ){
            userAcceptedPacket.setName( newUserPacket.getName() );
        }else{
            sprintf( buffer, "%s (%d)", newUserPacket.getName(), newUserID );
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
        users_[newUserID] =
                std::make_shared<PublicUser>(
                        newUserID,
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
    lock();

    const CommandsList* commands = nullptr;

    if( errorCode ){
        log_->error( "ERROR reading SCENE_UPDATE packet from [", users_.at(userID)->getName(), "] : ", errorCode.message(), "\n" );
        deleteUser( userID );
    }else{
        // Get the commands from the packet.
        commands = sceneUpdate.getCommands();

        log_->debug( "SCENE_UPDATE received from [", users_.at( userID )->getName(), "] with (", commands->size(), ") commands\n" );

        // Process and add the commands to the historic.
        for( const auto& command : *commands ){
            processSceneCommand( *command );
        }

        //broadcastCallback_();
        users_.at( userID )->readSceneUpdatePacket();
    }
}

void Server::processSceneCommand( const Command& sceneCommand )
{
    lock();

    // This includes inserting the command into the historic.
    scene_.processCommand( sceneCommand );
}


/***
 * 6. Commands historic management.
 ***/

void Server::addCommand( CommandConstPtr sceneCommand )
{
    lock();
    // Add the command to the historic.
    commandsHistoric_->addCommand( std::move( sceneCommand ) );
}


/***
 * 7. Auxiliar methods
 ***/


void Server::deleteUser( UserID id )
{
    lock();
    log_->debug( "Server::deleteUser(", id, ")\n" );

    // Return user's color to free colors container.
    freeUserColors_.push( users_.at( id )->getColor() );

    // Delete the user from the scene (unlocks its current selection).
    scene_.removeUser( id );

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
            throw;
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
