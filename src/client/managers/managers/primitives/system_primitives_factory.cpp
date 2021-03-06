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

#include "system_primitives_factory.hpp"

namespace como {


/***
 * 1. Construction
 ***/

SystemPrimitivesFactory::SystemPrimitivesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager ) :
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

ResourceID SystemPrimitivesFactory::createCube( float width, float height, float depth )
{
    LOCK
    return cubesFactory_.createCube( width, height, depth );
}


ResourceID SystemPrimitivesFactory::createCone( float height, float radius, std::uint16_t nBaseVertices )
{
    LOCK
    return conesFactory_.createCone( height, radius, nBaseVertices );
}


ResourceID SystemPrimitivesFactory::createCylinder( float height, float radius, std::uint16_t nRadialVertices )
{
    LOCK
    return cylindersFactory_.createCylinder( height, radius, nRadialVertices );
}


ResourceID SystemPrimitivesFactory::createSphere( float radius, std::uint16_t nDivisions )
{
    LOCK
    return spheresFactory_.createSphere( radius, nDivisions );
}


/***
 * 4. Command execution
 ***/

void SystemPrimitivesFactory::executeRemoteCommand( const SystemPrimitiveCommand& command )
{
    LOCK
    switch( command.getType() ){
        case SystemPrimitiveCommandType::CUBE_CREATION:{
            const CubeCreationCommand& creationCommand =
                dynamic_cast< const CubeCreationCommand& >( command );

            cubesFactory_.executeRemoteCommand( creationCommand );
        }break;
        case SystemPrimitiveCommandType::CONE_CREATION:{
            const ConeCreationCommand& creationCommand =
                dynamic_cast< const ConeCreationCommand& >( command );

            conesFactory_.executeRemoteCommand( creationCommand );
        }break;
        case SystemPrimitiveCommandType::CYLINDER_CREATION:{
            const CylinderCreationCommand& creationCommand =
                dynamic_cast< const CylinderCreationCommand& >( command );

            cylindersFactory_.executeRemoteCommand( creationCommand );
        }break;
        case SystemPrimitiveCommandType::SPHERE_CREATION:{
            const SphereCreationCommand& creationCommand =
                dynamic_cast< const SphereCreationCommand& >( command );

            spheresFactory_.executeRemoteCommand( creationCommand );
        }break;
    }
}


} // namespace como
