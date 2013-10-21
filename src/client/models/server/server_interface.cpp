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

ServerInterface::ServerInterface() :
    socket_( io_service_ )
{
}


void ServerInterface::connect( const char* host, const char* port, const char* userName )
{
    boost::system::error_code errorCode;
    como::NewUser newUserPackage;
    como::UserAccepted userAcceptedPackage;
    char buffer[128];

    // Create the TCP resolver and query needed for connecting to the server.
    boost::asio::ip::tcp::resolver resolver( io_service_ );
    boost::asio::ip::tcp::resolver::query query( host, port );

    // Get the list of TCP endpoints given by resolving the previous query.
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );

    // Connect to the server.
    std::cout << "Connecting ..." << std::endl;
    socket_.connect( *endpoint_iterator, errorCode );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR: Couldn't connect to server (" ) + errorCode.message() + ")" );
    }

    std::cout << "Connected!" << std::endl;

    // Prepare a NEW_USER network package with the user name, and send it to
    // the server.
    strcpy( newUserPackage.name, userName );
    newUserPackage.pack( buffer );
    boost::asio::write( socket_, boost::asio::buffer( buffer ), errorCode );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when sending NEW_USER package to server (" ) + errorCode.message() + ")" );
    }

    // Read from the server an USER_ACCEPTED network package and unpack it.
    boost::asio::read( socket_, boost::asio::buffer( buffer ), errorCode );
    userAcceptedPackage.unpack( buffer );

    std::cout << "User accepted: " << std::endl
              << "\tID: [" << userAcceptedPackage.id << "]" << std::endl
              << "\tName: [" << userAcceptedPackage.name << "]" << std::endl
              << "\tSelection color: [" << (int)userAcceptedPackage.selectionColor[0] << ", "
              << (int)userAcceptedPackage.selectionColor[1] << ", "
              << (int)userAcceptedPackage.selectionColor[2] << ", "
              << (int)userAcceptedPackage.selectionColor[3] << ")" << std::endl << std::endl;

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when receiving USER_ACCEPTED package from server (" ) + errorCode.message() + ")" );
    }

    std::cout << "Press any key to close the connection" << std::endl;
    std::cin.get();

    // TODO: Delete this in future versions.
    // Close the socket.
    socket_.close();
}


} // namespace como
