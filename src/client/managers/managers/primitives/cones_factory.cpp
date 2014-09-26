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
#include <glm/gtc/constants.hpp>

namespace como {

const unsigned int FIRST_CIRCUMFERENCE_VERTEX_INDEX = 2;

/***
 * 1. Construction
 ***/

ConesFactory::ConesFactory(ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager *textureWallsManager) :
    SpecializedSystemPrimitivesFactory( server, meshesManager, materialsManager, textureWallsManager )
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

    // TODO: Send command to server
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
    unsigned int i;
    const float stepAngle = 2.0f * glm::pi<float>() / coneNBaseVertices_;
    float currentAngle = 0.0f;

    // Set cone vertices.
    vertexData.vertices.push_back( glm::vec3( 0.0f, coneHeight_ / 2.0f, 0.0f ) );
    vertexData.vertices.push_back( glm::vec3( 0.0f, -coneHeight_ / 2.0f, 0.0f ) );
    for( i = 0; i < coneNBaseVertices_; i++ ){
        vertexData.vertices.push_back(
                    glm::vec3( coneRadius_ * cos( currentAngle ),
                               -coneHeight_ / 2.0f,
                               coneRadius_ * sin( currentAngle ) ) );
        currentAngle += stepAngle;
    }

    // Set cone triangles
    generateTriangles( vertexData.vertexTriangles );

    // TODO: Include base's center and triangles.
}


void ConesFactory::generateUVData( MeshTextureData &uvData )
{
    unsigned int i;
    const float stepAngle = 2.0f * glm::pi<float>() / coneNBaseVertices_;
    float currentAngle = 0.0f;

    // Set UV coordinates.
    uvData.uvVertices.push_back( glm::vec2( 0.5f, 0.5f ) );
    uvData.uvVertices.push_back( glm::vec2( 0.5f, 0.5f ) ); // TODO: Change coordinates.
    for( i = 0; i < coneNBaseVertices_; i++ ){
        uvData.uvVertices.push_back(
                    glm::vec2(
                        0.5f + 0.5f * cos( currentAngle ),
                        0.5f + 0.5f * sin( currentAngle ) ) );
        currentAngle += stepAngle;
    }

    // Set UV triangles
    generateTriangles( uvData.uvTriangles );
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

    materialsManager_->createMaterial( MaterialInfo(), materialID, coneID );

    cone = std::unique_ptr< Mesh >( new SystemMesh( coneID, firstTextureWallID, generatePrimitiveData(), materialsManager_->getMaterial( materialID ) ) );

    meshesManager_->addMesh( std::move( cone ), coneID );
}


/***
 * 8. Auxiliar methods
 ***/

void ConesFactory::generateTriangles( std::vector< IndicesTriangle > &triangles )
{
    int i;
    unsigned int circumferenceVertexIndex;

    for( i = 0; i < coneNBaseVertices_ - 1; i++ ){
        circumferenceVertexIndex = i + FIRST_CIRCUMFERENCE_VERTEX_INDEX;
        triangles.push_back( { 0, circumferenceVertexIndex + 1, circumferenceVertexIndex } );
    }
    circumferenceVertexIndex = i + FIRST_CIRCUMFERENCE_VERTEX_INDEX;
    triangles.push_back( { 0, FIRST_CIRCUMFERENCE_VERTEX_INDEX, circumferenceVertexIndex } );

    // Base triangles
    for( i = 0; i < coneNBaseVertices_ - 1; i++ ){
        circumferenceVertexIndex = i + FIRST_CIRCUMFERENCE_VERTEX_INDEX;
        triangles.push_back( { 1, circumferenceVertexIndex, circumferenceVertexIndex + 1 } );
    }
    circumferenceVertexIndex = i + FIRST_CIRCUMFERENCE_VERTEX_INDEX;
    triangles.push_back( { 1, circumferenceVertexIndex, FIRST_CIRCUMFERENCE_VERTEX_INDEX } );
}


} // namespace como
