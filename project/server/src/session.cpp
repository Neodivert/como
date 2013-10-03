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

#include "session.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

Session::Session( unsigned int id, boost::asio::ip::tcp::socket socket ) :
    id_( id ),
    socket_( std::move( socket ) )
{
    std::cout << "Session created (id: " << id_ << ")" << std::endl;
    read();
}


Session::~Session()
{
    std::cout << "Session (" << id_ << ") destroyed" << std::endl;
}


/***
 * 2. Getters
 ***/

unsigned int Session::getId()
{
    return id_;
}


/***
 * 3. Socket reading
 ***/

void Session::read()
{
    socket_.async_read_some( boost::asio::buffer( buffer_, BUFFER_SIZE ), boost::bind( &Session::onRead, this, _1, _2 ) );
}

void Session::onRead( const boost::system::error_code& errorCode, std::size_t length )
{
    if( errorCode ){
        if( errorCode == boost::asio::error::eof ){
            std::cout << "User disconnected" << std::endl;
        }else{
            std::cout << "onRead - ERROR: " << errorCode << std::endl;
        }
    }else{
        std::cout << "Client " << id_ << " : ";
        std::cout.write( buffer_, length );

        read();
    }
}

} // namespace como
