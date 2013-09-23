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

/*
 * The server code was created from the following example:
 *
 * Daytime.2 - A synchronous TCP daytime server
 * http://www.boost.org/doc/libs/1_54_0/doc/html/boost_asio/tutorial/tutdaytime2.html
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

namespace como {

class Server
{
    private:
        // I/O service.
        boost::asio::io_service io_service;

        // TCP acceptor.
        tcp::acceptor acceptor;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Server( int port );


        /***
         * 2. Main loop
         ***/
        void run();


        /***
         * 3. Auxiliar methods
         ***/
    private:
        std::string getCurrentDayTime() const ;
};

} // namespace como

#endif // SERVER_HPP
