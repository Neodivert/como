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
    acceptor_( io_service_ ),
    work_( io_service_ ),
    N_THREADS( nThreads ),
    MAX_SESSIONS( maxSessions ),
    newSocket_( io_service_ ),
    newId_( 0 ),
    port_( port_ )
{
    unsigned int i;

    // Create the threads pool.
    for( i=0; i<N_THREADS; i++ ){
        threads_.create_thread( boost::bind( &Server::workerThread, this ) );
    }
}


/***
 * 2. Main loop
 ***/

void Server::run()
{
    // Create a TCP resolver and a query for getting the endpoint the server
    // must listen to.
    boost::asio::ip::tcp::resolver resolver( io_service_ );
    boost::asio::ip::tcp::resolver::query query( "localhost", boost::lexical_cast< std::string >( port_ ) );

    try{
        coutMutex_.lock();
        std::cout << "Press any key to exit" << std::endl;
        coutMutex_.unlock();

        // Resolve the query to localhost:port and get its TCP end point.
        boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve( query );
        endPoint_ = *iterator;

        // Open the acceptor.
        openAcceptor();

        // Wait for new connections.
        listen();

        // User only needs to press any key to stop the server.
        std::cin.get();

        boost::system::error_code errorCode;

        // Stop the I/O processing.
        io_service_.stop();

        // TODO: Are these methods called in their corresponding destructors?
        //newSocket_.shutdown( boost::asio::ip::tcp::socket::shutdown_both, errorCode );
        //newSocket_.close( errorCode );

        // Free TCP acceptor.
        //acceptor_.close( errorCode );

        // Wait for the server's threads to finish.
        threads_.join_all();
    }catch( std::exception& ex ){
        coutMutex_.lock();
        std::cout << "Exception: " << ex.what() << std::endl;
        coutMutex_.unlock();
    }
}


/***
 * 3. Listeners
 ***/

void Server::listen()
{
    coutMutex_.lock();
    std::cout << "Listening on port (" << port_ << ")" << std::endl;
    coutMutex_.unlock();

    // Wait for a new user connection.
    acceptor_.async_accept( newSocket_, boost::bind( &Server::onAccept, this, _1 ) );
}


/***
 * 4. Handlers
 ***/

void Server::onAccept( const boost::system::error_code& errorCode )
{
    boost::system::error_code closingErrorCode;
    boost::system::error_code readingErrorCode;

    como::NewUser newUserPacket;
    como::UserAccepted userAcceptedPacket;
    char buffer[128];

    if( errorCode ){
        coutMutex_.lock();
        std::cout << "[" << boost::this_thread::get_id() << "]: ERROR(" << errorCode.message() << ")" << std::endl;
        coutMutex_.unlock();
    }else{
        // Connection established. Wait synchronously for a NEW_USER package.
        boost::asio::read( newSocket_, boost::asio::buffer( buffer ), readingErrorCode );

        // Unpack the received NEW_USER package and prompt the new user's name.
        newUserPacket.unpack( buffer );
        coutMutex_.lock();
        std::cout << "Package received [type: " << (int)( newUserPacket.getType() ) << "]" << std::endl
                  << "\tName: " << newUserPacket.getName() << std::endl;

        std::cout << "[" << newUserPacket.getName() << "] (" << boost::this_thread::get_id() << "): Connected!" << std::endl;
        coutMutex_.unlock();

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
        userAcceptedPacket.pack( buffer );
        boost::asio::write( newSocket_, boost::asio::buffer( buffer ), readingErrorCode );

        // Add the new user to the sessions vector.
        sessions_.push_back( std::make_shared<Session>( newId_, userAcceptedPacket.getName(), std::move( newSocket_ ), std::bind( &Server::deleteSession, this, std::placeholders::_1 ) ) );

        // Increment the "new id" for the next user.
        newId_++;

        coutMutex_.lock();
        std::cout << "New sessions: (" << sessions_.back()->getId() << ") - total sessions: " << sessions_.size() << std::endl;
        coutMutex_.unlock();

        if( sessions_.size() < MAX_SESSIONS ){
            // There is room for more users, wait for a new connection.
            listen();
        }else{
            // There isn't room for more users, close the acceptor.
            acceptor_.close( closingErrorCode );
            //acceptor_.cancel();

            coutMutex_.lock();
            std::cout << "Server is full (MAX_SESSIONS: " << MAX_SESSIONS << ")" << std::endl;
            //std::cout << "acceptor_.close() : " << closingErrorCode << std::endl;
            coutMutex_.unlock();
        }
    }
}


/***
 * 5. Auxiliar methods
 ***/

void Server::deleteSession( unsigned int id )
{
    unsigned int i = 0;
    std::cout << "Server::deleteSession(" << id << ")" << std::endl;

    while( ( i < sessions_.size() ) &&
           ( id != sessions_[i]->getId() ) ){
        i++;
    }

    if( i < sessions_.size() ){
        std::cout << "Erasing (id: " << id << ") - (id: " << id << ")" << std::endl;
        sessions_.erase( sessions_.begin() + i );
    }

    if( sessions_.size() == (MAX_SESSIONS - 1) ){
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
    coutMutex_.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Start" << std::endl;
    coutMutex_.unlock();

    while( true )
    {
        try
        {
            boost::system::error_code ec;
            io_service_.run( ec );
            if( ec )
            {
                coutMutex_.lock();
                std::cout << "[" << boost::this_thread::get_id()
                    << "] Error: " << ec.message() << std::endl;
                coutMutex_.unlock();
            }
            break;
        }
        catch( std::exception & ex )
        {
            coutMutex_.lock();
            std::cout << "[" << boost::this_thread::get_id()
                << "] Exception: " << ex.what() << std::endl;
            coutMutex_.unlock();
        }
    }

    coutMutex_.lock();
    std::cout << "[" << boost::this_thread::get_id()
        << "] Thread Finish" << std::endl;
    coutMutex_.unlock();
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
    unsigned int i = 0;

    for( ; i<sessions_.size(); i++ ){
        if( !strcmp( newName, sessions_[i]->getName() ) ){
            return true;
        }
    }

    return false;
}

} // namespace como
