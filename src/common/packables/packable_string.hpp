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
#include <cstring>

namespace como {

/*!
 * \class PackableString
 *
 * \brief String that can be packed into / unpacked from a given buffer.
 * \tparam STRING_SIZE string size (in characters).
 */
template <unsigned int STRING_SIZE>
class PackableString : public Packable
{
    private:
        /*! Plain inner string */
        char str_[STRING_SIZE];

    public:
        /***
         * 1. Construction
         ***/

        /*! Default constructor */
        PackableString() = default;

        /*! Constructs a packable string by copying a plain one. */
        PackableString( const char* str );

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
        const char* getValue() const { return str_; }

        /*! \brief see Packable::getPacketSize const */
        virtual PacketSize getPacketSize() const { return STRING_SIZE; }

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
        void setValue( const char* str ){ strncpy( str_, str, STRING_SIZE ); }


        /***
         * 6. Operators
         ***/

        /*! Assigns the given plain string to this instance's inner one */
        PackableString& operator = ( const char* str );

        /*! Copy assignment operator */
        PackableString& operator = (const PackableString& b);

        /*! Move assignment operator */
        PackableString& operator = ( PackableString&& ) = default;
};


/***
 * 1. Construction
 ***/

template <unsigned int STRING_SIZE>
PackableString<STRING_SIZE>::PackableString( const PackableString<STRING_SIZE>& b ) :
    Packable()
{
    strncpy( str_, b.str_, STRING_SIZE );
}

template <unsigned int STRING_SIZE>
PackableString<STRING_SIZE>::PackableString( const char* str ) :
    Packable()
{
    strncpy( str_, str, STRING_SIZE );
}


/***
 * 3. Packing and unpacking
 ***/

template <unsigned int STRING_SIZE>
void* PackableString<STRING_SIZE>::pack( void* buffer ) const
{
    // Cast the buffer so we can pack this string.
    char* castedBuffer = static_cast< char* >( buffer );

    // Pack the string into the buffer.
    strncpy( castedBuffer, str_, STRING_SIZE );

    // Return a pointer to the next position in the buffer.
    return static_cast< void *>( castedBuffer + STRING_SIZE );
}


template <unsigned int STRING_SIZE>
const void* PackableString<STRING_SIZE>::unpack( const void* buffer )
{
    // Cast the buffer so we can unpack the string.
    const char* castedBuffer = static_cast< const char* >( buffer );

    // Unpack the string from the buffer.
    strncpy( str_, static_cast< const char* >( buffer ), STRING_SIZE );

    // Return a pointer to the next position in the buffer.
    return static_cast< const void *>( castedBuffer + STRING_SIZE );
}


template <unsigned int STRING_SIZE>
const void* PackableString<STRING_SIZE>::unpack( const void* buffer ) const
{
    char unpackedStr[STRING_SIZE] = { 0 };

    // Cast the buffer so we can unpack the string.
    const char* castedBuffer = static_cast< const char* >( buffer );

    // Unpack a string from the given buffer.
    strncpy( unpackedStr, static_cast< const char* >( buffer ), STRING_SIZE );

    // Throw an exception if the unpacked string doesn't match str_.
    if( strncmp( str_, unpackedStr, STRING_SIZE ) ){
        throw std::runtime_error( "ERROR: Unpacked an unexpected PackableString" );
    }

    // Return a pointer to the next position in the buffer.
    return static_cast< const void *>( castedBuffer + STRING_SIZE );
}


/***
 * 6. Operators
 ***/

template <unsigned int STRING_SIZE>
PackableString<STRING_SIZE>& PackableString<STRING_SIZE>::operator = ( const PackableString<STRING_SIZE>& b )
{
    if( this != &b ){
        strncpy( str_, b.str_, STRING_SIZE );
    }
    return *this;
}

template <unsigned int STRING_SIZE>
PackableString<STRING_SIZE>& PackableString<STRING_SIZE>::operator = ( const char* str )
{
    strncpy( str_, str, STRING_SIZE );

    return *this;
}


} // namespace como

#endif // PACKABLE_STRING_HPP
