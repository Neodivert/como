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

#ifndef GEOMETRY_PRIMITIVES_FACTORY_HPP
#define GEOMETRY_PRIMITIVES_FACTORY_HPP

#include <client/managers/utilities/server_writer.hpp>
#include <client/managers/managers/primitives/cubes_factory.hpp>
#include <client/managers/managers/textures/texture_walls_manager.hpp>

namespace como {

class GeometricPrimitivesFactory;
typedef std::shared_ptr< GeometricPrimitivesFactory > GeometricPrimitivesFactoryPtr;

class GeometricPrimitivesFactory : public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        GeometricPrimitivesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager );
        GeometricPrimitivesFactory() = delete;
        GeometricPrimitivesFactory( const GeometricPrimitivesFactory& ) = delete;
        GeometricPrimitivesFactory( GeometricPrimitivesFactory&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~GeometricPrimitivesFactory() = default;


        /***
         * 3. Local geometric primitives creation
         ***/
        ResourceID createCube( float width = 1.0f, float height = 1.0f, float depth = 1.0f );


        /***
         * 4. Command execution
         ***/
        void executeRemoteCommand( const GeometricPrimitiveCommand& command );


        /***
         * 5. Operators
         ***/
        GeometricPrimitivesFactory& operator = ( const GeometricPrimitivesFactory& ) = delete;
        GeometricPrimitivesFactory& operator = ( GeometricPrimitivesFactory&& ) = delete;


    private:
        CubesFactory cubesFactory_;


        TextureWallsManagerPtr textureWallsManager_;
};


} // namespace como

#endif // GEOMETRY_PRIMITIVES_FACTORY_HPP
