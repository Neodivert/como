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

#ifndef PACKET_HEADER_HPP
#define PACKET_HEADER_HPP

#include <common/packables/composite_packable.hpp>
#include <common/packables/packable_integer.hpp>

namespace como {

/*!
 * \enum PacketType
 *
 * \brief List of possibles types of packets sent between client and server.
 */
enum class PacketType : std::uint8_t
{
    NEW_USER = 0,
    USER_ACCEPTED = 1,
    SCENE_UPDATE = 2
};


typedef PackableUint32< PacketSize > PackablePacketSize;

/*!
 * \class PacketHeader
 *
 * \brief Metadata added to every Packet for indicating its type and size.
 */
class PacketHeader : public CompositePackable
{
    private:
        /*! Type of the packet */
        PackableUint8< PacketType > type_;

        /*! Size of the packet's body */
        PackablePacketSize bodySize_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Constructs a PacketHeader with the given type. */
        PacketHeader( PacketType type );

        /*! \brief Copy constructor. */
        PacketHeader( const PacketHeader& );

        /*! \brief Move constructor. */
        PacketHeader( PacketHeader&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        virtual ~PacketHeader(){}


        /***
         * 3. Getters
         ***/

        /*!
         * \brief Returns the type of the packet associated with this
         * PacketHeader
         */
        PacketType getType() const;

        /*!
         * \brief Returns the size of the packet body associated with
         * this PacketHeader
         */
        PacketSize getBodySize() const;


        /***
         * 4. Setters
         ***/

        /*! \brief Set the value of the body size attribute */
        void setBodySize( PacketSize bodySize );


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        CompositePackable& operator = (const PacketHeader& b);

        /*! \brief Move assignment operator */
        CompositePackable& operator = ( PacketHeader&& );
};

} // namespace como

#endif // PACKET_HEADER_HPP
