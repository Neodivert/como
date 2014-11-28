/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "packet.hpp"

namespace como {


/***
 * 1. Construction
 ***/

Packet::Packet( PacketType type ) :
    header_( type ),
    headerBuffer_{ 0 }
{}

Packet::Packet( const Packet& b ) :
    CompositePackable(),
    header_( b.header_ ),
    bodyBuffer_{ b.bodyBuffer_ }
{
    strncpy( headerBuffer_, b.headerBuffer_, PACKET_HEADER_BUFFER_SIZE );
}


/***
 * 3. Getters.
 ***/

PacketType Packet::getType() const
{
    return header_.getType();
}


/***
 * 4. Synchronous communication.
 ***/

void Packet::send( boost::asio::ip::tcp::socket& socket )
{
    boost::system::error_code errorCode;

    // Update and pack the packet's header into the buffer.
    updateHeader();
    packHeader( headerBuffer_ );

    // Write synchronously the packet's header into the socket.
    boost::asio::write( socket, boost::asio::buffer( headerBuffer_, (int)( header_.getPacketSize() ) ), errorCode );
    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when sending packet header' (" ) + errorCode.message() + ")" );
    }

    // Pack the packet's body into the buffer.
    bodyBuffer_.resize( header_.getBodySize() );
    packBody( &( bodyBuffer_[0] ) );

    // Write synchronously the packet's body into the socket.
    boost::asio::write( socket, boost::asio::buffer( &( bodyBuffer_[0] ), (int)( header_.getBodySize() ) ), errorCode );
    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when sending packet body' (" ) + errorCode.message() + ")" );
    }
    bodyBuffer_.clear();
}


void Packet::recv( boost::asio::ip::tcp::socket& socket )
{
    boost::system::error_code errorCode;

    // Read synchronously the packet's header from the socket.
    boost::asio::read( socket, boost::asio::buffer( headerBuffer_, (int)( header_.getPacketSize() ) ), errorCode );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when receiving packet header' (" ) + errorCode.message() + ")" );
    }

    // Unpack the packet's header from the buffer and check its type.
    unpackHeader( headerBuffer_ );
    if( !expectedType() ){
        throw std::runtime_error( std::string( "ERROR: Received an unexpected packet" ) );
    }

    // Read synchronously the packet's body from the socket.
    bodyBuffer_.resize( header_.getBodySize() );
    boost::asio::read( socket, boost::asio::buffer( &( bodyBuffer_[0] ), (int)( header_.getBodySize() ) ), errorCode );

    if( errorCode ){
        throw std::runtime_error( std::string( "ERROR when receiving packet body' (" ) + errorCode.message() + ")" );
    }

    // Unpack the packet's body.
    unpackBody( &( bodyBuffer_[0] ) );

    bodyBuffer_.clear();
}


/***
 * 5. Asynchronous communication
 ***/

void Packet::asyncSend( Socket& socket, PacketHandler packetHandler )
{
    // Update and pack the packet's header into the buffer.
    updateHeader();
    packHeader( headerBuffer_ );

    // Write asynchronously the packet's header into the socket.
    boost::asio::async_write(
                socket,
                boost::asio::buffer( headerBuffer_, (int)( header_.getPacketSize() ) ),
                boost::bind( &Packet::asyncSendBody, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, boost::ref( socket ), packetHandler )
                );
}


void Packet::asyncRecv( Socket& socket, PacketHandler packetHandler )
{
    // Read asynchronously the packet's header from the socket.
    boost::asio::async_read(
                socket,
                boost::asio::buffer( headerBuffer_, (int)( header_.getPacketSize() ) ),
                boost::bind( &Packet::asyncRecvBody, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred, boost::ref( socket ), packetHandler )
                );
}


/***
 * 6. Packing and unpacking
 ***/

void* Packet::pack( void* buffer ) const
{
    buffer = packHeader( buffer );
    return packBody( buffer );
}


const void* Packet::unpack( const void* buffer )
{
    buffer = unpackHeader( buffer );
    return unpackBody( buffer );
}


const void* Packet::unpack( const void* buffer ) const
{
    buffer = unpackHeader( buffer );
    return unpackBody( buffer );
}


/***
 * 7. Operators
 ***/

Packet& Packet::operator =( const Packet& b )
{
    if( this != &b ){
        header_ = b.header_;
        strncpy( headerBuffer_, b.headerBuffer_, PACKET_HEADER_BUFFER_SIZE );
        bodyBuffer_ = b.bodyBuffer_;
    }

    return *this;
}


/***
 * 8. Asynchronous communication (private)
 ***/

void Packet::asyncSendBody( const boost::system::error_code& headerErrorCode, std::size_t, Socket& socket, PacketHandler packetHandler )
{
    if( headerErrorCode ){
        packetHandler( headerErrorCode, PacketPtr( clone() ) );
        return;
    }

    // Pack the packet's body into the buffer.
    bodyBuffer_.resize( header_.getBodySize() );
    packBody( &( bodyBuffer_[0] ) );

    // Write asynchronously the packet's body into the socket.
    boost::asio::async_write(
                socket,
                boost::asio::buffer( &( bodyBuffer_[0] ), (int)( header_.getBodySize() ) ),
                boost::bind( &Packet::onPacketSend, this, _1, _2, packetHandler )
                );
}

void Packet::onPacketSend( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler )
{
    // Call the packet handler.
    packetHandler( errorCode, PacketPtr( clone() ) );

    bodyBuffer_.clear();
}


void Packet::asyncRecvBody( const boost::system::error_code& headerErrorCode, std::size_t, Socket& socket, PacketHandler packetHandler )
{
    if( headerErrorCode ){
        packetHandler( headerErrorCode, PacketPtr( clone() ) );
        return;
    }

    // Unpack the packet's header from the buffer.
    unpackHeader( headerBuffer_ );

    // Check that the received packet has the expected type.
    if( !expectedType() ){
        throw std::runtime_error( std::string( "ERROR: Unexpected packet" ) );
    }

    // Write synchronously the packet's body to the socket.
    // Write asynchronously the packet's header to the socket.
    bodyBuffer_.resize( header_.getBodySize() );
    boost::asio::async_read(
                socket,
                boost::asio::buffer( &( bodyBuffer_[0] ), (int)( header_.getBodySize() ) ),
                boost::bind( &Packet::onPacketRecv, this, _1, _2, packetHandler ) );
}


void Packet::onPacketRecv( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler )
{   
    if( !errorCode ){
        // Unpack the packet's body from the buffer.
        unpackBody( &( bodyBuffer_[0] ) );
    }

    // Call the packet handler.
    packetHandler( errorCode, PacketPtr( clone() ) );

    bodyBuffer_.clear();
}


/***
 * 9. Packing and unpacking (private)
 ***/

void* Packet::packHeader( void* buffer ) const
{
    return header_.pack( buffer );
}


void* Packet::packBody( void* buffer ) const
{
    return CompositePackable::pack( buffer );
}


const void* Packet::unpackHeader( const void* buffer )
{
    return header_.unpack( buffer );
}


const void* Packet::unpackBody( const void* buffer )
{
    return CompositePackable::unpack( buffer );
}


const void* Packet::unpackHeader( const void* buffer ) const
{
    return header_.unpack( buffer );
}


const void* Packet::unpackBody( const void* buffer ) const
{
    return CompositePackable::unpack( buffer );
}


/***
 * 10. Auxiliar networking methods.
 ***/

void Packet::updateHeader()
{
    // Update the value for the packet body size to be packed.
    header_.setBodySize( getPacketSize() );
}

} // namespace como
