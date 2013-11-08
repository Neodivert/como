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

#include "server_interface.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

ServerInterface::ServerInterface() :
    work_( std::shared_ptr< boost::asio::io_service::work >( new boost::asio::io_service::work( io_service_ ) ) ),
    socket_( SocketPtr( new boost::asio::ip::tcp::socket( io_service_ ) ) )
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

void ServerInterface::connect( const char* host, const char* port, const char* userName )
{
    boost::system::error_code errorCode;
    como::NewUser newUserPacket;
    como::UserAccepted userAcceptedPacket;
    const std::uint8_t* selectionColor;

    // Create the TCP resolver and query needed for connecting to the server.
    boost::asio::ip::tcp::resolver resolver( io_service_ );
    boost::asio::ip::tcp::resolver::query query( host, port );

    // Get the list of TCP endpoints given by resolving the previous query.
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );

    // Connect to the server.
    consoleMutex.lock();
    std::cout << "Connecting to the server..." << std::endl;
    consoleMutex.unlock();
    socket_->connect( *endpoint_iterator, errorCode );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR: Couldn't connect to server (" ) + errorCode.message() + ")" );
    }

    consoleMutex.lock();
    std::cout << "Connecting to the server...OK" << std::endl;
    consoleMutex.unlock();

    // Prepare a NEW_USER network package with the user name, and send it to
    // the server.
    newUserPacket.setName( userName );
    newUserPacket.send( *socket_ );

    // Read from the server an USER_ACCEPTED network package and unpack it.
    userAcceptedPacket.recv( *socket_ );

    consoleMutex.lock();
    selectionColor = userAcceptedPacket.getSelectionColor();
    std::cout << "User accepted: " << std::endl
              << "\tID: [" << userAcceptedPacket.getId() << "]" << std::endl
              << "\tName: [" << userAcceptedPacket.getName() << "]" << std::endl
              << "\tSelection color: [" << (int)selectionColor[0] << ", "
              << (int)selectionColor[1] << ", "
              << (int)selectionColor[2] << ", "
              << (int)selectionColor[3] << ")" << std::endl << std::endl;
    consoleMutex.unlock();

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when receiving USER_ACCEPTED package from server (" ) + errorCode.message() + ")" );
    }

    // Initialize the vector of translations from local User IDs to remote
    // ones with the remote ID given to the user by the server.
    localToRemoteUserID_.push_back( userAcceptedPacket.getId() );

    //listenerThread = new std::thread( std::bind( &ServerInterface::listen, this ) );
    listen();
}


void ServerInterface::disconnect()
{
    boost::system::error_code errorCode;

    consoleMutex.lock();
    std::cout << "Disconnecting from server ..." << std::endl;
    consoleMutex.unlock();

    // Close the socket to the server if it's open.
    if( socket_->is_open() ){
        consoleMutex.lock();
        std::cout << "Closing socket to server" << std::endl;
        consoleMutex.unlock();

        socket_->shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
        socket_->close();
        consoleMutex.lock();
        std::cout << "Closing socket to server... OK" << std::endl;
        consoleMutex.unlock();
    }

    // Stop the IO service and join the threads group.
    work_.reset();
    io_service_.stop();

    consoleMutex.lock();
    std::cout << "Waiting for working threads to finish ..." << std::endl;
    consoleMutex.unlock();
    workerThreads_.join_all();
    consoleMutex.lock();
    std::cout << "Waiting for working threads to finish ...OK" << std::endl;
    consoleMutex.unlock();

    // Clear the vector of translations from local user IDs to remote ones.
    localToRemoteUserID_.clear();

    consoleMutex.lock();
    std::cout << "Disconnecting from server ...OK" << std::endl;
    consoleMutex.unlock();
}


/***
 * 3. Handlers
 ***/

void ServerInterface::onSceneUpdateReceived( const boost::system::error_code& errorCode, PacketPtr packet )
{
    if( errorCode ){
        consoleMutex.lock();
        std::cout << "Error when receiving packet: " << errorCode.message() << std::endl;
        consoleMutex.unlock();
        return;
    }

    unsigned int i;
    const std::vector< SceneCommandConstPtr >* sceneCommands = nullptr;

    consoleMutex.lock();
    std::cout << "Packet received" << std::endl;
    consoleMutex.unlock();

    SceneUpdate* sceneUpdate = dynamic_cast< SceneUpdate *>( packet.get() );
    //const UserConnected* userConnectedCommand = nullptr;

    if( !sceneUpdate ){
        throw std::runtime_error( std::string( "ERROR in \"onSceneUpdateReceived\": not a SCENE_UPDATE packet" ) );
    }

    consoleMutex.lock();
    std::cout << "nCommands: " << sceneUpdate->getCommands()->size() << std::endl;

    sceneCommands = sceneUpdate->getCommands();
    for( i=0; i<sceneCommands->size(); i++ ){
        std::cout << "\tCommand[" << i << "]: ";
        switch( ( ( *sceneCommands )[i] )->getType() ){
            case SceneCommandType::USER_CONNECTED:
                std::cout << "USER_CONNECTED" << std::endl;
            break;
            case SceneCommandType::USER_DISCONNECTED:
                std::cout << "USER_DISCONNECTED" << std::endl;
            break;
        }
    }


    /*userConnectedCommand = dynamic_cast< const UserConnected* >( ( (*( sceneUpdate->getCommands() ) )[0] ).get() );

    std::cout << "First command of type USER_CONNECTED: " << userConnectedCommand << std::endl;
    std::cout << "User connected: [" << userConnectedCommand->getName() << "]" << std::endl;
    */

    consoleMutex.unlock();

    listen();
}


void ServerInterface::listen()
{   
    consoleMutex.lock();
    std::cout << "Listening for new scene updates from server ..." << std::endl;
    consoleMutex.unlock();

    sceneUpdatePacketFromServer_.asyncRecv( socket_, std::bind( &ServerInterface::onSceneUpdateReceived, this, std::placeholders::_1, std::placeholders::_2 ) );
}


void ServerInterface::work()
{
    boost::system::error_code errorCode;
    bool exit = false;

    consoleMutex.lock();
    std::cout << "[" << boost::this_thread::get_id() << "] thread start" << std::endl;
    consoleMutex.unlock();

    while( !exit ){
        try{
            consoleMutex.lock();
            std::cout << "[" << boost::this_thread::get_id() << "] io_service::run() ... " << std::endl;
            consoleMutex.unlock();
            io_service_.run( errorCode );
            consoleMutex.lock();
            std::cout << "[" << boost::this_thread::get_id() << "] io_service::run() ...OK" << std::endl;
            consoleMutex.unlock();
            if( errorCode ){
                consoleMutex.lock();
                std::cout << "[" << boost::this_thread::get_id() << "] Error: " << errorCode.message() << std::endl;
                consoleMutex.unlock();
            }
            exit = true;
        }catch( std::exception& ex ){
            // TODO: close the server correctly.
            throw ex;
        }
    }

    consoleMutex.lock();
    std::cout << "[" << boost::this_thread::get_id() << "] thread end" << std::endl;
    consoleMutex.unlock();
}

} // namespace como
