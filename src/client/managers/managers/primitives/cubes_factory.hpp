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

#ifndef CUBES_FACTORY_HPP
#define CUBES_FACTORY_HPP

#include <client/managers/managers/primitives/specialized_system_primitives_factory.hpp>
#include <client/models/3d/meshes/system_mesh.hpp>

namespace como {

class CubesFactory : public SpecializedSystemPrimitivesFactory
{
    public:
        /***
         * 1. Construction
         ***/
        CubesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager );
        CubesFactory() = delete;
        CubesFactory( const CubesFactory& ) = delete;
        CubesFactory( CubesFactory&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~CubesFactory() = default;


        /***
         * 3. Cubes creation
         ***/
        ResourceID createCube( float width, float height, float depth );


        /***
         * 4. Operators
         ***/
        CubesFactory& operator = ( const CubesFactory& ) = delete;
        CubesFactory& operator = ( CubesFactory&& ) = delete;


    protected:
        /***
         * 5. SystemPrimitiveData generation
         ***/
        virtual void generateVertexData( MeshVertexData &vertexData );
        virtual void generateUVData(MeshTextureData &uvData);


    private:
        float width_;
        float height_;
        float depth_;
};

} // namespace como

#endif // CUBES_FACTORY_HPP
