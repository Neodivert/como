/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "mesh.hpp"
#include <glm/gtx/intersect.hpp>

namespace como {

// Initialize the location of the uniform shader variable "color" as unitialized (-1).
GLint Mesh::uniformColorLocation = -1;


/***
 * 1. Initialization and destruction
 ***/

Mesh::Mesh( const std::uint8_t* color )
{   
    checkOpenGL( "Mesh constructor - 1" );
    GLint currentShaderProgram;

    // Generate a VAO for the Mesh and bind it as the active one.
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    checkOpenGL( "Mesh constructor - 2" );

    // Generate a VBO and bind it for holding vertex data.
    glGenBuffers( 1, &vbo );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    checkOpenGL( "Mesh constructor - 3" );

    // Generate an EBO and bind it for holding vertex indices.
    glGenBuffers( 1, &ebo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    checkOpenGL( "Mesh constructor - 4" );

    if( uniformColorLocation == -1 ){
        checkOpenGL( "Mesh constructor - 4 b1" );
        // Location of uniform shader variable "color" hasn't been initialized yet.

        // Get current shader program id.
        glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );

        checkOpenGL( "Mesh constructor - 4 b2" );

        // Get location of uniform shader variable "color".
        uniformColorLocation = glGetUniformLocation( currentShaderProgram, "color" );

        checkOpenGL( "Mesh constructor - 4 b3" );
    }

    checkOpenGL( "Mesh constructor - 5" );
    // Set the mesh's color.
    if( color ){
        setColor( color[0]/255.0f, color[1]/255.0f, color[2]/255.0f, 1.0f );
    }else{
        setColor( (100+rand()%100)/(float)255, (100+rand()%100)/(float)255, (100+rand()%100)/(float)255, 1.0f );
    }

    checkOpenGL( "Mesh constructor - 6" );
    // Set both original and transformed centroids.
    originalCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
    transformedCentroid = glm::vec4( 0.0f, 0.0f, 0.0f, 1.0f );
}


Mesh::~Mesh()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &vbo );
    glDeleteBuffers( 1, &ebo );
    glDeleteVertexArrays( 1, &vao );
}


void Mesh::setVertices( const GLuint nVertices, const GLfloat* vertices )
{
    checkOpenGL( "Mesh constructor - Setting vertices" );
    // Copy given vertices to this mesh's original vertices. Also compute
    // the mesh's centroid.
    originalVertices.resize( nVertices );
    for( GLuint i=0; i<nVertices; i++ )
    {
        originalVertices[i] = glm::vec3( vertices[i*COMPONENTS_PER_VERTEX+X],
                                         vertices[i*COMPONENTS_PER_VERTEX+Y],
                                         vertices[i*COMPONENTS_PER_VERTEX+Z] );

        originalCentroid += glm::vec4( originalVertices[i], 1.0f );
    }
    originalCentroid /= originalVertices.size();
    originalCentroid.w = 1.0f;

    checkOpenGL( "Mesh constructor, before setting VBO vertex data" );

    // Allocate a VBO for transformed vertices.
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBufferData( GL_ARRAY_BUFFER, nVertices*COMPONENTS_PER_VERTEX*sizeof( GLfloat ), NULL, GL_DYNAMIC_DRAW );

    checkOpenGL( "Mesh constructor, after setting VBO vertex data" );

    // Get the location of the input variable "vPosition" for the current shader program.
    GLint prog;
    glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
    GLint vPosition = glGetAttribLocation( prog, "vPosition" );

    // By using the previous "vPosition" position, specify the location and data format of
    // the array of vertex positions.
    glBindVertexArray( vao );
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    // Enable previous array of vertex positions.
    glEnableVertexAttribArray( vPosition );

    // Update transformed vertices.
    update();
}


void Mesh::setElements( const GLuint nElements, const GLubyte* elements )
{
    // Compute the number of triangles for this mesh.
    const GLuint nTriangles = nElements / N_TRIANGLE_VERTICES;

    // Copy the number of elements (indices).
    this->nElements = nElements;

    // Copy original triangles to this geometry's triangles.
    triangles.resize( nTriangles );
    for( GLuint i = 0; i<nTriangles; i++ ){
        triangles[i][0] = elements[i*N_TRIANGLE_VERTICES];
        triangles[i][1] = elements[i*N_TRIANGLE_VERTICES+1];
        triangles[i][2] = elements[i*N_TRIANGLE_VERTICES+2];
    }

    // Copy the mesh's elements to a EBO.
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, nElements*sizeof( GLubyte ), elements, GL_STATIC_DRAW );
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
                                       glm::vec3( transformedVertices[triangles[i][0]*3+X], transformedVertices[triangles[i][0]*3+Y], transformedVertices[triangles[i][0]*3+Z] ),
                                       glm::vec3( transformedVertices[triangles[i][1]*3+X], transformedVertices[triangles[i][1]*3+Y], transformedVertices[triangles[i][1]*3+Z] ),
                                       glm::vec3( transformedVertices[triangles[i][2]*3+X], transformedVertices[triangles[i][2]*3+Y], transformedVertices[triangles[i][2]*3+Z] ),
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
    GLfloat* transformedVertices = nullptr;
    glm::vec4 transformedVertex;

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
    transformedVertices = (GLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    checkOpenGL( "Mesh::update() - 4" );

    // Recompute each transformed vertex by multiplying its corresponding original vertex
    // by transformation matrix.
    for( GLuint i = 0; i<originalVertices.size(); i++ ){
        transformedVertex = transformationMatrix * glm::vec4( originalVertices[i], 1.0f );

        transformedVertices[i*N_TRIANGLE_VERTICES+X] = transformedVertex.x;
        transformedVertices[i*N_TRIANGLE_VERTICES+Y] = transformedVertex.y;
        transformedVertices[i*N_TRIANGLE_VERTICES+Z] = transformedVertex.z;
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
