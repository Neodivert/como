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

#ifndef SPECIALIZED_SYSTEM_PRIMITIVES_FACTORY_HPP
#define SPECIALIZED_SYSTEM_PRIMITIVES_FACTORY_HPP

#include <client/managers/managers/meshes/meshes_manager.hpp>
#include <common/primitives/primitive_data/system_primitive_data.hpp>
#include <client/managers/managers/textures/texture_walls_manager.hpp>

namespace como {

class SpecializedSystemPrimitivesFactory : public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        SpecializedSystemPrimitivesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager );
        SpecializedSystemPrimitivesFactory() = delete;
        SpecializedSystemPrimitivesFactory( const SpecializedSystemPrimitivesFactory& ) = delete;
        SpecializedSystemPrimitivesFactory( SpecializedSystemPrimitivesFactory&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SpecializedSystemPrimitivesFactory() = default;


        /***
         * 3. Operators
         ***/
        SpecializedSystemPrimitivesFactory& operator = ( const SpecializedSystemPrimitivesFactory& ) = delete;
        SpecializedSystemPrimitivesFactory& operator = ( SpecializedSystemPrimitivesFactory&& ) = delete;


    protected:
        /***
         * 4. SystemPrimitiveData generation
         ***/
        virtual SystemPrimitiveData generatePrimitiveData();
        virtual void generateVertexData( MeshVertexData& vertexData ) = 0;
        virtual void generateNormalData( const MeshVertexData& vertexData, MeshNormalData& normalData );
        virtual void generateUVData( MeshTextureData& uvData ) = 0;
        virtual void generateTrianglesGroups( std::vector< TrianglesGroupWithTextureWall >& trianglesGroups ) = 0;
        virtual void generateOGLData( SystemPrimitiveData& primitiveData );


        MeshesManagerPtr meshesManager_;
        TextureWallsManager* textureWallsManager_;
        MaterialsManagerPtr materialsManager_;
};

} // namespace como

#endif // SPECIALIZED_SYSTEM_PRIMITIVES_FACTORY_HPP
