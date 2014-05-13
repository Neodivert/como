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
#include <common/commands/primitive_commands/primitive_command.hpp> // PackablePrimitiveID type.
#include <common/utilities/ids.hpp>

namespace como {

const std::uint8_t DEFAULT_MESH_COLOR[] = { 0, 0, 0, 0 };


enum class MeshType : std::uint8_t
{
    PRIMITIVE_MESH = 0,
    CAMERA,
    LIGHT
};


class MeshCreationCommand : public DrawableCommand
{
    private:
        // Mesh type.
        PackableUint8< MeshType > meshType_;

        // Mesh color.
        PackableRGBA meshColor_;

    public:
        /***
         * 1. Construction
         ***/
        MeshCreationCommand() = delete;
        MeshCreationCommand( MeshType meshType, PackableDrawableID drawableID, const std::uint8_t* color = DEFAULT_MESH_COLOR );
        MeshCreationCommand( const MeshCreationCommand& b );
        MeshCreationCommand( MeshCreationCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MeshCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        MeshType getMeshType() const;
        const std::uint8_t* getMeshColor() const ;


        /***
         * 4. Setters
         ***/
        void setMeshColor( const std::uint8_t* color );


        /***
         * 5. Buffer pre-reading
         ***/
        static MeshType getMeshType( const void* buffer );


        /***
         * 6. Operators
         ***/
        MeshCreationCommand& operator=( const MeshCreationCommand& ) = delete;
        MeshCreationCommand& operator=( MeshCreationCommand&& ) = delete;
};

} // namespace como

#endif // CUBE_CREATION_COMMAND_HPP
