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

#include "cubes_factory.hpp"

namespace como {


/***
 * 1. Construction
 ***/

CubesFactory::CubesFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager* textureWallsManager ) :
    SpecializedSystemPrimitivesFactory( server, "Cube", meshesManager, materialsManager, textureWallsManager )
{}


/***
 * 3. Local cubes creation
 ***/

ResourceID CubesFactory::createCube( float width, float height, float depth )
{
    ResourceID cubeID = reserveResourceIDs( 1 );
    ResourceID materialID = reserveResourceIDs( 1 );
    ResourceID firstTextureWallID = reserveResourceIDs( 6 );

    const glm::vec3 cubeCentroid =
        createCube( cubeID, materialID, firstTextureWallID, width, height, depth );

    sendCommandToServer(
                CommandConstPtr(
                    new CubeCreationCommand( cubeID,
                                             materialID,
                                             firstTextureWallID,
                                             width,
                                             height,
                                             depth,
                                             cubeCentroid ) ) );

    return cubeID;
}


/***
 * 4. Command execution
 ***/

void CubesFactory::executeRemoteCommand( const CubeCreationCommand &command )
{
    createCube( command.getMeshID(),
                command.getMaterialID(),
                command.getFirstTextureWallID(),
                command.getCubeWidth(),
                command.getCubeHeight(),
                command.getCubeDepth() );
}


/***
 * 6. Remote cubes creation
 ***/

glm::vec3 CubesFactory::createCube( const ResourceID& cubeID, const ResourceID& materialID, const ResourceID& firstTextureWallID, float width, float height, float depth )
{
    std::unique_ptr< Mesh > cube;

    width_ = width;
    height_ = height;
    depth_ = depth;

    cube = std::unique_ptr< Mesh >( new SystemMesh( cubeID, materialID, firstTextureWallID, generatePrimitiveData(), *materialsManager_ ) );
    const glm::vec3 cubeCentroid =
            cube->getOriginalCentroid();

    meshesManager_->addMesh( std::move( cube ), cubeID );

    return cubeCentroid;
}

/***
 * 7. SystemPrimitiveData generation
 ***/

void CubesFactory::generateVerticesPositionsAndUV( std::vector<glm::vec3> &positions,
                                                   std::vector<glm::vec2> &uvCoordinates )
{
    // Front face (vertices)
    positions.push_back( glm::vec3( width_ / 2.0f, height_ / 2.0f, depth_ / 2.0f ) );
    positions.push_back( glm::vec3( -width_ / 2.0f, height_ / 2.0f, depth_ / 2.0f ) );
    positions.push_back( glm::vec3( -width_ / 2.0f, -height_ / 2.0f, depth_ / 2.0f ) );
    positions.push_back( glm::vec3( width_ / 2.0f, -height_ / 2.0f, depth_ / 2.0f ) );

    // Back face (vertices)
    positions.push_back( glm::vec3( width_ / 2.0f, height_ / 2.0f, -depth_ / 2.0f ) );
    positions.push_back( glm::vec3( -width_ / 2.0f, height_ / 2.0f, -depth_ / 2.0f ) );
    positions.push_back( glm::vec3( -width_ / 2.0f, -height_ / 2.0f, -depth_ / 2.0f ) );
    positions.push_back( glm::vec3( width_ / 2.0f, -height_ / 2.0f, -depth_ / 2.0f ) );

    // UV vertices
    uvCoordinates.push_back( glm::vec2( 1.0f, 0.0f ) ); // Top right
    uvCoordinates.push_back( glm::vec2( 0.0f, 0.0f ) ); // Top left
    uvCoordinates.push_back( glm::vec2( 0.0f, 1.0f ) ); // Bottom left
    uvCoordinates.push_back( glm::vec2( 1.0f, 1.0f ) ); // Botom right
}


void CubesFactory::generateWalls( SystemPrimitiveData &primitiveData )
{
    unsigned int cubeFaceIndex;
    const char textureWallsNames[][32] =
    {
        "Front face",
        "Right face",
        "Left face",
        "Back face",
        "Top face",
        "Bottom face"
    };

    // Front face
    primitiveData.addQuad( IndicesQuad{ 0, 1, 2, 3 },
                           IndicesQuad{ 0, 1, 2, 3 } );

    // Right face
    primitiveData.addQuad( IndicesQuad{ 4, 0, 3, 7 },
                           IndicesQuad{ 0, 1, 2, 3 } );

    // Left face
    primitiveData.addQuad( IndicesQuad{ 1, 5, 6, 2 },
                           IndicesQuad{ 0, 1, 2, 3 } );

    // Back face
    primitiveData.addQuad( IndicesQuad{ 5, 4, 7, 6 },
                           IndicesQuad{ 0, 1, 2, 3 } );

    // Top face
    primitiveData.addQuad( IndicesQuad{ 4, 5, 1, 0 },
                           IndicesQuad{ 0, 1, 2, 3 } );

    // Bottom face
    primitiveData.addQuad( IndicesQuad{ 3, 2, 6, 7 },
                           IndicesQuad{ 0, 1, 2, 3 } );

    for( cubeFaceIndex = 0; cubeFaceIndex < 6; cubeFaceIndex++ ){
        primitiveData.trianglesGroups.push_back(
                    NamedTrianglesGroup(
                        textureWallsNames[cubeFaceIndex],
                        cubeFaceIndex * 2,
                        2 ) );

        primitiveData.generatePerFaceNormals( cubeFaceIndex );
    }
}

} // namespace como
