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

#include "mesh.hpp"

#define GLM_FORCE_RADIANS
#include <glm/gtx/intersect.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <common/primitives/primitive_file.hpp>

#include <iostream>

namespace como {

// Initialize the location of various uniform shader variables as unitialized (-1).
GLint Mesh::uniformColorLocation = -1;
GLint Mesh::mvpMatrixLocation_ = -1;
GLint Mesh::normalMatrixLocation_ = -1;

const GLint SHADER_VERTEX_ATTR_LOCATION = 0;
const GLint SHADER_NORMAL_ATTR_LOCATION = 1;
const GLint SHADER_UV_ATTR_LOCATION = 2;

const char DEFAULT_MESH_NAME[] = "Mesh #";



/***
 * 1. Construction.
 ***/

Mesh::Mesh( MeshType type, const char* filePath, MaterialConstPtr material, bool displayVertexNormals ) :
    Entity( DrawableType::MESH, DEFAULT_MESH_NAME ),
    type_( type ),
    displayVertexNormals_( displayVertexNormals )
{
    MeshInfo meshInfo;
    PrimitiveFile::read( meshInfo, filePath );

    vertexData_ = meshInfo.vertexData;
    polygonsGroups_ = meshInfo.polygonGroupsData;
    materials_.push_back( material );

    init( meshInfo.oglData );
}


Mesh::Mesh( MeshVertexData vertexData, const MeshOpenGLData& oglData, const std::vector< PolygonGroupData >& polygonsGroups, const std::vector< MaterialConstPtr >& materials, bool displayVertexNormals ) :
    Entity( DrawableType::MESH, DEFAULT_MESH_NAME ),
    type_( MeshType::MESH ),
    vertexData_( vertexData ),
    polygonsGroups_( polygonsGroups ),
    materials_( materials ),
    displayVertexNormals_( displayVertexNormals )
{
    init( oglData );
}


/*
Mesh::Mesh( const Mesh& b ) :
    Entity( b ),
    type_( b.type_ ),
    vertexData_( b.vertexData_ ),
    triangles( b.triangles ),
    originalCentroid( b.originalCentroid ),
    transformedCentroid( b.transformedCentroid ),
    material_( b.material_ )
{
    init( oglData );
    // TODO: Copy buffers.
}
*/


/***
 * 2. Destruction.
 ***/

Mesh::~Mesh()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &vbo );
    glDeleteBuffers( 1, &ebo );
    glDeleteVertexArrays( 1, &vao );
}


/***
 * 3. Initialization.
 ***/

void Mesh::init( const MeshOpenGLData& oglData )
{
    genOpenGLBuffers();

    initShaderLocations();

    populateOpenGLBuffers( oglData );

    includesTexture_ = oglData.includesTextures;
    componensPerVertex_ = oglData.componentsPerVertex;

    initVAO();

    // Set both original and transformed centroids.
    originalCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    transformedCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
}


void Mesh::genOpenGLBuffers()
{
    // Generate a VAO for the Mesh and bind it as the active one.
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Generate a VBO and bind it for holding vertex data.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Generate an EBO and bind it for holding vertex indices.
    glGenBuffers( 1, &ebo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );
}


void Mesh::initShaderLocations()
{
    GLint currentShaderProgram;

    if( uniformColorLocation == -1 ){
        // Location of uniform shader variable "color" hasn't been initialized yet.

        // Get current shader program id.
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
        assert( currentShaderProgram != 0 );

        // Get location of uniform shader variable "color".
        uniformColorLocation = glGetUniformLocation( currentShaderProgram, "material.color" );
        assert( uniformColorLocation != -1 );

        // Get location of uniform shader variable "mvpMatrix".
        mvpMatrixLocation_ = glGetUniformLocation( currentShaderProgram, "mvpMatrix" );
        assert( mvpMatrixLocation_ != -1 );

        // Get location of uniform shader variable "normalMatrix".
        normalMatrixLocation_ = glGetUniformLocation( currentShaderProgram, "normalMatrix" );
        assert( normalMatrixLocation_ != -1 );
    }
}


void Mesh::populateOpenGLBuffers( const MeshOpenGLData& oglData )
{
    const unsigned int vboSize = oglData.vboData.size() * sizeof( GLfloat );
    const unsigned int eboSize = oglData.eboData.size() * sizeof( GLuint );

    // Populate the VBO.
    glBufferData( GL_ARRAY_BUFFER, vboSize, &( oglData.vboData[0] ), GL_STATIC_DRAW );

    // Populate the EBO.
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, eboSize, &( oglData.eboData[0] ), GL_STATIC_DRAW );

    nEboElements_ = oglData.eboData.size();
}


// TODO: Remove this method.
void Mesh::initMeshBuffers()
{
    GLint currentShaderProgram;

    // Generate a VAO for the Mesh and bind it as the active one.
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Generate a VBO and bind it for holding vertex data.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Generate an EBO and bind it for holding vertex indices.
    glGenBuffers( 1, &ebo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    if( uniformColorLocation == -1 ){
        // Location of uniform shader variable "color" hasn't been initialized yet.

        // Get current shader program id.
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

        // Get location of uniform shader variable "color".
        uniformColorLocation = glGetUniformLocation( currentShaderProgram, "material.color" );

        // Get location of uniform shader variable "mvpMatrix".
        mvpMatrixLocation_ = glGetUniformLocation( currentShaderProgram, "mvpMatrix" );

        // Get location of uniform shader variable "normalMatrix".
        normalMatrixLocation_ = glGetUniformLocation( currentShaderProgram, "normalMatrix" );
    }

    // Set both original and transformed centroids.
    originalCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    transformedCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
}

void Mesh::initVAO()
{
    // Set the organization of the vertex and normals data in the VBO.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glVertexAttribPointer( SHADER_VERTEX_ATTR_LOCATION, 3, GL_FLOAT, GL_FALSE, getBytesPerVertex(), (void *)( 0 ) );
    glVertexAttribPointer( SHADER_NORMAL_ATTR_LOCATION, 3, GL_FLOAT, GL_FALSE, getBytesPerVertex(), (void *)( COMPONENTS_PER_VERTEX_POSITION * sizeof( GL_FLOAT ) ) );

    // Enable previous vertex data arrays.
    glEnableVertexAttribArray( SHADER_VERTEX_ATTR_LOCATION );
    glEnableVertexAttribArray( SHADER_NORMAL_ATTR_LOCATION );

    if( includesTexture_ ){
        glVertexAttribPointer( SHADER_UV_ATTR_LOCATION, 3, GL_FLOAT, GL_FALSE, getBytesPerVertex(), (void *)( COMPONENTS_PER_VERTEX_POSITION * 2 * sizeof( GL_FLOAT ) ) );
        glEnableVertexAttribArray( SHADER_UV_ATTR_LOCATION );
    }
}


unsigned int Mesh::getBytesPerVertex() const
{
    return getComponentsPerVertex() * sizeof( GL_FLOAT );
}

unsigned int Mesh::getComponentsPerVertex() const
{
    return componensPerVertex_;
}


void Mesh::computeCentroid()
{
    unsigned int i = 0;

    originalCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    for( i = 0; i < vertexData_.vertices.size(); i++ ){
        originalCentroid += glm::vec4( vertexData_.vertices[i], 1.0f );
    }

    originalCentroid /= vertexData_.vertices.size();
    originalCentroid.w = 1.0f;
}


/***
 * 5. Getters.
 ***/

MeshType Mesh::getType() const
{
    return type_;
}

bool Mesh::displaysVertexNormals() const
{
    return displayVertexNormals_;
}

glm::vec3 Mesh::centroid() const
{
    return glm::vec3( transformedCentroid );
}


/***
 * 6. Setters
 ***/

void Mesh::displayVertexNormals( bool display )
{
    displayVertexNormals_ = display;
}


/***
 * 7. Intersections.
 ***/

void Mesh::intersects( glm::vec3 rayOrigin, glm::vec3 rayDirection, float& minT, unsigned int* triangle ) const
{
    const float MAX_T = 999999.9f;
    float t = MAX_T;
    glm::vec3 intersection;

    minT = MAX_T;

    // Transform the ray's origin and direction from world to object
    // coordinates.
    rayOrigin = glm::vec3( glm::inverse( modelMatrix_ ) * glm::vec4( rayOrigin, 1.0f ) );
    rayDirection = glm::vec3( glm::inverse( modelMatrix_ ) * glm::vec4( rayDirection, 0.0f ) );

    // Normalize the direction of the ray.
    rayDirection = glm::normalize( rayDirection );

    // Compute intersections with all triangles in this Mesh.
    for( unsigned int i = 0; i < vertexData_.vertexTriangles.size(); i++ ){
        if( glm::intersectRayTriangle( rayOrigin,
                                       rayDirection,
                                       vertexData_.vertices[vertexData_.vertexTriangles[i][0]],
                                       vertexData_.vertices[vertexData_.vertexTriangles[i][1]],
                                       vertexData_.vertices[vertexData_.vertexTriangles[i][2]],
                                       intersection ) ){

            // There was an intersection with this triangle. Get the parameter t.
            t = intersection.z;

            // Do we have a new minimum t?
            if( t < minT ){
                minT = t;
                if( triangle != nullptr ){
                    *triangle = i;
                }
            }
        }
    }

    // If the ray didn't intersect the mesh, we "return" -1.
    if( minT >= MAX_T ){
        minT = -1.0f;
    }
}


/***
 * 8. Update and drawing.
 ***/

void Mesh::update()
{
    // Update mesh's orientation.
    Entity::update();

    // Update mesh's centroid.
    transformedCentroid = modelMatrix_ * originalCentroid;
}


void Mesh::draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec4* contourColor ) const
{
    if( includesTexture_ ){
        openGL->setShadingMode( ShadingMode::SOLID_LIGHTING_AND_TEXTURING );
    }else{
        openGL->setShadingMode( ShadingMode::SOLID_LIGHTING );
    }

    // Compute MVP matrix and pass it to the shader.
    openGL->setMVPMatrix( modelMatrix_, viewMatrix, projectionMatrix );

    // Bind Mesh VAO and VBOs as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    for( auto polygonsGroup : polygonsGroups_ ){
        // Send this mesh's material to shader.
        materials_[polygonsGroup.materialIndex]->sendToShader();

        glDrawElements( GL_TRIANGLES,
                        polygonsGroup.nTriangles * 3,
                        GL_UNSIGNED_INT,
                        ( std::intptr_t* )( polygonsGroup.firstTriangle * 3 * sizeof( GL_UNSIGNED_INT ) ) );
    }


    // Set the color for the mesh's contour.
    if( contourColor != nullptr ){
        openGL->setShadingMode( ShadingMode::SOLID_PLAIN );

        glUniform4fv( uniformColorLocation, 1, glm::value_ptr( *contourColor ) );
        OpenGL::checkStatus( "contourColor sent to shader" );

        // Now we'll draw mesh's contour. Set polygon mode for rendering
        // lines.
        // TODO: I still have to use polygon offset.
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // Draw Mesh's contour
        glDrawElements( GL_TRIANGLES, nEboElements_, GL_UNSIGNED_INT, NULL );

        // Return polygon mode to previos GL_FILL.
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }

    if( displayVertexNormals_ ){
        drawVertexNormals( openGL, viewMatrix, projectionMatrix, glm::vec4( 1.0f, 0.0f, 0.0f, 0.0f ) );
    }
}


void Mesh::drawVertexNormals( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::vec4 color ) const
{
    GLint currentProgram = -1;
    GLint colorUniformLocation = -1;

    openGL->setShadingMode( ShadingMode::NORMALS );
    openGL->setMVPMatrix( modelMatrix_, viewMatrix, projectionMatrix );

    // We don't want to send UV coordinates to shader.
    if( includesTexture_ ){
        glDisableVertexAttribArray( SHADER_UV_ATTR_LOCATION );
    }

    glGetIntegerv( GL_CURRENT_PROGRAM, &currentProgram );
    assert( currentProgram != 0 );
    colorUniformLocation = glGetUniformLocation( currentProgram, "color" );
    assert( colorUniformLocation != -1 );
    glUniform4fv( colorUniformLocation, 1, &( color[0] ) );

    glDrawArrays( GL_POINTS, 0, vertexData_.vertices.size() );

    if( includesTexture_ ){
        glEnableVertexAttribArray( SHADER_UV_ATTR_LOCATION );
    }
}


bool Mesh::containsProperty( const void* property ) const
{
    for( auto material : materials_ ){
        if( material.get() == property ){
            return true;
        }
    }
    return false;
}

} // namespace como
