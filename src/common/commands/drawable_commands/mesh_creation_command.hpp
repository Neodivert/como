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

#ifndef MESH_CREATION_COMMAND_HPP
#define MESH_CREATION_COMMAND_HPP

#include "drawable_command.hpp"
#include <common/commands/primitive_commands/primitive_command.hpp> // PackablePrimitiveID type.
#include <common/utilities/ids.hpp>

namespace como {

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

        // Mesh material ID.
        PackableMaterialID materialID_;

    public:
        /***
         * 1. Construction
         ***/
        MeshCreationCommand() = delete;
        MeshCreationCommand( MeshType meshType, PackableDrawableID drawableID, const MaterialID& materialID );
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
        MaterialID getMaterialID() const ;


        /***
         * 4. Setters
         ***/
        void setMaterialID( const MaterialID& id );


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
