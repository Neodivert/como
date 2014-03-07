/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef PACKET_HPP
#define PACKET_HPP

#include "../packables/packables.hpp"
#include "../utilities/packer.hpp"
#include <stdexcept>
#include <boost/asio.hpp>
#include <functional>
#include <boost/bind.hpp>
#include "packet_header.hpp"

namespace como {

class Packet;

typedef boost::asio::ip::tcp::socket Socket;
typedef std::shared_ptr< Socket > SocketPtr;
typedef std::shared_ptr< Packet > PacketPtr;
typedef std::function<void( const boost::system::error_code& errorCode, PacketPtr)> PacketHandler;


const unsigned int PACKET_BUFFER_SIZE = 512;


class Packet : public CompositePackable
{
    private:
        PacketHeader header_;

        char buffer_[PACKET_BUFFER_SIZE];

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Packet() = delete ;
        Packet( PacketType type );
        Packet( const Packet& b );
        Packet( Packet&& ) = delete;
        virtual Packet* clone() const = 0;

        ~Packet() = default;

        const char* getBuffer() const { return buffer_; }


        /***
         * 2. Socket communication
         ***/
        void send( boost::asio::ip::tcp::socket& socket );
        void asyncSend( SocketPtr socket, PacketHandler packetHandler );

        void recv( boost::asio::ip::tcp::socket& socket );
        void asyncRecv( SocketPtr socket, PacketHandler packetHandler );

    private:
        void updateHeader();
        void asyncSendBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket, PacketHandler packetHandler );
        void asyncRecvBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket, PacketHandler packetHandler );
        void onPacketRecv( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler );
        void onPacketSend( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler );
    public:


        /***
         * 3. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const ;

        virtual void* packHeader( void* buffer ) const;
        virtual const void* unpackHeader( const void* buffer );
        virtual const void* unpackHeader( const void* buffer ) const ;

        virtual void* packBody( void* buffer ) const;
        virtual const void* unpackBody( const void* buffer );
        virtual const void* unpackBody( const void* buffer ) const ;


        /***
         * 4. Getters
         ***/
        PacketType getType() const ;
        virtual bool expectedType() const = 0;


        /***
         * 5. Operators
         ***/
        Packet& operator = ( const Packet& b );
        Packet& operator = ( Packet&& ) = delete;
};

} // namespace como

#endif // PACKET_HPP
