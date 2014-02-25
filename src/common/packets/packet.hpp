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

#include "../packables/composite_packable.hpp"
#include "../packables/packable_wrapper.hpp"
#include "../utilities/packer.hpp"
#include <stdexcept>
#include <boost/asio.hpp>
#include <functional>
#include <boost/bind.hpp>

namespace como {

class Packet;

typedef boost::asio::ip::tcp::socket Socket;
typedef std::shared_ptr< Socket > SocketPtr;
typedef std::shared_ptr< Packet > PacketPtr;
typedef std::function<void( const boost::system::error_code& errorCode, PacketPtr)> PacketHandler;

enum class PacketType : std::uint8_t
{
    NEW_USER = 0,
    USER_ACCEPTED = 1,
    SCENE_UPDATE = 2
};

const unsigned int PACKET_BUFFER_SIZE = 512;


class Packet : public CompositePackable
{
    private:
        PackableWrapper< std::uint8_t, PacketType > type_;
        PackableWrapper< std::uint16_t, std::uint16_t > bodySize_;

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
         * 2. Packing
         ***/
        virtual void* packHeader( void* buffer ) const;

        /***
         * 2. Socket communication
         ***/
        void send( boost::asio::ip::tcp::socket& socket );
        void asyncSend( SocketPtr socket, PacketHandler packetHandler );

        void recv( boost::asio::ip::tcp::socket& socket );
        void asyncRecv( SocketPtr socket, PacketHandler packetHandler );

    private:
        void asyncSendBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket, PacketHandler packetHandler );
        void asyncRecvBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket, PacketHandler packetHandler );
        void onPacketRecv( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler );
        void onPacketSend( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler );
    public:


        /***
         * 3. Getters
         ***/
        PacketType getType() const ;
        virtual bool expectedType() const = 0;


        /***
         * 4. Operators
         ***/
        Packet& operator = ( const Packet& b );
        Packet& operator = ( Packet&& ) = delete;
};

} // namespace como

#endif // PACKET_HPP
