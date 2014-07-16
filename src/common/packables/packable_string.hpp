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

#ifndef PACKABLE_STRING_HPP
#define PACKABLE_STRING_HPP

#include "packable.hpp"
#include <string>

namespace como {

/*!
 * \class PackableString
 *
 * \brief String that can be packed into / unpacked from a given buffer.
 */
class PackableString : public Packable
{
    private:
        /*! Plain inner string */
        std::string str_;

    public:
        /***
         * 1. Construction
         ***/

        /*! Default constructor */
        PackableString() = default;

        /*! Constructs a packable string by copying a plain one. */
        PackableString( const char* str );

        PackableString( const std::string& str );

        /*! Copy constructor */
        PackableString( const PackableString& b );

        /*! Move constructor */
        PackableString( PackableString&& ) = default;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        virtual ~PackableString(){}


        /***
         * 3. Packing and unpacking
         ***/

        /*! \brief see Packable::pack */
        virtual void* pack( void* buffer ) const ;

        /*! \brief see Packable::unpack */
        virtual const void* unpack( const void* buffer ) ;

        /*! \brief see Packable::unpack const */
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 4. Getters
         ***/

        /*!
         * \brief returns a pointer to inner plain string.
         * \return a pointer to inner plain string.
         */
        const char* getValue() const { return str_.c_str(); }

        /*! \brief see Packable::getPacketSize const */
        virtual PacketSize getPacketSize() const { return sizeof( std::uint16_t ) + str_.size(); }

        /*! \brief see PackableArrayWrapper::operator[] */
        char& operator []( unsigned int index ){ return str_[index]; }

        /*! \brief see PackableArrayWrapper::operator[] const */
        const char& operator []( unsigned int index ) const { return str_[index]; }

        /***
         * 5. Setters
         ***/

        /*!
         * \brief assign the given plain string to this instance's inner
         * string.
         * \param str plain string we are assigning.
         */
        void setValue( const char* str ){ str_ = str; }


        /***
         * 6. Operators
         ***/

        /*! Assigns the given plain string to this instance's inner one */
        PackableString& operator = ( const char* str );

        PackableString& operator = ( const std::string& str );

        /*! Copy assignment operator */
        PackableString& operator = (const PackableString& b);

        /*! Move assignment operator */
        PackableString& operator = ( PackableString&& ) = default;
};

} // namespace como

#endif // PACKABLE_STRING_HPP
