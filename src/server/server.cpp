/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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


Server::Server( unsigned int port_, unsigned int maxSessions, unsigned int nThreads ) :
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

    // Create the threads pool.
    for( i=0; i<N_THREADS; i++ ){
        threads_.create_thread( boost::bind( &Server::workerThread, this ) );
    }

    // Initialize the commands historic.
    commandsHistoric_ = CommandsHistoricPtr( new CommandsHistoric( std::bind( &Server::broadcast, this ) ) );

    // Initialize the log
    log_ = LogPtr( new Log );
}



/***
 * 3. Main loop
 ***/

void Server::run()
{
    // Create a TCP resolver and a query for getting the endpoint the server
    // must listen to.
    boost::asio::ip::tcp::resolver resolver( *io_service_ );
    boost::asio::ip::tcp::resolver::query query( "localhost", boost::lexical_cast< std::string >( port_ ) );

    try{
        log_->debug( "Press any key to exit\n" );

        // Resolve the query to localhost:port and get its TCP end point.
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
        endPoint_ = *iterator;

        // Open the acceptor.
        openAcceptor();

        // Wait for new connections.
        listen();

        // User only needs to press any key to stop the server.
        std::cin.get();

        // Stop the I/O processing.
        io_service_->stop();

        // TODO: Are these methods called in their corresponding destructors?
        // boost::system::error_code errorCode;
        //newSocket_.shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
        //newSocket_.close( errorCode );

        // Free TCP acceptor.
        //acceptor_.close( errorCode );

        // Wait for the server's threads to finish.
        threads_.join_all();
    }catch( std::exception& ex ){
        log_->error( "Exception: ", ex.what(), "\n" );
    }
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
 * 5. Listeners
 ***/

void Server::listen()
{
    log_->debug( "Listening on port (", port_, ")\n" );

    // Wait for a new user connection.
    acceptor_.async_accept( newSocket_, boost::bind( &Server::onAccept, this, _1 ) );
}


/***
 * 4. Handlers
 ***/

void Server::onAccept( const boost::system::error_code& errorCode )
{
    boost::system::error_code closingErrorCode;

    como::NewUser newUserPacket;
    como::UserAccepted userAcceptedPacket;
    char buffer[128];

    if( errorCode ){
        log_->error( "[", boost::this_thread::get_id(), "]: ERROR(", errorCode.message(), ")\n" );
    }else{
        // Connection established. Wait synchronously for a NEW_USER package.
        newUserPacket.recv( newSocket_ );
        log_->debug( "[", newUserPacket.getName(), "] (", boost::this_thread::get_id(), "): Connected!\n" );

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

        // Assign the new user a fixed selectionColor.
        // TODO: In future versions with multiple writers, change this so
        // every user receives a different color.
        userAcceptedPacket.setSelectionColor( 255, 0, 0, 255 );

        // Pack the network package and send it synchronously to the client.
        userAcceptedPacket.send( newSocket_ );

        // Add the new user to the users map.
        users_[newId_] =
                std::make_shared<PublicUser>(
                        newId_,
                        userAcceptedPacket.getName(),
                        io_service_,
                        std::move( newSocket_ ),
                        std::bind( &Server::processSceneUpdate, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3 ),
                        std::bind( &Server::deleteUser, this, std::placeholders::_1 ),
                        commandsHistoric_,
                        log_
                    );

        // Add an USER_CONNECTED scene command to the server historic.
        addCommand( SceneCommandConstPtr( new UserConnected( userAcceptedPacket ) ) );

        log_->debug( "New sessions: (", newId_, ") - total sessions: ", users_.size(), "\n" );

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


void Server::processSceneUpdate( const boost::system::error_code& errorCode,
                                 UserID userID,
                                 SceneUpdateConstPtr sceneUpdate )
{
    unsigned int i;
    const std::vector< SceneCommandConstPtr >* commands = nullptr;

    mutex_.lock();
    if( errorCode ){
        log_->error( "ERROR reading SCENE_UPDATE packet from [", users_.at(userID)->getName(), "] : ", errorCode.message(), "\n" );
        deleteUser( userID );

        mutex_.unlock();
    }else{
        // FIXME: Make use of the packet.

        // Get the commands from the packet.
        commands = sceneUpdate->getCommands();

        log_->debug( "SCENE_UPDATE received from [", users_.at( userID )->getName(), "] with (", commands->size(), ") commands\n" );

        // Add the commands to the historic.
        for( i=0; i<commands->size(); i++ ){
            processSceneCommand( userID, (*commands)[i] );

        }

        mutex_.unlock();

        //broadcastCallback_();
        users_.at( userID )->readSceneUpdate();
    }
}

void Server::processSceneCommand( UserID userID,
                                  SceneCommandConstPtr sceneCommand )
{
    commandsHistoric_->addCommand( sceneCommand );
}


/***
 * 7. Commands historic management.
 ***/

void Server::addCommand( SceneCommandConstPtr sceneCommand )
{
    // Add the command to the historic.
    commandsHistoric_->addCommand( sceneCommand );

    // A new command has been added to the historic. Broadcast a signal
    // informing about it to all the users.
    //broadcast();

    // Write the full historic in the log.
    log_->lock();
    switch( sceneCommand->getType() ){
        case SceneCommandType::USER_CONNECTED:
            log_->debug( "Command added to historic [USER_CONNECTED]\n" );
        break;
        case SceneCommandType::USER_DISCONNECTED:
            log_->debug( "Command added to historic [USER_DISCONNECTED]\n" );
        break;
        case SceneCommandType::CREATE_CUBE:
            log_->debug( "Command added to historic [CREATE_CUBE]\n" );
        break;
        case SceneCommandType::SELECTION_RESPONSE:
        break;
    }


    /*
    i = 0;
    log_->debug( "Historic after insertion: \n" );
    for( it = commandsHistoric_.begin() ; it != commandsHistoric_.end(); it++ ){
        log_->debug( "Command [", i, "]: " );

        switch( (*it)->getType() ){
            case SceneCommandType::USER_CONNECTED:
                log_->debug( "USER_CONNECTED" );
            break;
            case SceneCommandType::USER_DISCONNECTED:
                log_->debug( "USER_DISCONNECTED" );
            break;
        }
        log_->debug( "\n" );
        i++;
    }
    */

    log_->unlock();
}


/***
 * 8. Auxiliar methods
 ***/

void Server::deleteUser( UserID id )
{
    log_->debug( "Server::deleteUser(", id, ")\n" );

    // Delete the requested user.
    users_.erase( id );

    // Add a SceneCommand to the historic informing about the user
    // disconnection.
    addCommand( SceneCommandConstPtr( new SceneCommand( SceneCommandType::USER_DISCONNECTED, id ) ) );

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
    // Set the acceptor parameters.
    acceptor_.open( endPoint_.protocol() );
    acceptor_.set_option( boost::asio::ip::tcp::acceptor::reuse_address( true ) );

    // Start listening.
    // FIXME: Sometimes I get an exception "bind address already in use" when is
    // the server who closes the connections.
    acceptor_.bind( endPoint_ );
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
