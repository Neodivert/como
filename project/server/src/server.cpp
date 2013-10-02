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

Server::Server( unsigned int port_, unsigned int nThreads ) :
    // Initialize the server parameters.
    acceptor_( io_service ),
    work( io_service ),
    newSocket_( io_service ),
    N_THREADS( nThreads ),
    MAX_USERS( 2 ),
    port( port_ )
{
    unsigned int i;

    // Create the threads pool.
    for( i=0; i<N_THREADS; i++ ){
        threads.create_thread( boost::bind( &Server::workerThread, this ) );
    }
}


/***
 * 2. Main loop
 ***/

void Server::run()
{
    // Create a TCP resolver and a query for getting the endpoint the server
    // must listen to.
    boost::asio::ip::tcp::resolver resolver( io_service );
    boost::asio::ip::tcp::resolver::query query( "localhost", boost::lexical_cast< std::string >( port ) );

    try{
        coutMutex.lock();
        std::cout << "Press any key to exit" << std::endl;
        coutMutex.unlock();

        // Resolve the query to localhost:port and get its TCP end point.
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
        boost::asio::ip::tcp::endpoint endPoint = *iterator;

        // Set the acceptor parameters.
        acceptor_.open( endPoint.protocol() );
        acceptor_.set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );

        // Start listening.
        // FIXME: Sometimes I get an exception "bind address already in use"
        acceptor_.bind( endPoint );
        acceptor_.listen( MAX_USERS );

        // Wait for new connections.
        listen();

        // User only needs to press any key to stop the server.
        std::cin.get();

        boost::system::error_code errorCode;

        // Stop the I/O processing.
        io_service.stop();

        // Close all user connections.
        for( unsigned int i = 0; i<users.size(); i++ ){
            users[i].socket.shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
            users[i].socket.close( errorCode );
        }

        // Free TCP acceptor.
        acceptor_.close( errorCode );

        // Wait for the server's threads to finish.
        threads.join_all();
    }catch( std::exception& ex ){
        coutMutex.lock();
        std::cout << "Exception: " << ex.what() << std::endl;
        coutMutex.unlock();
    }
}


/***
 * 3. Listeners
 ***/

void Server::listen()
{
    coutMutex.lock();
    std::cout << "Listening on port (" << port << ")" << std::endl;
    coutMutex.unlock();

    // Wait for a new user connection.
    acceptor_.async_accept( newSocket_, boost::bind( &Server::onAccept, this, _1 ) );
}


/***
 * 4. Handlers
 ***/

void Server::onAccept( const boost::system::error_code& errorCode )
{
    if( errorCode ){
        coutMutex.lock();
        std::cout << "[" << boost::this_thread::get_id() << "]: ERROR(" << errorCode << ")" << std::endl;
        coutMutex.unlock();
    }else{
        coutMutex.lock();
        std::cout << "[" << boost::this_thread::get_id() << "]: Connected!" << std::endl;
        coutMutex.unlock();

        // Add the new user to the users vector.
        users.emplace_back( users.size(), std::move( newSocket_ ) );

        coutMutex.lock();
        std::cout << "New user: (" << users.back().id << ") - total users: " << users.size() << std::endl;
        coutMutex.unlock();

        if( users.size() < MAX_USERS ){
            // Wait for a new connection.
            listen();
        }else{
            coutMutex.lock();
            std::cout << "Server is full (MAX_USERS: " << MAX_USERS << ")" << std::endl;
            coutMutex.unlock();
        }
    }
}


/***
 * 5. Auxiliar methods
 ***/

std::string Server::getCurrentDayTime() const
{
    std::time_t now = std::time( 0 );
    return std::ctime(&now);
}


void Server::workerThread()
{
    coutMutex.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Start" << std::endl;
    coutMutex.unlock();

    while( true )
    {
        try
        {
            boost::system::error_code ec;
            io_service.run( ec );
            if( ec )
            {
                coutMutex.lock();
                std::cout << "[" << boost::this_thread::get_id()
                    << "] Error: " << ec << std::endl;
                coutMutex.unlock();
            }
            break;
        }
        catch( std::exception & ex )
        {
            coutMutex.lock();
            std::cout << "[" << boost::this_thread::get_id()
                << "] Exception: " << ex.what() << std::endl;
            coutMutex.unlock();
        }
    }

    coutMutex.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Finish" << std::endl;
    coutMutex.unlock();
}

} // namespace como
