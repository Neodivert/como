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
         * latter is defined in every inherited class).
         */
        virtual bool expectedType() const = 0;

        /*! \brief Returns a pointer to the buffer used for packing /
         * unpacking this Packet */
        const char* getBuffer() const { return buffer_; }


        /***
         * 4. Synchronous communication.
         ***/

        /*!
         * \brief Synchronously sends this packet.
         * \param socket socket this packet will be sent through.
         */
        void send( boost::asio::ip::tcp::socket& socket );

        /*!
         * \brief Synchronously receives this packet.
         * \param socket socket this packet will be received through.
         */
        void recv( boost::asio::ip::tcp::socket& socket );


        /***
         * 5. Asynchronous shipment.
         ***/
    public:
        /*!
         * \brief Asynchronously sends this packet.
         * \param socket socket this packet will be sent through.
         * \param packetHandler handler to be invoked once the packet has been
         * sent. Such handler must accept the sent packet as an argument.
         */
        void asyncSend( SocketPtr socket, PacketHandler packetHandler );
    private:

        /*!
         * \brief Asynchronously sends this packet's body.
         * \param headerErrorCode error code indicating the transfer result of
         * this packet's header.
         * \param socket socket this packet's body will be sent through.
         * \param packetHandler handler to be invocked once the packet body
         * has been sent. Such handler must accept the sent packet as an
         * argument.
         */
        void asyncSendBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket, PacketHandler packetHandler );

        /*!
         * \brief Auxiliar method invoked once the current packet has been sent
         * through the network. It simply calls the packet handler given as an
         * argument.
         * \param errorCode error code indicating the transfer result of the
         * sent packet.
         * \param packetHandler handler to be invoked once the packet has been
         * sent. Such handler must accept the sent packet as an argument.
         */
        void onPacketSend( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler ); // TODO: Typo in "onPacketSenD".

        /***
         * 6. Asynchronous reception.
         ***/
    public:
        /*!
         * \brief Asynchronously receives this packet.
         * \param socket socket this packet will be received through.
         * \param packetHandler handler to be invoked once the packet has been
         * received. Such handler must accept the received packet as an
         * argument.
         */
        void asyncRecv( SocketPtr socket, PacketHandler packetHandler );
    private:

        /*!
         * \brief Asynchronously receives this packet's body.
         * \param headerErrorCode error code indicating the transfer result of
         * this packet's header.
         * \param socket socket this packet's body will be received through.
         * \param packetHandler handler to be invocked once the packet body
         * has been received. Such handler must accept the received packet as
         * an argument.
         */
        void asyncRecvBody( const boost::system::error_code& headerErrorCode, std::size_t, SocketPtr socket, PacketHandler packetHandler );

        /*!
         * \brief Axuliar method invoked once the current packet has been
         * received through the network. It simply calls the packet handler
         * given as an argument.
         * \param errorCode error code indicating the transfer result of the
         * received packet.
         * \param packetHandler handler to be invoked once the packet has been
         * received. Such handler must accept the received packet as an
         * argument.
         */
        void onPacketRecv( const boost::system::error_code& errorCode, std::size_t, PacketHandler packetHandler );

        /***
         * 7. Auxiliar networking methods.
         ***/

        /*!
         * \brief Updates the header of this packet by updating its "packet
         * size" field. Invoked when sending a packet through the network.
         */
        void updateHeader();
    public:

        /***
         * 8. Packing.
         ***/

        /*!
         * \brief Packs this packet into the given buffer.
         * See Packable::pack const.
         */
        virtual void* pack( void* buffer ) const;

        /*!
         * \brief Packs this packet's header into the given buffer. See
         * Packable::pack const.
         */
        virtual void* packHeader( void* buffer ) const;

        /*!
         * \brief Packs this packet's body into the given buffer. See
         * Packable::pack const.
         */
        virtual void* packBody( void* buffer ) const;


        /***
         * 9. Unpacking.
         ***/

        /*!
         * \brief Unpacks this packet from the given buffer.
         * See Packable::unpack.
         */
        virtual const void* unpack( const void* buffer );

        /*!
         * \brief Unpacks this packet's header from the given buffer. See
         * Packable::unpack.
         */
        virtual const void* unpackHeader( const void* buffer );

        /*!
         * \brief Unpacks this packet's body from the given buffer. See
         * Packable::unpack.
         */
        virtual const void* unpackBody( const void* buffer );


        /***
         * 10. Test unpacking.
         ***/

        /*!
         * \brief Unpacks a packet from the given buffer and throw an exception
         * if the unpacked data doesn't match this packet's data. See
         * Packable::unpack const.
         */
        virtual const void* unpack( const void* buffer ) const ;

        /*!
         * \brief Unpacks a packet's header from the given buffer and throw an
         * exception if the unpacked data doesn't match this packet's header
         * data. See Packable::unpack const.
         */
        virtual const void* unpackHeader( const void* buffer ) const ;

        /*!
         * \brief Unpacks a packet's body from the given buffer and throw an
         * exception if the unpacked data doesn't match this packet's body
         * data. See Packable::unpack const.
         */
        virtual const void* unpackBody( const void* buffer ) const ;


        /***
         * 11. Operators.
         ***/

        /*! \brief Copy assignment operator */
        Packet& operator = ( const Packet& b );

        /*! \brief Move assignment operator */
        Packet& operator = ( Packet&& ) = delete;
};

} // namespace como

#endif // PACKET_HPP
