/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
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
#include <glm/gtx/intersect.hpp>

namespace como {

// Initialize the location of the uniform shader variable "color" as unitialized (-1).
GLint Mesh::uniformColorLocation = -1;

const GLint SHADER_VERTEX_ATTR_LOCATION = 0;
const GLint SHADER_NORMAL_ATTR_LOCATION = 1;


/***
 * 1. Initialization and destruction
 ***/

Mesh::Mesh( const char* filePath, const std::uint8_t* color )
{   
    // Initialize OpenGL objects (VBO, VAO, EBO, ...) associated to this Mesh.
    initMeshBuffers();

    // Set the mesh's color.
    if( color ){
        setColor( color[0] / 255.0f, color[1] / 255.0f, color[2] / 255.0f, 1.0f );
    }else{
        setColor( (100+rand()%100)/(float)255, (100+rand()%100)/(float)255, (100+rand()%100)/(float)255, 1.0f );
    }

    // Load vertex data from given file.
    LoadFromOBJ( filePath );
}


Mesh::~Mesh()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &vbo );
    glDeleteBuffers( 1, &ebo );
    glDeleteVertexArrays( 1, &vao );
}


void Mesh::initMeshBuffers()
{
    //checkOpenGL( "Mesh constructor - 1" );
    GLint currentShaderProgram;

    // Generate a VAO for the Mesh and bind it as the active one.
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    //checkOpenGL( "Mesh constructor - 2" );

    // Generate a VBO and bind it for holding vertex data.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    //checkOpenGL( "Mesh constructor - 3" );

    // Generate an EBO and bind it for holding vertex indices.
    glGenBuffers( 1, &ebo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    //checkOpenGL( "Mesh constructor - 4" );

    if( uniformColorLocation == -1 ){
        checkOpenGL( "Mesh constructor - 4 b1" );
        // Location of uniform shader variable "color" hasn't been initialized yet.

        // Get current shader program id.
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

        //checkOpenGL( "Mesh constructor - 4 b2" );

        // Get location of uniform shader variable "color".
        uniformColorLocation = glGetUniformLocation( currentShaderProgram, "color" );

        //checkOpenGL( "Mesh constructor - 4 b3" );
    }

    //checkOpenGL( "Mesh constructor - 5" );

    //checkOpenGL( "Mesh constructor - 6" );
    // Set both original and transformed centroids.
    originalCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    transformedCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
}


void Mesh::initTransformedVertexData()
{
    unsigned int i;

    // Allocate a VBO for transformed vertices.
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, originalVertices.size()*COMPONENTS_PER_VERTEX*sizeof( GLfloat ), NULL, GL_DYNAMIC_DRAW );

    checkOpenGL( "Mesh constructor, after setting VBO vertex data" );

    // Set the organization of the vertex and normals data in the VBO.
    checkOpenGL( "Mesh::setVertices() - 1" );
    glBindVertexArray( vao );
    glVertexAttribPointer( SHADER_VERTEX_ATTR_LOCATION, 3, GL_FLOAT, GL_FALSE, COMPONENTS_PER_VERTEX*sizeof( GL_FLOAT ), (void *)( 0 ) );
    checkOpenGL( "Mesh::setVertices() - 2" );
    glVertexAttribPointer( SHADER_NORMAL_ATTR_LOCATION, 3, GL_FLOAT, GL_FALSE, COMPONENTS_PER_VERTEX*sizeof( GL_FLOAT ), (void *)( COMPONENTS_PER_VERTEX_POSITION * sizeof( GL_FLOAT ) ) );

    checkOpenGL( "Mesh::setVertices() - 3" );
    // Enable previous vertex data arrays.
    glEnableVertexAttribArray( SHADER_VERTEX_ATTR_LOCATION );
    checkOpenGL( "Mesh::setVertices() - 4" );
    glEnableVertexAttribArray( SHADER_NORMAL_ATTR_LOCATION );

    // Copy the mesh's elements to a EBO.
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, nElements*sizeof( GLubyte ), nullptr, GL_STATIC_DRAW );

    for( i=0; i<triangles.size(); i++ ){
        glBufferSubData( GL_ELEMENT_ARRAY_BUFFER, 3*i*sizeof( GLubyte ), 3*sizeof( GLubyte ), &triangles[i] );
    }

    // Compute vertex normals.
    // TODO: This requires setVertices() to be called before setElements().
    // Make both methods private and implement a public setVertexData() which
    // calls them in order.
    computeVertexNormals();

    // Update transformed vertices.
    update();
}


void Mesh::computeVertexNormals()
{
    unsigned int normalIndex, faceIndex;

    // Allocate space for holding one normal per vertex.
    originalNormals.resize( originalVertices.size() );

    std::cout << "originalNormals.size(): " << originalNormals.size() << std::endl;

    // Compute every vertex's normal.
    for( normalIndex = 0; normalIndex < originalNormals.size(); normalIndex++ ){
        originalNormals[normalIndex] = glm::vec3( 0.0f );
        for( faceIndex = 0; faceIndex < triangles.size(); faceIndex++ ){
            if( ( triangles[faceIndex][0] == normalIndex ) ||
                    ( triangles[faceIndex][1] == normalIndex ) ||
                    ( triangles[faceIndex][2] == normalIndex ) ){

                // TODO: Is it necessary to normalize here?
                originalNormals[normalIndex] += glm::normalize( glm::cross( originalVertices[ triangles[faceIndex][2] ] - originalVertices[ triangles[faceIndex][0] ], originalVertices[ triangles[faceIndex][1] ] - originalVertices[ triangles[faceIndex][0] ] ) );
            }
        }
        originalNormals[normalIndex] = glm::normalize( originalNormals[normalIndex] );
    }
}


/***
 * 2. Loading
 ***/

void Mesh::LoadFromOBJ( const char* filePath )
{
    const unsigned int LINE_SIZE = 250;
    std::ifstream file;
    char line[LINE_SIZE];
    glm::vec3 vertex;
    std::array< GLubyte, 3 > triangle;

    // Initialize original vertex data.
    originalVertices.clear();
    triangles.clear();
    originalCentroid = glm::vec4( 0.0f );

    // Try to open the OBJ file.
    file.open( filePath );

    if( !file.is_open() ){
        throw std::runtime_error( "ERROR: File not found" );
    }

    // Read vertex data from file.
    // TODO: Accept OBJ files with multiple objects inside.
    while( file.getline( line, LINE_SIZE ) ){
        if( line[0] == 'v' ){
            // Extract the vertex from the line and add it to the Mesh.
            sscanf( line, "v %f %f %f", &vertex[0], &vertex[1], &vertex[2] );
            originalVertices.push_back( vertex );
        }else if( line[0] == 'f' ){
            // Extract the face from the line and add it to the Mesh.
            sscanf( line, "f %c %c %c", &triangle[0], &triangle[1], &triangle[2] );
            triangles.push_back( triangle );
        }
    }

    // Close the input file and finish initializing the mesh.
    file.close();

    // Initialize Mesh transformed vertex data.
    initTransformedVertexData();
}


/***
 * 2. Getters and setters
 ***/

void Mesh::setColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a )
{
    color[0] = r;
    color[1] = g;
    color[2] = b;
    color[3] = a;
}


glm::vec4 Mesh::getCentroid() const
{
    return transformedCentroid;
}


void Mesh::getTransformedVertices( unsigned int& n, GLfloat* vertices )
{
    std::cout << "Mesh::getTransformedVertices" << std::endl;

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
 * 3. Transformations
 ***/

/*
void Mesh::rotateAroundCentroid( const GLfloat& angle, const glm::vec3& axis )
{
    // Set the transformed centroid as the pivot point and rotate the mesh.
    rotate( angle, axis, glm::vec3( transformedCentroid ) );
}
*/

/***
 * 4. Intersections
 ***/

void Mesh::intersects( glm::vec3 rayOrigin, glm::vec3 rayDirection, float& minT, unsigned int* triangle ) const
{
    const float MAX_T = 999999.9f;
    float t = MAX_T;
    glm::vec3 intersection;

    minT = MAX_T;

    // Normalize the direction of the ray.
    rayDirection = glm::normalize( rayDirection );

    // Set mesh's vbo as the active one.
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // The ray's origind and direction are in world space. We have to map the OpenGL's VBO with
    // the mesh's transformed vertices (world space) to client memory, so we can intersect them
    // (the ray and the mesh).
    GLfloat* transformedVertices = (GLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_READ_ONLY );

    // Compute intersections with all triangles in this Mesh.
    for( unsigned int i = 0; i < triangles.size(); i++ ){
        if( glm::intersectRayTriangle( rayOrigin,
                                       rayDirection,
                                       glm::vec3( transformedVertices[triangles[i][0]*COMPONENTS_PER_VERTEX+X], transformedVertices[triangles[i][0]*COMPONENTS_PER_VERTEX+Y], transformedVertices[triangles[i][0]*COMPONENTS_PER_VERTEX+Z] ),
                                       glm::vec3( transformedVertices[triangles[i][1]*COMPONENTS_PER_VERTEX+X], transformedVertices[triangles[i][1]*COMPONENTS_PER_VERTEX+Y], transformedVertices[triangles[i][1]*COMPONENTS_PER_VERTEX+Z] ),
                                       glm::vec3( transformedVertices[triangles[i][2]*COMPONENTS_PER_VERTEX+X], transformedVertices[triangles[i][2]*COMPONENTS_PER_VERTEX+Y], transformedVertices[triangles[i][2]*COMPONENTS_PER_VERTEX+Z] ),
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
    // We finished updating the VBO, unmap it so OpenGL can take control over it.
    glUnmapBuffer( GL_ARRAY_BUFFER );

    // If the ray didn't intersect the mesh, we "return" -1.
    if( minT >= MAX_T ){
        minT = -1.0f;
    }
}

/***
 * 5. Update and drawing.
 ***/

void Mesh::update()
{
    GLfloat* vertexData = nullptr;
    glm::vec4 transformedVertex;
    glm::vec4 transformedNormal;

    checkOpenGL( "Mesh::update() - 1" );

    // Update mesh's orientation.
    Drawable::update();

    checkOpenGL( "Mesh::update() - 2" );

    // Update mesh's centroid.
    transformedCentroid = transformationMatrix * originalCentroid;

    //std::cout << "Transformed Centroid: (" << transformedCentroid.x << ", " << transformedCentroid.y << ", " << transformedCentroid.z << ", " << transformedCentroid.w << ")" << std::endl;

    // Set Mesh's VAO and VBO as the current ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    checkOpenGL( "Mesh::update() - 3" );
    // Map the OpenGL's VBO for transformed vertices to client memory, so we can update it.
    vertexData = (GLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    checkOpenGL( "Mesh::update() - 4" );

    // Recompute each transformed vertex and normal by multiplying their
    // corresponding original values by transformation matrix.
    for( GLuint i = 0; i<originalVertices.size(); i++ ){
        transformedVertex = transformationMatrix * glm::vec4( originalVertices[i], 1.0f );

        vertexData[i*COMPONENTS_PER_VERTEX+X] = transformedVertex.x;
        vertexData[i*COMPONENTS_PER_VERTEX+Y] = transformedVertex.y;
        vertexData[i*COMPONENTS_PER_VERTEX+Z] = transformedVertex.z;

        // TODO: w = 1.0f?
        transformedNormal = transformationMatrix * glm::vec4( originalNormals[i], 0.0f );

        vertexData[i*COMPONENTS_PER_VERTEX+COMPONENTS_PER_VERTEX_POSITION+X] = transformedNormal.x;
        vertexData[i*COMPONENTS_PER_VERTEX+COMPONENTS_PER_VERTEX_POSITION+Y] = transformedNormal.y;
        vertexData[i*COMPONENTS_PER_VERTEX+COMPONENTS_PER_VERTEX_POSITION+Z] = transformedNormal.z;
    }

    checkOpenGL( "Mesh::update() - 5" );
    // We finished updating the VBO, unmap it so OpenGL can take control over it.
    glUnmapBuffer( GL_ARRAY_BUFFER );

    checkOpenGL( "Mesh::update() - 6" );
}

void Mesh::draw( const GLfloat* contourColor ) const
{
    // Feed uniform shader variable "color" with Mesh inner color.
    // I fallen in a common mistake D:.
    // http://www.opengl.org/wiki/GLSL_:_common_mistakes
    // The problem is that for count, you set it to 4 while it should be 1 because you
    // are sending 1 vec4​ to the shader. The count is the number of that type
    // (4f, which corresponds to vec4​) that you are setting.
    glUniform4fv( uniformColorLocation, 1, color );

    // Bind Mesh VAO and VBOs as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    // Draw Mesh's interior.
    glDrawElements( GL_TRIANGLES, nElements, GL_UNSIGNED_BYTE, NULL );

    // Set the color for the mesh's contour.
    if( contourColor != nullptr ){
        glUniform4fv( uniformColorLocation, 1, contourColor );

        // Now we'll draw mesh's contour. Set polygon mode for rendering
        // lines.
        // TODO: I still have to use polygon offset.
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        // Draw Mesh's contour
        glDrawElements( GL_TRIANGLES, nElements, GL_UNSIGNED_BYTE, NULL );

        // Return polygon mode to previos GL_FILL.
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    }
}

} // namespace como
