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

GeometricPrimitivesFactory::GeometricPrimitivesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager ) :
    ServerWriter( server ),
    textureWallsManager_( new TexturesManager( server ) ),
    cubesFactory_( server, meshesManager, materialsManager, textureWallsManager_.get() )
{}


/***
 * 3. Geometry primitives creation
 ***/

ResourceID GeometricPrimitivesFactory::createCube( float width, float height, float depth )
{
    return cubesFactory_.createCube( width, height, depth );
}


/***
 * 4. Command execution
 ***/

void GeometricPrimitivesFactory::executeRemoteCommand( const GeometricPrimitiveCommand& command )
{
    switch( command.getType() ){
        case GeometricPrimitiveCommandType::CUBE_CREATION:{
            const CubeCreationCommand& creationCommand =
                dynamic_cast< const CubeCreationCommand& >( command );

            cubesFactory_.executeRemoteCommand( creationCommand );
        }break;
    }
}


} // namespace como
