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

#include "cones_factory.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtc/constants.hpp> // glm::pi

const unsigned int TOP_CENTER_VERTEX_INDEX = 0;
const unsigned int BOTTOM_CENTER_VERTEX_INDEX = 1;
const unsigned int FIRST_RADIAL_VERTEX_INDEX = 2;

namespace como {

/***
 * 1. Construction
 ***/

ConesFactory::ConesFactory(ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager *textureWallsManager) :
    SpecializedSystemPrimitivesFactory( server, "Cone", meshesManager, materialsManager, textureWallsManager )
{}


/***
 * 3. Local cones creation
 ***/

ResourceID ConesFactory::createCone( float height, float radius, std::uint16_t nBaseVertices )
{
    ResourceID coneID = reserveResourceIDs( 1 );
    ResourceID materialID = reserveResourceIDs( 1 );
    ResourceID firstTextureWallID = reserveResourceIDs( 6 );

    createCone( coneID, materialID, firstTextureWallID, height, radius, nBaseVertices );

    // Send command to server
    sendCommandToServer(
                CommandConstPtr(
                    new ConeCreationCommand(
                        coneID,
                        materialID,
                        firstTextureWallID,
                        height,
                        radius,
                        nBaseVertices ) ) );

    return coneID;
}


/***
 * 4. Remote command execution
 ***/

void ConesFactory::executeRemoteCommand( const ConeCreationCommand &command )
{
    createCone( command.getMeshID(),
                command.getMaterialID(),
                command.getFirstTextureWallID(),
                command.coneHeight(),
                command.coneRadius(),
                command.coneNBaseVertices() );
}




/***
 * 6. Primitive data generation
 ***/

void ConesFactory::generateVerticesPositionsAndUV( std::vector<glm::vec3> &positions, std::vector<glm::vec2> &uvCoordinates )
{
    assert( TOP_CENTER_VERTEX_INDEX == positions.size() );
    assert( TOP_CENTER_VERTEX_INDEX == uvCoordinates.size() );

    // Create top center vertex (position and UV).
    positions.push_back({
                            0.0f,
                            coneHeight_ / 2.0f,
                            0.0f });
    uvCoordinates.push_back({ 0.5f, 0.5f });

    assert( BOTTOM_CENTER_VERTEX_INDEX == positions.size() );
    assert( BOTTOM_CENTER_VERTEX_INDEX == uvCoordinates.size() );

    // Create bottom vertices (positions and UVs).
    generateHorizontalVerticesCircle( positions,
                                      coneRadius_,
                                      coneNBaseVertices_,
                                      -coneHeight_ / 2.0f );
    generateHorizontalUVCircle( uvCoordinates,
                                coneNBaseVertices_ );
}


void ConesFactory::generateWalls( SystemPrimitiveData &primitiveData )
{
    // Create radial face triangles (positions and UV).
    generateTrianglesCircle( primitiveData,
                             coneNBaseVertices_,
                             TOP_CENTER_VERTEX_INDEX,
                             FIRST_RADIAL_VERTEX_INDEX,
                             false );

    // Create base face triangles (positions and UV).
    generateTrianglesCircle( primitiveData,
                             coneNBaseVertices_,
                             BOTTOM_CENTER_VERTEX_INDEX,
                             FIRST_RADIAL_VERTEX_INDEX,
                             true );

    primitiveData.trianglesGroups.push_back( NamedTrianglesGroup( "Round", 0, coneNBaseVertices_ ) );
    primitiveData.generatePerVertexNormals( 0 );

    primitiveData.trianglesGroups.push_back( NamedTrianglesGroup( "Base", coneNBaseVertices_, coneNBaseVertices_ ) );
    primitiveData.generatePerVertexNormals( 1 );
}


/***
 * 7. Remote cones creation
 ***/

void ConesFactory::createCone( const ResourceID &coneID, const ResourceID &materialID, const ResourceID &firstTextureWallID, float height, float radius, std::uint16_t nBaseVertices )
{
    std::unique_ptr< Mesh > cone;

    coneHeight_ = height;
    coneRadius_ = radius;
    coneNBaseVertices_ = nBaseVertices;

    cone = std::unique_ptr< Mesh >( new SystemMesh( coneID, materialID, firstTextureWallID, generatePrimitiveData(), *materialsManager_ ) );

    // On this version of COMO, geometric primitives are synchronized as they
    // had a centroid 0, so check that this is true.
    assert( cone->centroid() == glm::vec3( 0.0f ) );

    meshesManager_->addMesh( std::move( cone ), coneID );
}


} // namespace como
