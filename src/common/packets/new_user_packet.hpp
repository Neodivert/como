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

#ifndef NEW_USER_PACKET_HPP
#define NEW_USER_PACKET_HPP

#include <common/packables/packable_string.hpp>
#include "packet.hpp"

namespace como {

/*!
 * \class NewUserPacket
 *
 * \brief Packet sent from user to server when the former tries to connect to
 * the latter.
 */
class NewUserPacket : public Packet
{  
    public:
        /*! Name of the user trying to connect to server */
        PackableString<NAME_SIZE> name_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor. */
        NewUserPacket();

        /*! \brief Constructs a NewUserPacket with the given user name. */
        NewUserPacket( const char* name );

        /*! \brief Copy constructor. */
        NewUserPacket( const NewUserPacket& b );

        /*! \brief Move constructor. */
        NewUserPacket( NewUserPacket&& ) = delete;

        /*! \brief See Packet::clone const. */
        virtual Packet* clone() const ; // TODO: Memory leack?


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor. */
        ~NewUserPacket() = default;


        /***
         * 3. Getters
         ***/

        // TODO: Move to another "section" (no "Getters")?
        /*! \brief See Packet::expectedType const */
        virtual bool expectedType() const;

        /*!
         * \brief Returns the user name field inside this packet.
         * \return a string with the user name written in this packet.
         */
        const char* getName() const ;


        /***
         * 4. Setters
         ***/

        /*!
         * \brief Sets the user name field of this packet.
         * \param name new user name.
         */
        void setName( const char* name );


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        NewUserPacket& operator = ( const NewUserPacket& b );

        /*! \brief Move assignment operator */
        NewUserPacket& operator = ( NewUserPacket&& ) = delete;
};

} // namespace como

#endif // NEW_USER_PACKET_HPP
