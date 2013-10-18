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


bool ServerInterface::connect( const char* host, const char* port )
{
    boost::system::error_code errorCode;

    // Create the TCP resolver and query needed for connecting to the server.
    boost::asio::ip::tcp::resolver resolver( io_service_ );
    boost::asio::ip::tcp::resolver::query query( host, port );

    // Get the list of TCP endpoints given by resolving the previous query.
    boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve( query );

    // Connect to the server.
    std::cout << "Connecting ..." << std::endl;
    socket_.connect( *endpoint_iterator, errorCode );

    if( errorCode ){
        std::cerr << "ERROR: Couldn't connect to server" << "(" << errorCode.message() << ")" << std::endl;
        return false;
    }

    std::cout << "Connected!" << std::endl;

    socket_.close();

    return true;
}


} // namespace como
