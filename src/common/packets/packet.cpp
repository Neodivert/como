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

#include "packet.hpp"

namespace como {


/***
 * 1. Initialization and destruction
 ***/

Packet::Packet( PacketType type ) :
    type_( type ),
    bodySize_( 0 )
{
}


/***
 * 2. Socket communication
 ***/


void Packet::send( SocketPtr socket )
{
    char buffer[256];
    boost::system::error_code errorCode;

    // Pack the packet's header into the buffer.
    packHeader( buffer );

    // Write synchronously the packet's header to the socket.
    boost::asio::write( *socket, boost::asio::buffer( buffer, (int)getHeaderSize() ), errorCode );
    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when sending packet header' (" ) + errorCode.message() + ")" );
    }

    // Pack the packet's body into the buffer.
    packBody( buffer );

    // Write synchronously the packet's body to the socket.
    boost::asio::write( *socket, boost::asio::buffer( buffer, (int)bodySize_ ), errorCode );
    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when sending packet body' (" ) + errorCode.message() + ")" );
    }
}


void Packet::asyncSend( SocketPtr socket, PacketHandlerPtr packetHandler )
{
    // Pack the packet's header into the buffer.
    packHeader( buffer_ );

    packetSendHandler_ = packetHandler;

    // Write asynchronously the packet's header to the socket.
    boost::asio::async_write(
                *socket,
                boost::asio::buffer( buffer_, (int)getHeaderSize() ),
                boost::bind( &Packet::asyncSendBody, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, socket/*, packetHandler*/ )
                );

    //boost::asio::write( socket, boost::asio::buffer( buffer, (int)getHeaderSize() ), errorCode );
}

void Packet::asyncSendBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket/*, PacketHandlerPtr packetHandler*/ )
{
    if( headerErrorCode ){
        throw std::runtime_error( std::string( "ERROR when sending packet header' (" ) + headerErrorCode.message() + ")" );
    }

    // Pack the packet's body into the buffer.
    packBody( buffer_ );

    // Write synchronously the packet's body to the socket.
    // Write asynchronously the packet's header to the socket.
    boost::asio::async_write(
                *socket,
                boost::asio::buffer( buffer_, (int)bodySize_ ),
                [&,this]( boost::system::error_code ec, std::size_t ){
                    if( ec ){
                        throw std::runtime_error( std::string( "ERROR when sending packet body' (" ) + ec.message() + ")" );
                    }
                    //(*packetHandler)( std::make_shared( *this ) );
    });
}


void Packet::recv( boost::asio::ip::tcp::socket& socket )
{
    char buffer[256];
    boost::system::error_code errorCode;

    // Read synchronously the packet's header from the socket.
    boost::asio::read( socket, boost::asio::buffer( buffer, (int)getHeaderSize() ), errorCode );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when receiving packet header' (" ) + errorCode.message() + ")" );
    }

    // Unpack the packet's header from the buffer and check its type.
    unpackHeader( buffer );
    if( !expectedType() ){
        throw std::runtime_error( std::string( "ERROR: Received an unexpected packet" ) );
    }

    // Read synchronously the packet's body from the socket.
    boost::asio::read( socket, boost::asio::buffer( buffer, (int)bodySize_ ), errorCode );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when receiving packet body' (" ) + errorCode.message() + ")" );
    }

    // Unpack the packet's body.
    unpackBody( buffer );
}


/***
 * 2. Packing and unpacking
 ***/

char* Packet::pack( char* buffer ) const
{
    // Pack the packet's header.
    buffer = packHeader( buffer );

    // Pack the packet's body and return the updated buffer pointer (the
    // packing updates it).
    return packBody( buffer );
}


const char* Packet::unpack( const char* buffer )
{
    // Unpack the packet's header.
    buffer = unpackHeader( buffer );

    // Unpack the packet's body and return the updated buffer pointer (the
    // unpacking updates it).
    return unpackBody( buffer );
}


char* Packet::packHeader( char* buffer ) const
{
    // Pack the packet's type.
    packer::pack( static_cast< std::uint8_t >( type_ ), buffer );

    // Pack the body's size.
    packer::pack( bodySize_, buffer );

    return buffer;
}

const char* Packet::unpackHeader( const char* buffer )
{
    std::uint8_t type;

    // Unpack the packet's type.
    packer::unpack( type, buffer );
    type_ = static_cast< PacketType >( type );

    // Unpack the body's size.
    packer::unpack( bodySize_, buffer );

    return buffer;
}


/***
 * 3. Getters
 ***/

PacketType Packet::getType() const
{
    return type_;
}


std::uint16_t Packet::getBodySize() const
{
    return bodySize_;
}


std::uint8_t Packet::getHeaderSize()
{
    return sizeof( type_ ) + sizeof( bodySize_ );
}


std::uint16_t Packet::getPacketSize() const
{
    return getHeaderSize() + getBodySize();
}


} // namespace como

