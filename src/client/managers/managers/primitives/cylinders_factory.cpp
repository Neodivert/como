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

#include "cylinders_factory.hpp"

namespace como {


/***
 * 1. Construction
 ***/

CylindersFactory::CylindersFactory( ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager *textureWallsManager ) :
    SpecializedSystemPrimitivesFactory( server, "Cylinder", meshesManager, materialsManager, textureWallsManager )
{}


/***
 * 3. Local cylinders creation
 ***/

ResourceID CylindersFactory::createCylinder( float height, float radius, std::uint16_t nRadialVertices )
{
    const ResourceID meshID = reserveResourceIDs( 1 );
    const ResourceID materialID = reserveResourceIDs( 1 );
    const ResourceID firstTextureWallID = reserveResourceIDs( 3 );

    createCylinder( meshID, materialID, firstTextureWallID, height, radius, nRadialVertices );

    sendCommandToServer(
                CommandConstPtr(
                    new CylinderCreationCommand(
                        meshID,
                        materialID,
                        firstTextureWallID,
                        height,
                        radius,
                        nRadialVertices ) ) );

    return meshID;
}


/***
 * 4. Remote command execution
 ***/

void CylindersFactory::executeRemoteCommand( const CylinderCreationCommand &command )
{
    createCylinder( command.getMeshID(),
                    command.getMaterialID(),
                    command.getFirstTextureWallID(),
                    command.cylinderHeight(),
                    command.cylinderRadius(),
                    command.cylinderNRadialVertices() );
}


/***
 * 6. Primitive data generation
 ***/

void CylindersFactory::generateVerticesPositionsAndUV(std::vector<glm::vec3> &positions, std::vector<glm::vec2> &uvCoordinates)
{
    int i;

    // Create top vertices (positions and UV)
    generateHorizontalVerticesCircle( positions,
                                      cylinderRadius_,
                                      cylinderNRadialVertices_,
                                      cylinderHeight_ / 2.0f );
    generateHorizontalUVCircle( uvCoordinates,
                                cylinderNRadialVertices_ );

    // Create bottom vertices (positions and UV)
    generateHorizontalVerticesCircle( positions,
                                      cylinderRadius_,
                                      cylinderNRadialVertices_,
                                      -cylinderHeight_ / 2.0f );
    // FIXME: Duplicated UV vertices.
    generateHorizontalUVCircle( uvCoordinates,
                                cylinderNRadialVertices_ );

    // Create radial top UV vertices.
    for( i = 0; i < cylinderNRadialVertices_ + 1; i++ ){
        uvCoordinates.push_back(
                    glm::vec2(
                        static_cast<float>( i ) / static_cast<float>( cylinderNRadialVertices_ ),
                        0.0f ) );
    }

    // Create radial bottom UV vertices.
    for( i = 0; i < cylinderNRadialVertices_ + 1; i++ ){
        uvCoordinates.push_back(
                    glm::vec2(
                        static_cast<float>( i ) / static_cast<float>( cylinderNRadialVertices_ ),
                        1.0f ) );
    }
}


void CylindersFactory::generateWalls( SystemPrimitiveData &primitiveData )
{
    IndicesQuad currentVertexQuad;
    IndicesQuad currentUVQuad;
    int i;
    unsigned int currentTopVertexIndex;
    unsigned int currentBottomVertexIndex;
    unsigned int currentTopUVIndex;
    unsigned int currentBottomUVIndex;

    // Create top wall's triangles (positions and UV).
    generateTrianglesCircle( primitiveData,
                             cylinderNRadialVertices_,
                             0,
                             1,
                             false );

    // Create bottom wall's triangles (positions and UV).
    generateTrianglesCircle( primitiveData,
                             cylinderNRadialVertices_,
                             cylinderNRadialVertices_ + 1,
                             cylinderNRadialVertices_ + 2,
                             true );

    // Create radial wall's triangles (positions).
    const unsigned int TOP_FIRST_RADIAL_VERTEX_INDEX = 1;
    const unsigned int BOTTOM_FIRST_RADIAL_VERTEX_INDEX = cylinderNRadialVertices_ + 2;
    const unsigned int TOP_FIRST_RADIAL_UV_INDEX = 2 + cylinderNRadialVertices_ * 2;
    const unsigned int BOTTOM_FIRST_RADIA_UV_INDEX = 3 + cylinderNRadialVertices_ * 3;

    for( i = 0; i < cylinderNRadialVertices_ - 1; i++ ){
        currentTopVertexIndex = TOP_FIRST_RADIAL_VERTEX_INDEX + i;
        currentTopUVIndex = TOP_FIRST_RADIAL_UV_INDEX + i;
        currentBottomVertexIndex = BOTTOM_FIRST_RADIAL_VERTEX_INDEX + i;
        currentBottomUVIndex = BOTTOM_FIRST_RADIA_UV_INDEX + i;

        currentVertexQuad = { currentTopVertexIndex,
                              currentTopVertexIndex + 1,
                              currentBottomVertexIndex + 1,
                              currentBottomVertexIndex };
        currentUVQuad = { currentTopUVIndex,
                          currentTopUVIndex + 1,
                          currentBottomUVIndex + 1,
                          currentBottomUVIndex };

        primitiveData.addQuad( currentVertexQuad, currentUVQuad );
    }

    currentTopVertexIndex = TOP_FIRST_RADIAL_VERTEX_INDEX + i;
    currentTopUVIndex = TOP_FIRST_RADIAL_UV_INDEX + i;
    currentBottomVertexIndex = BOTTOM_FIRST_RADIAL_VERTEX_INDEX + i;
    currentBottomUVIndex = BOTTOM_FIRST_RADIA_UV_INDEX + i;

    currentVertexQuad = { currentTopVertexIndex,
                          TOP_FIRST_RADIAL_VERTEX_INDEX,
                          BOTTOM_FIRST_RADIAL_VERTEX_INDEX,
                          currentBottomVertexIndex };
    currentUVQuad = { currentTopUVIndex,
                      currentTopUVIndex + 1,
                      currentBottomUVIndex + 1,
                      currentBottomUVIndex };
    primitiveData.addQuad( currentVertexQuad, currentUVQuad );

    // Create walls
    primitiveData.trianglesGroups.push_back( NamedTrianglesGroup( "Top", 0, cylinderNRadialVertices_ ) );
    primitiveData.trianglesGroups.push_back( NamedTrianglesGroup( "Bottom", cylinderNRadialVertices_, cylinderNRadialVertices_ ) );
    primitiveData.trianglesGroups.push_back( NamedTrianglesGroup( "Radial", cylinderNRadialVertices_ * 2, cylinderNRadialVertices_ * 2 ) );

    for( unsigned int wallIndex = 0;
         wallIndex < primitiveData.trianglesGroups.size();
         wallIndex++ ){
        primitiveData.generatePerVertexNormals( wallIndex );
    }
}


/***
 * 7. Remote cylinders creation
 ***/

void CylindersFactory::createCylinder( const ResourceID &cylinderID, const ResourceID &materialID, const ResourceID &firstTextureWallID, float height, float radius, std::uint16_t nRadialVertices )
{
    std::unique_ptr< Mesh > cylinder = nullptr;

    cylinderHeight_ = height;
    cylinderRadius_ = radius;
    cylinderNRadialVertices_ = nRadialVertices;

    cylinder = std::unique_ptr< Mesh >( new SystemMesh( cylinderID, materialID, firstTextureWallID, generatePrimitiveData(), *materialsManager_ ) );

    // On this version of COMO, geometric primitives are synchronized as they
    // had a centroid 0, so check that this is true.
    assert( cylinder->centroid() == glm::vec3( 0.0f ) );

    meshesManager_->addMesh( std::move( cylinder ), cylinderID );
}

} // namespace como
