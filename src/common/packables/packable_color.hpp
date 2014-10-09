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

#ifndef PACKABLE_COLOR_HPP
#define PACKABLE_COLOR_HPP

#include <common/packables/abstract_packable_wrapper.hpp>
#include <common/utilities/color.hpp>

namespace como {

class PackableColor : public AbstractPackableWrapper< Color >
{
    public:
        /***
         * 1. Construction
         ***/
        PackableColor();
        PackableColor( const Color& color );
        PackableColor( const PackableColor& ) = default;
        PackableColor( PackableColor&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~PackableColor() = default;


        /***
         * 3. Getters
         ***/
        virtual Color getValue() const;
        virtual PacketSize getPacketSize() const;


        /***
         * 4. Setters
         ***/
        virtual void setValue( const Color& color );


        /***
         * 5. Packing / unpacking
         ***/
        virtual void *pack(void *buffer) const;
        virtual const void* unpack( const void* buffer );
        virtual const void* unpack( const void* buffer ) const;


        /***
         * 6. Operators
         ***/
        PackableColor& operator = ( const PackableColor& ) = default;
        PackableColor& operator = ( PackableColor&& ) = default;

    private:
        Color plainColor_;
};

} // namespace como

#endif // PACKABLE_COLOR_HPP
