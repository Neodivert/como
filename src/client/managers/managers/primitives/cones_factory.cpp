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

void ConesFactory::generateVertexData( MeshVertexData &vertexData )
{
    // Create top center vertex
    const unsigned int TOP_CENTER_INDEX =
            vertexData.vertices.size();
    vertexData.vertices.push_back({
                0.0f,
                coneHeight_ / 2.0f,
                0.0f });

    // Create bottom vertices
    const unsigned int BOTTOM_CENTER_INDEX =
            generateHorizontalVerticesCircle( vertexData.vertices,
                                              coneRadius_,
                                              coneNBaseVertices_,
                                              -coneHeight_ / 2.0f );
    const unsigned int FIRST_RADIAL_VERTEX_INDEX =
            BOTTOM_CENTER_INDEX + 1;

    // Create radial face triangles
    generateTrianglesCircle( vertexData.vertexTriangles,
                             coneNBaseVertices_,
                             TOP_CENTER_INDEX,
                             FIRST_RADIAL_VERTEX_INDEX,
                             false );

    // Create bottom face triangles
    generateTrianglesCircle( vertexData.vertexTriangles,
                             coneNBaseVertices_,
                             BOTTOM_CENTER_INDEX,
                             FIRST_RADIAL_VERTEX_INDEX,
                             true );
}


void ConesFactory::generateUVData( MeshTextureData &uvData )
{
    // Create top and bottom UV vertices
    const unsigned int TOP_CENTER_INDEX =
            generateHorizontalUVCircle( uvData.uvVertices,
                                        coneNBaseVertices_ );
    const unsigned int BOTTOM_CENTER_INDEX = TOP_CENTER_INDEX;
    const unsigned int FIRST_RADIAL_VERTEX_INDEX =
            BOTTOM_CENTER_INDEX + 1;

    // Create radial UV triangles
    generateTrianglesCircle( uvData.uvTriangles,
                             coneNBaseVertices_,
                             TOP_CENTER_INDEX,
                             FIRST_RADIAL_VERTEX_INDEX,
                             false );

    // Create bottom UV triangles
    generateTrianglesCircle( uvData.uvTriangles,
                             coneNBaseVertices_,
                             BOTTOM_CENTER_INDEX,
                             FIRST_RADIAL_VERTEX_INDEX,
                             true );
}


void ConesFactory::generateTrianglesGroups( std::vector<NamedTrianglesGroup> &trianglesGroups )
{
    trianglesGroups.push_back( NamedTrianglesGroup( "Round", 0, coneNBaseVertices_ ) );
    trianglesGroups.push_back( NamedTrianglesGroup( "Base", coneNBaseVertices_, coneNBaseVertices_ ) );
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
