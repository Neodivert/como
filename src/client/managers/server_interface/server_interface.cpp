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

#include "server_interface.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

ServerInterface::ServerInterface( LogPtr log ) :
    work_( std::shared_ptr< boost::asio::io_service::work >( new boost::asio::io_service::work( io_service_ ) ) ),
    socket_( SocketPtr( new boost::asio::ip::tcp::socket( io_service_ ) ) ),
    timer_( io_service_ ),
    log_( log )
{   
    for( unsigned int i=0; i<2; i++ ){
        workerThreads_.create_thread( boost::bind( &ServerInterface::work, this ) );
    }
}


ServerInterface::~ServerInterface()
{
    disconnect();
}


/***
 * 2. Connection and disconnection
 ***/

std::shared_ptr< const UserAcceptancePacket > ServerInterface::connect( const char* host, const char* port, const char* userName )
{
    boost::system::error_code errorCode;
    como::NewUserPacket newUserPacket;
    como::UserAcceptancePacket userAcceptedPacket;
    PackableColor selectionColor;

    // Create the TCP resolver and query needed for connecting to the server.
    boost::asio::ip::tcp::resolver resolver( io_service_ );
    boost::asio::ip::tcp::resolver::query query( host, port, boost::asio::ip::resolver_query_base::numeric_service );

    // Get the list of TCP endpoints given by resolving the previous query.
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );

    // Connect to the server.
    log_->debug( "Connecting to the server...\n" );
    socket_->connect( *endpoint_iterator, errorCode );

    if( errorCode ){
        disconnect();
        throw std::runtime_error( std::string( "ERROR: Couldn't connect to server (" ) + errorCode.message() + ")" );
    }

    log_->debug( "Connecting to the server...OK\n" );

    // Prepare a NEW_USER network package with the user name, and send it to
    // the server.
    newUserPacket.setName( userName );
    newUserPacket.send( *socket_ );

    // Read from the server an USER_ACCEPTED network package and unpack it.
    userAcceptedPacket.recv( *socket_ );

    selectionColor = userAcceptedPacket.getSelectionColor();
    log_->debug( "User accepted: \n",
                 "\tID: [", userAcceptedPacket.getId(), "]\n",
                 "\tName: [", userAcceptedPacket.getName(), "]\n",
                 "\tSelection color: [", (int)( selectionColor[0].getValue() ), ", ",
                 (int)( selectionColor[1].getValue() ), ", ",
                 (int)( selectionColor[2].getValue() ), ", ",
                 (int)( selectionColor[3].getValue() ), ")\n\n" );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when receiving USER_ACCEPTED package from server (" ) + errorCode.message() + ")" );
    }


    //listenerThread = new std::thread( std::bind( &ServerInterface::listen, this ) );
    listen();

    setTimer();

    return std::shared_ptr< const UserAcceptancePacket >( dynamic_cast< const UserAcceptancePacket* >( userAcceptedPacket.clone() ) );
}


void ServerInterface::disconnect()
{
    boost::system::error_code errorCode;

    log_->debug( "Disconnecting from server ...\n" );

    // Close the socket to the server if it's open.
    if( socket_->is_open() ){
        socket_->shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
        socket_->close();
    }

    // Stop the IO service and join the threads group.
    work_.reset();
    io_service_.stop();
    workerThreads_.join_all();

    log_->debug( "Disconnecting from server ...OK\n" );
}


/***
 * 3. Handlers
 ***/

void ServerInterface::onSceneUpdatePacketReceived( const boost::system::error_code& errorCode, PacketPtr packet )
{
    if( errorCode ){
        log_->error( "Error when receiving packet: ", errorCode.message(), "\n" );
        return;
    }

    const CommandsList* sceneCommands = nullptr;
    CommandsList::const_iterator commandsInterator;

    SceneUpdatePacket* sceneUpdate = dynamic_cast< SceneUpdatePacket *>( packet.get() );
    //const UserConnectionCommand* userConnectedCommand = nullptr;

    if( !sceneUpdate ){
        throw std::runtime_error( std::string( "ERROR in \"onSceneUpdatePacketReceived\": not a SCENE_UPDATE packet" ) );
    }

    log_->debug( "Scene update received with nCommands: ", sceneUpdate->getCommands()->size(), "\n" );

    sceneCommands = sceneUpdate->getCommands();
    for( commandsInterator = sceneCommands->begin(); commandsInterator != sceneCommands->end(); commandsInterator++ ){
        emit commandReceived( *commandsInterator );
    }

    listen();
}


void ServerInterface::onSceneUpdatePacketSended( const boost::system::error_code& errorCode, PacketPtr packet )
{
    if( errorCode ){
        log_->error( "Error when sending packet: ", errorCode.message(), "\n" );
        return;
    }

    log_->debug( "SCENE_UPDATE sent to the server - nCommands ", ( dynamic_cast< const SceneUpdatePacket* >( packet.get() ) )->getCommands()->size(), "\n" );

    setTimer();
}


/***
 * 4. Server communication
 ***/

void ServerInterface::sendCommand( CommandConstPtr sceneCommand )
{
    // Queue the new scene command.
    sceneCommandsToServer_.push( sceneCommand );
}


void ServerInterface::sendPendingCommands()
{
    unsigned int nCommands = 0;

    sceneUpdatePacketToServer_.clear();

    // Move commands from the queue to the SCENE_UPDATE packet.
    while( ( nCommands < MAX_COMMANDS_PER_SCENE_UPDATE ) && !sceneCommandsToServer_.empty() ){
        // TODO: Delete the second argument is not necessary in a SCENE_UPDATE
        // packet sent from client to server.
        sceneUpdatePacketToServer_.addCommand( sceneCommandsToServer_.front(), 0, 0 );
        sceneCommandsToServer_.pop();

        nCommands++;
    }

    // If there in the packet, send it to the server. Otherwise, set a timer
    // to call this method again.
    if( nCommands ){
        log_->debug( "Sending SCENE_UPDATE packet to the server\n" );
        sceneUpdatePacketToServer_.asyncSend( socket_, std::bind( &ServerInterface::onSceneUpdatePacketSended, this, std::placeholders::_1, std::placeholders::_2 ) );
    }else{
        //log_->debug( "No commands to send to the server\n" );
        setTimer();
    }
}


void ServerInterface::setTimer()
{
    // Set a timer for sending pending commands to the server.
    timer_.expires_from_now( boost::posix_time::milliseconds( 100 ) );
    timer_.async_wait( boost::bind( &ServerInterface::sendPendingCommands, this ) );
}


void ServerInterface::listen()
{   
    log_->debug( "Listening for new scene updates from server ...\n" );

    sceneUpdatePacketFromServer_.clear();
    sceneUpdatePacketFromServer_.asyncRecv( socket_, std::bind( &ServerInterface::onSceneUpdatePacketReceived, this, std::placeholders::_1, std::placeholders::_2 ) );
}


void ServerInterface::work()
{
    boost::system::error_code errorCode;
    bool exit = false;

    log_->debug( "[", boost::this_thread::get_id(), "] thread start\n" );

    while( !exit ){
        try{
            io_service_.run( errorCode );
            log_->debug( "[", boost::this_thread::get_id(), "] io_service::run() ...OK\n" );
            if( errorCode ){
                log_->error( "[", boost::this_thread::get_id(), "] Error: ", errorCode.message(), "\n" );
            }
            exit = true;
        }catch( std::exception& ex ){
            // TODO: close the server correctly.
            log_->debug( ex.what(), "\n" );
            throw ex;
        }
    }

    log_->debug( "[", boost::this_thread::get_id(), "] thread end\n" );
}

} // namespace como
