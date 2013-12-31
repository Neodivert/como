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

#ifndef CREATE_CUBE_HPP
#define CREATE_CUBE_HPP

#include "drawable_command.hpp"

namespace como {

class CreateCube : public DrawableCommand
{
    private:
        // Mesh color.
        std::uint8_t color_[4];

    public:
        /***
         * 1. Initialization and destruction
         ***/
        CreateCube();
        CreateCube( UserID userID, DrawableID drawableID, const std::uint8_t* color );
        CreateCube( const CreateCube& b );

        virtual ~CreateCube(){}


        /***
         * 2. Packing and unpacking
         ***/
        virtual char* pack( char* buffer ) const ;
        virtual const char* unpack( const char* buffer ) ;


        /***
         * 3. Getters
         ***/
        virtual std::uint16_t getPacketSize() const ;
        const std::uint8_t* getColor() const ;


        /***
         * 4. Setters
         ***/
        void setColor( const std::uint8_t* color );
};

} // namespace como

#endif // CREATE_CUBE_HPP
