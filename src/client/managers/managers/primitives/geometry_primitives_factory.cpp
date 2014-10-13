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

#include "geometry_primitives_factory.hpp"

namespace como {


/***
 * 1. Construction
 ***/

GeometricPrimitivesFactory::GeometricPrimitivesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager ) :
    ServerWriter( server ),
    cubesFactory_( server, meshesManager, materialsManager, textureWallsManager ),
    conesFactory_( server, meshesManager, materialsManager, textureWallsManager ),
    cylindersFactory_( server, meshesManager, materialsManager, textureWallsManager ),
    spheresFactory_( server, meshesManager, materialsManager, textureWallsManager )
{
    SystemMesh::setTextureWallsManager( *textureWallsManager );
}


/***
 * 3. Geometry primitives creation
 ***/

ResourceID GeometricPrimitivesFactory::createCube( float width, float height, float depth )
{
    lock();
    return cubesFactory_.createCube( width, height, depth );
}


ResourceID GeometricPrimitivesFactory::createCone( float height, float radius, std::uint16_t nBaseVertices )
{
    lock();
    return conesFactory_.createCone( height, radius, nBaseVertices );
}


ResourceID GeometricPrimitivesFactory::createCylinder( float height, float radius, std::uint16_t nRadialVertices )
{
    lock();
    return cylindersFactory_.createCylinder( height, radius, nRadialVertices );
}


ResourceID GeometricPrimitivesFactory::createSphere( float radius, std::uint16_t nDivisions )
{
    lock();
    return spheresFactory_.createSphere( radius, nDivisions );
}


/***
 * 4. Command execution
 ***/

void GeometricPrimitivesFactory::executeRemoteCommand( const GeometricPrimitiveCommand& command )
{
    lock();
    switch( command.getType() ){
        case GeometricPrimitiveCommandType::CUBE_CREATION:{
            const CubeCreationCommand& creationCommand =
                dynamic_cast< const CubeCreationCommand& >( command );

            cubesFactory_.executeRemoteCommand( creationCommand );
        }break;
        case GeometricPrimitiveCommandType::CONE_CREATION:{
            const ConeCreationCommand& creationCommand =
                dynamic_cast< const ConeCreationCommand& >( command );

            conesFactory_.executeRemoteCommand( creationCommand );
        }break;
        case GeometricPrimitiveCommandType::CYLINDER_CREATION:{
            const CylinderCreationCommand& creationCommand =
                dynamic_cast< const CylinderCreationCommand& >( command );

            cylindersFactory_.executeRemoteCommand( creationCommand );
        }break;
        case GeometricPrimitiveCommandType::SPHERE_CREATION:{
            const SphereCreationCommand& creationCommand =
                dynamic_cast< const SphereCreationCommand& >( command );

            spheresFactory_.executeRemoteCommand( creationCommand );
        }break;
    }
}


} // namespace como
