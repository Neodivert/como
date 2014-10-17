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

#include "spheres_factory.hpp"

namespace como {

const float FIRST_RADIAL_VERTEX_INDEX = 2;


/***
 * 1. Construction
 ***/

SpheresFactory::SpheresFactory(ServerInterfacePtr server, MeshesManagerPtr meshesManager, MaterialsManagerPtr materialsManager, TextureWallsManager *textureWallsManager) :
    SpecializedSystemPrimitivesFactory( server, "Sphere", meshesManager, materialsManager, textureWallsManager )
{}


/***
 * 3. Local cones creation
 ***/

ResourceID SpheresFactory::createSphere( float radius, std::uint16_t nDivisions )
{
    const ResourceID sphereID = reserveResourceIDs( 1 );
    const ResourceID materialID = reserveResourceIDs( 1 );
    const ResourceID firstTextureWallID = reserveResourceIDs( 1 );

    createSphere( sphereID, materialID, firstTextureWallID, radius, nDivisions );

    sendCommandToServer(
                CommandConstPtr(
                    new SphereCreationCommand(
                        sphereID,
                        materialID,
                        firstTextureWallID,
                        radius,
                        nDivisions ) ) );

    return sphereID;
}


/***
 * 4. Remote command execution
 ***/

void SpheresFactory::executeRemoteCommand( const SphereCreationCommand &command )
{
    createSphere( command.getMeshID(),
                  command.getMaterialID(),
                  command.getFirstTextureWallID(),
                  command.sphereRadius(),
                  command.sphereNDivisions() );
}


/***
 * 6. Primitive data generation
 ***/

void SpheresFactory::generateVertexData( MeshVertexData &vertexData )
{
    int currentDivision = 0;
    float currentRadius;
    float currentY;
    int firstCircleIndex;
    int secondCircleIndex;
    const float DELTA_ANGLE = glm::pi<float>() / sphereNDivisions_;
    float currentAngle = 0.0f;

    vertexData.vertices.reserve( nExpectedVertices() );
    vertexData.vertexTriangles.reserve( nExpectedTriangles() );

    // Generate the vertices of the sphere.
    for( currentDivision = 0; currentDivision < sphereNDivisions_ + 1; currentDivision++ ){
        currentAngle = currentDivision * DELTA_ANGLE;
        currentY = cos( currentAngle ) * sphereRadius_;
        currentRadius = sin( currentAngle ) * sphereRadius_;

        generateHorizontalCircleVertices( vertexData.vertices,
                                          currentRadius,
                                          sphereNDivisions_ + 1,
                                          currentY );
    }

    // Generate the triangles of the sphere.
    for( currentDivision = 0; currentDivision < sphereNDivisions_; currentDivision++ ){
        firstCircleIndex = currentDivision * ( sphereNDivisions_ + 1 );
        secondCircleIndex = firstCircleIndex + ( sphereNDivisions_ + 1 );

        generateTriangles( vertexData.vertexTriangles,
                           firstCircleIndex,
                           secondCircleIndex,
                           sphereNDivisions_ + 1 );
    }
}


void SpheresFactory::generateUVData( MeshTextureData &uvData )
{
    const float DELTA_ANGLE = glm::pi<float>() / sphereNDivisions_;
    float currentVerticalAngle = 0.0f;
    float currentHorizontalAngle = 0.0f;
    int currentDivision;
    int currentHorizontalDivision;
    float currentY;
    float currentX;
    int firstCircleIndex, secondCircleIndex;
    //const float DELTA_X = 1.0f / (sphereNDivisions_ + 1.0f);

    // Generate the UV vertices of the sphere.
    for( currentDivision = 0; currentDivision < sphereNDivisions_ + 1; currentDivision++ ){
        currentVerticalAngle = currentDivision * DELTA_ANGLE;
        currentY = 0.5f + 0.5f * -cos( currentVerticalAngle );

        for( currentHorizontalDivision = 0; currentHorizontalDivision < sphereNDivisions_ + 1; currentHorizontalDivision++ ){
            currentHorizontalAngle = currentHorizontalDivision * DELTA_ANGLE;
            currentX = 0.5f + 0.5f * cos( currentHorizontalAngle );

            uvData.uvVertices.push_back( glm::vec2( currentX, currentY ) );
        }
    }

    // Generate the UV triangles of the sphere.
    for( currentDivision = 0; currentDivision < sphereNDivisions_; currentDivision++ ){
        firstCircleIndex = currentDivision * ( sphereNDivisions_ + 1 );
        secondCircleIndex = firstCircleIndex + ( sphereNDivisions_ + 1 );

        generateTriangles( uvData.uvTriangles,
                           firstCircleIndex,
                           secondCircleIndex,
                           sphereNDivisions_ + 1 );
    }
}


void SpheresFactory::generateTrianglesGroups( std::vector<NamedTrianglesGroup> &trianglesGroups )
{
    trianglesGroups.push_back( NamedTrianglesGroup( "Sphere", 0, nExpectedTriangles() ) );
}


/***
 * 7. Remote spheres creation
 ***/

void SpheresFactory::createSphere(const ResourceID &sphereID, const ResourceID &materialID, const ResourceID &firstTextureWallID, float radius, std::uint16_t nDivisions )
{
    MeshPtr sphere;

    sphereRadius_ = radius;
    sphereNDivisions_ = nDivisions;

    sphere = MeshPtr( new SystemMesh( sphereID, materialID, firstTextureWallID, generatePrimitiveData(), *materialsManager_ ) );

    // On this version of COMO, geometric primitives are synchronized as they
    // had a centroid 0, so check that this is true.
    assert( sphere->centroid() == glm::vec3( 0.0f ) );

    meshesManager_->addMesh( std::move( sphere ), sphereID );
}


/***
 * 8. Auxiliar methods
 ***/

void SpheresFactory::generateHorizontalCircleVertices(std::vector<glm::vec3>& vertices, float radius, unsigned int nDivisions, float y)
{
    unsigned int i = 0;
    const float DELTA_ANGLE = ( 2.0f * glm::pi<float>() ) / (float)nDivisions;
    float currentAngle = 0.0f;

    for( i = 0; i < nDivisions; i++ ){
        vertices.push_back(
                    glm::vec3( radius * cos( currentAngle ),
                               y,
                               radius * sin( currentAngle ) ) );
        currentAngle += DELTA_ANGLE;
    }
}


void SpheresFactory::generateTriangles( std::vector<IndicesTriangle>& triangles, unsigned int firstCircleStartIndex, unsigned int secondCircleStartIndex, unsigned int nDivisions )
{
    unsigned int i;
    unsigned int firstCircleCurrentVertex;
    unsigned int secondCircleCurrentVertex;

    for( i = 0; i < nDivisions - 1; i++ ){
        firstCircleCurrentVertex = firstCircleStartIndex + i;
        secondCircleCurrentVertex = secondCircleStartIndex + i;

        triangles.push_back(
                    IndicesTriangle({
                                        firstCircleCurrentVertex,
                                        firstCircleCurrentVertex + 1,
                                        secondCircleCurrentVertex
                                    }) );
        triangles.push_back(
                    IndicesTriangle({
                                        firstCircleCurrentVertex + 1,
                                        secondCircleCurrentVertex + 1,
                                        secondCircleCurrentVertex
                                    }) );
    }
    firstCircleCurrentVertex = firstCircleStartIndex + i;
    secondCircleCurrentVertex = secondCircleStartIndex + i;

    triangles.push_back(
                IndicesTriangle({
                                    firstCircleCurrentVertex,
                                    firstCircleStartIndex,
                                    secondCircleCurrentVertex
                                }) );
    triangles.push_back(
                IndicesTriangle({
                                    firstCircleStartIndex,
                                    secondCircleStartIndex,
                                    secondCircleCurrentVertex
                                }) );
}


unsigned int SpheresFactory::nExpectedVertices() const
{
    return pow( sphereNDivisions_ + 1, 2 );
}


unsigned int SpheresFactory::nExpectedTriangles() const
{
    return ( sphereNDivisions_ + 1 ) * ( sphereNDivisions_ ) * 2;
}

} // namespace como
