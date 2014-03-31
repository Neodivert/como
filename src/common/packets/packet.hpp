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

// Convenient typedefs.
typedef boost::asio::ip::tcp::socket Socket;
typedef std::shared_ptr< Socket > SocketPtr;
typedef std::shared_ptr< Packet > PacketPtr;
typedef std::function<void( const boost::system::error_code& errorCode, PacketPtr)> PacketHandler;

// Maximum buffer available size for packing a packet.
const unsigned int PACKET_BUFFER_SIZE = 512;


/*!
 * \class Packet
 *
 * \brief Base class for all type of Packets sent between client and server.
 */
class Packet : public CompositePackable
{
    private:
        /*! Packet header. */
        PacketHeader header_;

        /*! Buffer where this Packet is read from / written to. */
        char buffer_[PACKET_BUFFER_SIZE];

    public:
        /***
         * 1. Construction.
         ***/

        /*! \brief Default constructor. */
        Packet() = delete ;

        /*! \brief Constructs a Packet of the given type. */
        Packet( PacketType type );

        /*! \brief Copy constructor. */
        Packet( const Packet& b );

        /*! \brief Default constructor. */
        Packet( Packet&& ) = delete;

        // TODO: Memory leak?
        /*!
         * \brief Clone method
         * \return a pointer to a new copy of this instance of Packet.
         */
        virtual Packet* clone() const = 0;


        /***
         * 2. Destruction.
         ***/
        ~Packet() = default;


        /***
         * 3. Getters.
         ***/

        /*! \brief Returns a PacketType indicating the type of this Packet */
        PacketType getType() const ;

        /*!
         * \brief Type checking method.
         * \return true if this Packet's type matches the expected one (the
         * latter defined in every inherited class).
         */
        virtual bool expectedType() const = 0;

        /*! \brief Returns a pointer to the buffer used for packing /
         * unpacking this Packet */
        const char* getBuffer() const { return buffer_; }


        /***
         * 4. Synchronous communication.
         ***/

        /*!
         * \brief synchronously sends this packet.
         * \param socket socket this packet will be sent through.
         */
        void send( boost::asio::ip::tcp::socket& socket );

        /*!
         * \brief synchronously received this packet.
         * \param socket socket this packet will be received through.
         */
        void recv( boost::asio::ip::tcp::socket& socket );


        /***
         * 5. Asynchronous shipment.
         ***/
    private:
        void asyncSend( SocketPtr socket, PacketHandler packetHandler );
        void asyncSendBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket, PacketHandler packetHandler );
        void onPacketSend( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler );

        /***
         * 6. Asynchronous reception.
         ***/
        void asyncRecv( SocketPtr socket, PacketHandler packetHandler );
        void asyncRecvBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket, PacketHandler packetHandler );
        void onPacketRecv( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler );

        // TODO: Classify.
        void updateHeader();
    public:


        /***
         * 5. Packing and unpacking.
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
         * 6. Operators.
         ***/
        Packet& operator = ( const Packet& b );
        Packet& operator = ( Packet&& ) = delete;
};

} // namespace como

#endif // PACKET_HPP
