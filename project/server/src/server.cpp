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
    work( io_service ),
    socket( io_service ),
    N_THREADS( nThreads ),
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

        // Create a TCP acceptor and set its parameters.
        boost::shared_ptr< boost::asio::ip::tcp::acceptor > acceptor( new boost::asio::ip::tcp::acceptor( io_service ) );
        acceptor->open( endPoint.protocol() );
        acceptor->set_option( boost::asio::ip::tcp::acceptor::reuse_address( false ) );

        // Start listening.
        acceptor->bind( endPoint );
        acceptor->listen( boost::asio::socket_base::max_connections );

        // Wait for new connections.
        acceptor->async_accept( socket, boost::bind( &Server::onAccept, this, _1 ) );

        coutMutex.lock();
        std::cout << "Listening on: " << endPoint << std::endl;
        coutMutex.unlock();
        std::cin.get();

        boost::system::error_code errorCode;

        // Free TCP acceptor and socket.
        acceptor->close( errorCode );
        socket.shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
        socket.close( errorCode );

        // Stop the I/O processing.
        io_service.stop();

        // Wait for the server's threads to finish.
        threads.join_all();
    }catch( std::exception& ex ){
        std::cout << "Exception: " << ex.what() << std::endl;
    }

}


/***
 * 3. Handlers
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
    }
}


/***
 * 4. Auxiliar methods
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
