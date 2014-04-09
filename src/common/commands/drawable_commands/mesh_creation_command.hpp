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

#ifndef MESH_CREATION_COMMAND_HPP
#define MESH_CREATION_COMMAND_HPP

#include "drawable_command.hpp"
#include "../primitive_commands/primitive_command.hpp" // PackablePrimitiveID type.

namespace como {

class MeshCreationCommand : public DrawableCommand
{
    private:
        // ID of the primitive we are creating this mesh from.
        PackablePrimitiveID primitiveID_;

        // Mesh color.
        PackableRGBA color_;

    public:
        /***
         * 1. Construction
         ***/
        MeshCreationCommand();
        MeshCreationCommand( UserID userID, PackableDrawableID drawableID, PrimitiveID primitiveID, const std::uint8_t* color );
        MeshCreationCommand( const MeshCreationCommand& b );
        MeshCreationCommand( MeshCreationCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MeshCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        PrimitiveID getPrimitiveID() const;
        const std::uint8_t* getColor() const ;


        /***
         * 4. Setters
         ***/
        void setColor( const std::uint8_t* color );


        /***
         * 5. Operators
         ***/
        MeshCreationCommand& operator=( const MeshCreationCommand& ) = delete;
        MeshCreationCommand& operator=( MeshCreationCommand&& ) = delete;
};

} // namespace como

#endif // CUBE_CREATION_COMMAND_HPP
