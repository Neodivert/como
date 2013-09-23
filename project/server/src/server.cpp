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

Server::Server( int port ) :
    // Create an acceptor for listening for new connections. It will listen
    // on TCP port 13, for IPv4.
    acceptor( io_service, tcp::endpoint(tcp::v4(), port ) )
{
}

/***
 * 2. Main loop
 ***/

void Server::run()
{
    std::string message;
    boost::system::error_code ignored_error;

    while( true ){
        // Create a TCP socket and start listening.
        std::cout << "Listening ..." << std::endl;
        tcp::socket socket( io_service );
        acceptor.accept( socket );


        // Someone has connected to the server! Get the current day time and
        // send it to the client.is accessing our service.
        message = getCurrentDayTime();
        boost::asio::write( socket, boost::asio::buffer(message), ignored_error );
        std::cout << "Client attended!" << std::endl << std::endl;
    }
}


/***
 * 3. Auxiliar methods
 ***/

std::string Server::getCurrentDayTime() const
{
    std::time_t now = std::time( 0 );
    return std::ctime(&now);
}

} // namespace como
