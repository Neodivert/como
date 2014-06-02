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

#ifndef PRIMITIVE_MESH_CREATION_COMMAND_HPP
#define PRIMITIVE_MESH_CREATION_COMMAND_HPP

#include <common/commands/drawable_commands/mesh_creation_command.hpp>

namespace como {

class PrimitiveMeshCreationCommand : public MeshCreationCommand
{
    private:
        // ID of the primitive we are creating this mesh from.
        PackablePrimitiveID primitiveID_;

    public:
        /***
         * 1. Construction
         ***/
        PrimitiveMeshCreationCommand();
        PrimitiveMeshCreationCommand( PackableDrawableID drawableID, PrimitiveID primitiveID, const MaterialID& materialID );
        PrimitiveMeshCreationCommand( const PrimitiveMeshCreationCommand& b );
        PrimitiveMeshCreationCommand( PrimitiveMeshCreationCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~PrimitiveMeshCreationCommand() = default;


        /***
         * 3. Getters
         ***/
        PrimitiveID getPrimitiveID() const;


        /***
         * 4. Operators
         ***/
        PrimitiveMeshCreationCommand& operator=( const PrimitiveMeshCreationCommand& ) = delete;
        PrimitiveMeshCreationCommand& operator=( PrimitiveMeshCreationCommand&& ) = delete;
};

} // namespace como

#endif // PRIMITIVE_MESH_CREATION_COMMAND_HPP
