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

#ifndef CUBE_CREATION_COMMAND_HPP
#define CUBE_CREATION_COMMAND_HPP

#include "drawable_command.hpp"

namespace como {

class CubeCreationCommand : public DrawableCommand
{
    private:
        // Mesh color.
        PackableRGBA color_;

    public:
        /***
         * 1. Construction
         ***/
        CubeCreationCommand();
        CubeCreationCommand( UserID userID, PackableDrawableID drawableID, const std::uint8_t* color );
        CubeCreationCommand( const CubeCreationCommand& b );
        CubeCreationCommand( CubeCreationCommand&& ) = delete;

        /***
         * 2. Destruction
         ***/
        ~CubeCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        const std::uint8_t* getColor() const ;


        /***
         * 4. Setters
         ***/
        void setColor( const std::uint8_t* color );


        /***
         * 5. Operators
         ***/
        CubeCreationCommand& operator=( const CubeCreationCommand& ) = delete;
        CubeCreationCommand& operator=( CubeCreationCommand&& ) = delete;
};

} // namespace como

#endif // CUBE_CREATION_COMMAND_HPP
