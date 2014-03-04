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

#ifndef PACKABLE_VECTOR4_HPP
#define PACKABLE_VECTOR4_HPP

#include "packable.hpp"

namespace como {

const unsigned int PACK_STRING_SIZE = 32;

class PackableVector4 : public Packable
{
    private:
        float values_[4];
        char packString_[PACK_STRING_SIZE];

    public:
        /***
         * 1. Construction
         ***/
        PackableVector4();
        PackableVector4( float x, float y, float z, float w );
        PackableVector4( const float* values );
        PackableVector4( const PackableVector4& b );
        PackableVector4( PackableVector4&& b );


        /***
         * 2. Destruction
         ***/
        ~PackableVector4() = default;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const;
        const float* getValue() const ;
        const float& operator []( unsigned int index ) const;


        /***
         * 4. Setters
         ***/
        void setValue( float x, float y, float z, float w );


        /***
         * 5. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const ;
        virtual const void* unpack( const void* buffer ) ;


        /***
         * 6. Operators
         ***/
        PackableVector4& operator = ( const PackableVector4& b );
        PackableVector4& operator = ( const float* values );
        PackableVector4& operator = ( PackableVector4&& ) = delete;
};

} // namespace como

#endif // PACKABLE_VECTOR4_HPP
