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

#ifndef SYSTEM_PRIMITIVES_FACTORY_HPP
#define SYSTEM_PRIMITIVES_FACTORY_HPP

#include <client/managers/utilities/server_writer.hpp>
#include <client/managers/managers/primitives/cubes_factory.hpp>
#include <client/managers/managers/primitives/cones_factory.hpp>
#include <client/managers/managers/primitives/cylinders_factory.hpp>
#include <client/managers/managers/primitives/spheres_factory.hpp>
#include <client/managers/managers/textures/textures_manager.hpp>

namespace como {

class SystemPrimitivesFactory;
typedef std::shared_ptr< SystemPrimitivesFactory > GeometricPrimitivesFactoryPtr;

class SystemPrimitivesFactory : public ServerWriter, public Lockable
{
    public:
        /***
         * 1. Construction
         ***/
        SystemPrimitivesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager );
        SystemPrimitivesFactory() = delete;
        SystemPrimitivesFactory( const SystemPrimitivesFactory& ) = delete;
        SystemPrimitivesFactory( SystemPrimitivesFactory&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SystemPrimitivesFactory() = default;


        /***
         * 3. Local geometric primitives creation
         ***/
        ResourceID createCube( float width = 1.0f, float height = 1.0f, float depth = 1.0f );
        ResourceID createCone( float height = 1.0f, float radius = 0.5f, std::uint16_t nBaseVertices = 32 );
        ResourceID createCylinder( float height = 1.0f, float radius = 0.5f, std::uint16_t nRadialVertices = 32 );
        ResourceID createSphere( float radius = 0.5f, std::uint16_t nDivisions = 32 );


        /***
         * 4. Command execution
         ***/
        void executeRemoteCommand( const GeometricPrimitiveCommand& command );


        /***
         * 5. Operators
         ***/
        SystemPrimitivesFactory& operator = ( const SystemPrimitivesFactory& ) = delete;
        SystemPrimitivesFactory& operator = ( SystemPrimitivesFactory&& ) = delete;


    private:
        CubesFactory cubesFactory_;
        ConesFactory conesFactory_;
        CylindersFactory cylindersFactory_;
        SpheresFactory spheresFactory_;
};


} // namespace como

#endif // SYSTEM_PRIMITIVES_FACTORY_HPP
