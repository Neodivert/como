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
 * The server code was created thanks to the following tutorial:
 *
 * A guide to getting started with boost::asio
 * http://www.gamedev.net/blog/950/entry-2249317-a-guide-to-getting-started-with-boostasio/
 */

#ifndef SERVER_HPP
#define SERVER_HPP

#include <ctime>
#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/lexical_cast.hpp>
#include "session.hpp"
#include "../common/packets/packets.hpp"

using boost::asio::ip::tcp;

namespace como {

class Server
{
    private:
        // I/O service.
        boost::asio::io_service io_service_;

        // Aceptor
        boost::asio::ip::tcp::acceptor acceptor_;

        // Work object.
        boost::asio::io_service::work work_;

        // Sessions vector.
        std::vector< SessionPtr > sessions_;

        // Number of worker threads in the server.
        const unsigned int N_THREADS;

        // Maximum number of sessions (users) allowed in the server.
        const unsigned int MAX_SESSIONS;

        // End point the acceptor listens to.
        boost::asio::ip::tcp::endpoint endPoint_;

        // Threads pool
        boost::thread_group threads_;

        // Mutex for console output (cout).
        boost::mutex coutMutex_;

        // New socket
        Socket newSocket_;

        // When a new user connect to the server, this will be the id givent to him/her.
        unsigned int newId_;

        // Server's port.
        unsigned int port_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Server( unsigned int port_, unsigned int maxSessions, unsigned int nThreads = 1 );


        /***
         * 2. Main loop
         ***/
        void run();


        /***
         * 3. Listeners
         ***/
        void listen();


        /***
         * 4. Handlers
         ***/
        void onAccept( const boost::system::error_code& errorCode );


        /***
         * 5. Auxiliar methods
         ***/
        void deleteSession( unsigned int id );
    private:
        std::string getCurrentDayTime() const ;
        void workerThread();
        void openAcceptor();
        bool nameInUse( const char* newName ) const ;
};

} // namespace como

#endif // SERVER_HPP
