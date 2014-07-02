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
#include <common/primitives/primitive_file.hpp>

// TODO: Remove this once the constants PRIMITIVES_*_DIR be moved to
// another place.
#include <common/utilities/ids.hpp>


#include <iostream>

namespace como {

// Initialize the location of various uniform shader variables as unitialized (-1).
GLint Mesh::uniformColorLocation = -1;
GLint Mesh::mvpMatrixLocation_ = -1;
GLint Mesh::normalMatrixLocation_ = -1;

const GLint SHADER_VERTEX_ATTR_LOCATION = 0;
const GLint SHADER_NORMAL_ATTR_LOCATION = 1;



/***
 * 1. Construction.
 ***/

Mesh::Mesh( MeshType type, const char* filePath, MaterialConstPtr material ) :
    Drawable( DrawableType::MESH, "Mesh #" ),
    type_( type ),
    material_( material )
{
    MeshInfo meshInfo;
    PrimitiveFile::read( meshInfo, filePath );

    vertexData_ = meshInfo.vertexData;

    init( meshInfo.oglData );
}


Mesh::Mesh( MeshVertexData vertexData, const MeshOpenGLData& oglData, MaterialConstPtr material ) :
    Drawable( DrawableType::MESH, "Mesh #" ),
    type_( MeshType::MESH ),
    vertexData_( vertexData ),
    material_( material )
{
    init( oglData );
}

/*
Mesh::Mesh( const Mesh& b ) :
    Drawable( b ),
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

DrawablePtr Mesh::clone()
{
    return DrawablePtr( new Mesh( *this ) );
}


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
    OpenGL::checkStatus( "Mesh::init - 1" );
    genOpenGLBuffers();
    OpenGL::checkStatus( "Mesh::init - 2" );

    initShaderLocations();
    OpenGL::checkStatus( "Mesh::init - 3" );

    populateOpenGLBuffers( oglData );
    OpenGL::checkStatus( "Mesh::init - 4" );

    initVAO();
    OpenGL::checkStatus( "Mesh::init - 5" );

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

        // Get location of uniform shader variable "color".
        uniformColorLocation = glGetUniformLocation( currentShaderProgram, "material.color" );

        // Get location of uniform shader variable "mvpMatrix".
        mvpMatrixLocation_ = glGetUniformLocation( currentShaderProgram, "mvpMatrix" );

        // Get location of uniform shader variable "normalMatrix".
        normalMatrixLocation_ = glGetUniformLocation( currentShaderProgram, "normalMatrix" );
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
}

unsigned int Mesh::getOwnBytesPerVertex() const
{
    return getOwnComponentsPerVertex() * sizeof( GL_FLOAT );
}

unsigned int Mesh::getOwnComponentsPerVertex() const
{
    // Vertex coordinates (3 components) + normal coordinates (3 components).
    return 6;
}


unsigned int Mesh::getBytesPerVertex() const
{
    return getComponentsPerVertex() * sizeof( GL_FLOAT );
}


unsigned int Mesh::getComponentsPerVertex() const
{
    return getOwnComponentsPerVertex();
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


glm::vec4 Mesh::getCentroid() const
{
    return transformedCentroid;
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
    rayOrigin = glm::vec3( glm::inverse( transformationMatrix ) * glm::vec4( rayOrigin, 1.0f ) );
    rayDirection = glm::vec3( glm::inverse( transformationMatrix ) * glm::vec4( rayDirection, 0.0f ) );

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
    Drawable::update();

    // Update mesh's centroid.
    transformedCentroid = transformationMatrix * originalCentroid;
}


void Mesh::draw( OpenGLPtr openGL, const glm::mat4& viewProjMatrix, const GLfloat* contourColor ) const
{
    OpenGL::checkStatus( "Mesh::draw - begin" );

    // Compute MVP matrix and pass it to the shader.
    sendMVPMatrixToShader( viewProjMatrix * transformationMatrix );

    // Send this mesh's material to shader.
    material_->sendToShader();

    // Bind Mesh VAO and VBOs as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    // Draw Mesh's interior.
    glDrawElements( GL_TRIANGLES, nEboElements_, GL_UNSIGNED_INT, NULL );


    // Set the color for the mesh's contour.
    if( contourColor != nullptr ){
        openGL->disableLighting();

        glUniform4fv( uniformColorLocation, 1, contourColor );

        // Now we'll draw mesh's contour. Set polygon mode for rendering
        // lines.
        // TODO: I still have to use polygon offset.
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // Draw Mesh's contour
        glDrawElements( GL_TRIANGLES, nEboElements_, GL_UNSIGNED_INT, NULL );

        // Return polygon mode to previos GL_FILL.
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        openGL->enableLighting();
    }

    OpenGL::checkStatus( "Mesh::draw - end" );
}

/***
 * 9. Auxliar methods.
 ***/

void Mesh::sendMVPMatrixToShader( const glm::mat4& mvpMatrix )
{
    // Send the given MVP matrix to shader.
    glUniformMatrix4fv( mvpMatrixLocation_, 1, GL_FALSE, &mvpMatrix[0][0] );

    // Compute normal matrix and send it to shader.
    glm::mat3 normalMatrix = glm::mat3( glm::transpose( glm::inverse( mvpMatrix ) ) );
    glUniformMatrix3fv( normalMatrixLocation_, 1, GL_FALSE, &normalMatrix[0][0] );
}

} // namespace como
