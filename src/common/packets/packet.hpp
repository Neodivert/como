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

#ifndef PACKAGE_HPP
#define PACKAGE_HPP

#include "packable.hpp"
#include "../utilities/packer.hpp"
#include <stdexcept>
#include <boost/asio.hpp>
#include <functional>
#include <boost/bind.hpp>

class Packet;

typedef boost::asio::ip::tcp::socket Socket;
typedef std::shared_ptr< Socket > SocketPtr;
typedef std::shared_ptr< Packet > PacketPtr;
typedef std::shared_ptr< std::function<void(PacketPtr)> > PacketHandlerPtr;

namespace como {

enum class PacketType : std::int8_t
{
    NEW_USER = 0,
    USER_ACCEPTED = 1,
    SCENE_UPDATE = 2
};

class Packet : public Packable
{
    private:
        PacketType type_;
    protected:
        std::uint16_t bodySize_;

        char buffer_[256];

        PacketHandlerPtr packetSendHandler_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Packet( PacketType type );


        /***
         * 2. Socket communication
         ***/
        void send( SocketPtr socket );

        void asyncSend( SocketPtr socket, PacketHandlerPtr packetHandler );
        void recv( boost::asio::ip::tcp::socket& socket );
    private:
        void asyncSendBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket/*, PacketHandlerPtr packetHandler*/ );
    public:


        /***
         * 2. Packing and unpacking
         ***/
        char* pack( char* buffer ) const ;
        const char* unpack( const char* buffer );
        char* packHeader( char* buffer ) const ;
        const char* unpackHeader( const char* buffer );
        virtual char* packBody( char* buffer ) const = 0;
        virtual const char* unpackBody( const char* buffer ) = 0;


        /***
         * 3. Getters
         ***/
        PacketType getType() const ;
        std::uint16_t getBodySize() const ;
        virtual bool expectedType() const = 0;
        static std::uint8_t getHeaderSize();
        virtual std::uint16_t getPacketSize() const ;
};



} // namespace como

#endif // PACKAGE_HPP
