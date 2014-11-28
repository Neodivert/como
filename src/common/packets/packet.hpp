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

#ifndef PACKET_HPP
#define PACKET_HPP

#include <boost/asio.hpp>
#include <common/packables/composite_packable.hpp>
#include <stdexcept>
#include <functional>
#include <boost/bind.hpp>
#include "packet_header.hpp"
#include <common/utilities/log.hpp>

namespace como {

class Packet;

// Convenient typedefs.
typedef boost::asio::ip::tcp::socket Socket;
typedef std::shared_ptr< Packet > PacketPtr;
typedef std::function<void( const boost::system::error_code& errorCode, PacketPtr)> PacketHandler;

// Maximum buffer available size for packing a packet.
const PacketSize PACKET_HEADER_BUFFER_SIZE = 20; // TODO: Use a real value.


/*!
 * \class Packet
 *
 * \brief Base class for all type of Packets sent between client and server.
 */
class Packet : public CompositePackable
{
    public:
        /***
         * 1. Construction
         ***/
        Packet() = delete ;
        Packet( PacketType type );
        Packet( const Packet& b );
        Packet( Packet&& ) = delete;
        virtual Packet* clone() const = 0;


        /***
         * 2. Destruction
         ***/
        virtual ~Packet() = default;


        /***
         * 3. Getters
         ***/
        PacketType getType() const ;

        // Returns true if this Packet's type matches the expected one (the
        // latter is defined in every inherited class).
        virtual bool expectedType() const = 0;


        /***
         * 4. Synchronous communication
         ***/
        void send( boost::asio::ip::tcp::socket& socket );
        void recv( boost::asio::ip::tcp::socket& socket );


        /***
         * 5. Asynchronous communication
         ***/
        void asyncSend( Socket& socket, PacketHandler packetHandler );
        void asyncRecv( Socket& socket, PacketHandler packetHandler );


        /***
         * 6. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 7. Operators
         ***/
        Packet& operator = ( const Packet& b );
        Packet& operator = ( Packet&& ) = delete;


    private:
        /***
         * 8. Asynchronous communication (private)
         ***/
        void asyncSendBody( const boost::system::error_code& headerErrorCode, std::size_t, Socket& socket, PacketHandler packetHandler );
        void onPacketSend( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler ); // TODO: Typo in "onPacketSenD".
        void asyncRecvBody( const boost::system::error_code& headerErrorCode, std::size_t, Socket& socket, PacketHandler packetHandler );
        void onPacketRecv( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler );


        /***
         * 9. Packing and unpacking (private)
         ***/
        virtual void* packHeader( void* buffer ) const;
        virtual void* packBody( void* buffer ) const;
        virtual const void* unpackHeader( const void* buffer );
        virtual const void* unpackBody( const void* buffer );
        virtual const void* unpackHeader( const void* buffer ) const ;
        virtual const void* unpackBody( const void* buffer ) const ;


        /***
         * 10. Auxiliar networking methods.
         ***/

        /*!
         * \brief Updates the header of this packet by updating its "packet
         * size" field. Invoked when sending a packet through the network.
         */
        void updateHeader();


        /***
         * Attributes
         ***/
        /*! Packet header. */
        PacketHeader header_;

        /*! Buffer where this Packet is read from / written to. */
        mutable char headerBuffer_[PACKET_HEADER_BUFFER_SIZE];
        mutable std::vector< char > bodyBuffer_;
};

} // namespace como

#endif // PACKET_HPP
