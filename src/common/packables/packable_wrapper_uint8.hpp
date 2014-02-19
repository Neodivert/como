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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef PACKABLE_WRAPPER_UINT8_HPP
#define PACKABLE_WRAPPER_UINT8_HPP

#include "packable_wrapper.hpp"

namespace como {

class PackableWrapperUint8 : public PackableWrapper< std::uint8_t >
{
    public:
        /***
         * 1. Construction
         ***/
        PackableWrapperUint8( std::uint8_t* value );
        PackableWrapperUint8( const PackableWrapperUint8& ) = default;
        PackableWrapperUint8( PackableWrapperUint8&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PackableWrapperUint8() = default;


        /***
         * 3. Packing and unpacking
         ***/
        virtual void* pack( void* buffer ) const = 0;
        virtual const void* unpack( const void* buffer ) = 0;


        /***
         * 4. Getters
         ***/
        virtual std::uint16_t getPacketSize() const = 0;


        /***
         * 5. Operators
         ***/
        PackableWrapperUint8& operator = (const PackableWrapperUint8& b) = delete;
        PackableWrapperUint8& operator = ( PackableWrapperUint8&& ) = delete;
};

} // namespace como

#endif // PACKABLE_WRAPPER_UINT8_HPP
