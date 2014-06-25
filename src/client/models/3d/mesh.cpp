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

// TODO: Replace meshColor argument by a MaterialConstPtr.
Mesh::Mesh( MeshType type, const char* fileName, MaterialConstPtr material ) :
    Drawable( DrawableType::MESH, "Mesh #" ),
    type_( type ),
    material_( material )
{
    loadFromOBJ( fileName );
}


Mesh::Mesh( MaterialConstPtr material ) :
    Drawable( DrawableType::MESH, "Mesh #" ),
    type_( MeshType::MESH ),
    material_( material )
{}


Mesh::Mesh( const Mesh& b ) :
    Drawable( b ),
    type_( b.type_ ),
    originalVertices( b.originalVertices ),
    originalNormals( b.originalNormals ),
    triangles( b.triangles ),
    originalCentroid( b.originalCentroid ),
    transformedCentroid( b.transformedCentroid ),
    material_( b.material_ )
{
    // Initialize and populate Mesh's vertex data.
    initMeshBuffers();
    initVertexData();
}


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


void Mesh::initVertexData()
{
    unsigned int i;

    // Initialize VAO and VBO.
    initVBO();
    initVAO();

    // Copy the mesh's elements to a EBO.
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, triangles.size()*3*sizeof( GLuint ), nullptr, GL_STATIC_DRAW );

    std::cout << "triangles: " << triangles.size() << std::endl;
    for( i=0; i<triangles.size(); i++ ){
        glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 3*i*sizeof( GLuint ), 3*sizeof( GLuint ), &triangles[i] );
    }

    // Copy the vertex data to VBO.
    for( GLuint i = 0; i<originalVertices.size(); i++ ){
        setVertexData( i );
    }

    // Update transformed vertices.
    update();
}


void Mesh::initVBO()
{
    // Allocate a VBO for transformed vertices.
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, originalVertices.size()*this->getBytesPerVertex(), NULL, GL_STATIC_DRAW );

    std::cout << "VBO (" << originalVertices.size() << " * " << this->getBytesPerVertex() << ")" << std::endl;
}


void Mesh::initVAO()
{
    // Set the organization of the vertex and normals data in the VBO.
    glBindVertexArray( vao );
    glVertexAttribPointer( SHADER_VERTEX_ATTR_LOCATION, 3, GL_FLOAT, GL_FALSE, getBytesPerVertex(), (void *)( 0 ) );
    glVertexAttribPointer( SHADER_NORMAL_ATTR_LOCATION, 3, GL_FLOAT, GL_FALSE, getBytesPerVertex(), (void *)( COMPONENTS_PER_VERTEX_POSITION * sizeof( GL_FLOAT ) ) );

    // Enable previous vertex data arrays.
    glEnableVertexAttribArray( SHADER_VERTEX_ATTR_LOCATION );
    glEnableVertexAttribArray( SHADER_NORMAL_ATTR_LOCATION );
}


unsigned int Mesh::getBytesPerVertex() const
{
    return getComponentsPerVertex() * sizeof( GL_FLOAT );
}


unsigned int Mesh::getComponentsPerVertex() const
{
    // Vertex coordinates (3 components) + normal coordinates (3 components).
    return 6;
}


void Mesh::setVertexData( GLint index )
{
    GLfloat vertexData[] =
    {
        // Vertex coordinates
        originalVertices[index].x,
        originalVertices[index].y,
        originalVertices[index].z,

        // Normal coordinates
        originalNormals[index].x,
        originalNormals[index].y,
        originalNormals[index].z
    };

    std::cout << "Mesh::setVertexData - Bytes per vertex [" << getBytesPerVertex() << "]" << std::endl;
    glBufferSubData( GL_ARRAY_BUFFER, index * getBytesPerVertex(), getBytesPerVertex(), vertexData );

    /*

    // Copy vertex position to VBO.
    vbo[index*COMPONENTS_PER_VERTEX+X] = originalVertices[index].x;
    vbo[index*COMPONENTS_PER_VERTEX+Y] = originalVertices[index].y;
    vbo[index*COMPONENTS_PER_VERTEX+Z] = originalVertices[index].z;

    // Copy vertex normal to VBO.
    vbo[index*COMPONENTS_PER_VERTEX+COMPONENTS_PER_VERTEX_POSITION+X] = originalNormals[index].x;
    vbo[index*COMPONENTS_PER_VERTEX+COMPONENTS_PER_VERTEX_POSITION+Y] = originalNormals[index].y;
    vbo[index*COMPONENTS_PER_VERTEX+COMPONENTS_PER_VERTEX_POSITION+Z] = originalNormals[index].z;
    */
}


void Mesh::computeCentroid()
{
    unsigned int i = 0;

    originalCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    for( i = 0; i < originalVertices.size(); i++ ){
        originalCentroid += glm::vec4( originalVertices[i], 1.0f );
    }

    originalCentroid /= originalVertices.size();
    originalCentroid.w = 1.0f;
}


void Mesh::computeVertexNormals()
{
    unsigned int normalIndex, faceIndex;

    // Allocate space for holding one normal per vertex.
    originalNormals.resize( originalVertices.size() );

    // Compute every vertex's normal.
    for( normalIndex = 0; normalIndex < originalNormals.size(); normalIndex++ ){
        originalNormals[normalIndex] = glm::vec3( 0.0f );
        for( faceIndex = 0; faceIndex < triangles.size(); faceIndex++ ){
            if( ( triangles[faceIndex][0] == normalIndex ) ||
                    ( triangles[faceIndex][1] == normalIndex ) ||
                    ( triangles[faceIndex][2] == normalIndex ) ){

                originalNormals[normalIndex] += glm::cross( originalVertices[ triangles[faceIndex][2] ] - originalVertices[ triangles[faceIndex][0] ], originalVertices[ triangles[faceIndex][1] ] - originalVertices[ triangles[faceIndex][0] ] );
            }
        }
        // TODO: Do I have to divide or something first?
        originalNormals[normalIndex] = glm::normalize( originalNormals[normalIndex] );
    }
}


/***
 * 4. File loading.
 ***/

void Mesh::loadFromOBJ( const char* filePath )
{
    // Initialize OpenGL objects (VBO, VAO, EBO, ...) associated to this Mesh.
    initMeshBuffers();

    std::ifstream file;
    std::string line;

    // Initialize original vertex data.
    originalVertices.clear();
    triangles.clear();
    originalCentroid = glm::vec4( 0.0f );

    // Try to open the OBJ file.
    file.open( filePath );

    if( !file.is_open() ){
        throw std::runtime_error( std::string( "ERROR: File [" ) + std::string( filePath ) + std::string( "] not found" ) );
    }

    // Read vertex data from file.
    // TODO: Accept OBJ files with multiple objects inside.
    while( std::getline( file, line ) ){
        std::cout << "line: [" << line << "]: " << this->processFileLine( line ) << std::endl;
    }

    // Close the input file and finish initializing the mesh.
    file.close();

    // Compute Mesh's centroid and vertex normals.
    computeCentroid();
    computeVertexNormals();

    // Initialize Mesh transformed vertex data.
    initVertexData();
}


bool Mesh::processFileLine( const string &line )
{
    glm::vec3 vertex;
    std::array< GLuint, 3 > triangle;
    unsigned int i;
    unsigned int componentsRead = 0;

    if( line[0] == 'v' ){
        // Set '.' as the float separator (for parsing floats from a text
        // line).
        setlocale( LC_NUMERIC, "C" );

        // Extract the vertex from the line and add it to the Mesh.
        sscanf( line.c_str(), "v %f %f %f", &vertex[0], &vertex[1], &vertex[2] );

        // Resize the vertex coordinates.
        vertex *= 0.5f;

        originalVertices.push_back( vertex );

    }else if( line[0] == 'f' ){
        // Extract the face from the line and add it to the Mesh.
        componentsRead = sscanf( line.c_str(), "f %u %u %u", &triangle[0], &triangle[1], &triangle[2] );

        if( componentsRead != 3 ){
            throw std::runtime_error( "Error reading mesh triangle (Is it a textured mesh?)" );
        }

        for( i=0; i<3; i++ ){
            // Decrement every vertex index because they are 1-based in the .obj file.
            triangle[i] -= 1;
        }

        triangles.push_back( triangle );
    }else{
        return false;
    }
    // TODO: And the normals?

    return true;
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


void Mesh::getVertexData( unsigned int& n, GLfloat* vertices )
{
    GLfloat* mappedVBO = nullptr;

    // Get the number of vertices of the mesh.
    n = originalVertices.size();

    // Bind the mesh's VBO
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Map the VBO to client memory.
    mappedVBO = (GLfloat *)glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY );

    // Copy the VBO data to the pointer given as an argument.
    memcpy( vertices, mappedVBO, n * COMPONENTS_PER_VERTEX * sizeof( GLfloat ) );

    // Unmap the VBO.
    glUnmapBuffer( GL_ARRAY_BUFFER );
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
    for( unsigned int i = 0; i < triangles.size(); i++ ){
        if( glm::intersectRayTriangle( rayOrigin,
                                       rayDirection,
                                       originalVertices[triangles[i][0]],
                                       originalVertices[triangles[i][1]],
                                       originalVertices[triangles[i][2]],
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
    // Compute MVP matrix and pass it to the shader.
    sendMVPMatrixToShader( viewProjMatrix * transformationMatrix );

    // Send this mesh's material to shader.
    material_->sendToShader();

    // Bind Mesh VAO and VBOs as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    // Draw Mesh's interior.
    glDrawElements( GL_TRIANGLES, triangles.size()*3, GL_UNSIGNED_INT, NULL );

    // Set the color for the mesh's contour.
    if( contourColor != nullptr ){
        openGL->disableLighting();

        glUniform4fv( uniformColorLocation, 1, contourColor );

        // Now we'll draw mesh's contour. Set polygon mode for rendering
        // lines.
        // TODO: I still have to use polygon offset.
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // Draw Mesh's contour
        glDrawElements( GL_TRIANGLES, triangles.size()*3, GL_UNSIGNED_INT, NULL );

        // Return polygon mode to previos GL_FILL.
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

        openGL->enableLighting();
    }
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
