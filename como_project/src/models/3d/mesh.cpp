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

#include <X11/X.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "camera.hpp"
#include <glm/gtx/intersect.hpp>

namespace como {

// Initialize the location of the uniform shader variable "color" as unitialized (-1).
GLint Mesh::uniformColorLocation = -1;


/***
 * 1. Initialization and destruction
 ***/

Mesh::Mesh()
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
        uniformColorLocation = glGetUniformLocation( currentShaderProgram, "color" );

        cout << "uniform color location initialized to (" << uniformColorLocation << ")" << endl;
    }

    // Set both inner and contour colors.
    //setInnerColor( 0.5f, 0.0f, 0.0f, 1.0f );
    setInnerColor( (100+rand()%100)/(float)255, (100+rand()%100)/(float)255, (100+rand()%100)/(float)255, 1.0f );

    setContourColor( 1.0f, 0.0f, 0.0f, 1.0f );
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
    // Copy given vertices to this mesh's original vertices.
    originalVertices.resize( nVertices );
    for( GLuint i=0; i<nVertices; i++ )
    {
        originalVertices[i] = glm::vec3( vertices[i*COMPONENTS_PER_VERTEX+X],
                                         vertices[i*COMPONENTS_PER_VERTEX+Y],
                                         vertices[i*COMPONENTS_PER_VERTEX+Z] );
    }
    // Allocate a VBO for transformed vertices.
    glBufferData( GL_ARRAY_BUFFER, nVertices*COMPONENTS_PER_VERTEX*sizeof( GLfloat ), NULL, GL_DYNAMIC_DRAW );

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
void Mesh::setInnerColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a )
{
    innerColor[0] = r;
    innerColor[1] = g;
    innerColor[2] = b;
    innerColor[3] = a;
}


void Mesh::setContourColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a )
{
    contourColor[0] = r;
    contourColor[1] = g;
    contourColor[2] = b;
    contourColor[3] = a;
}


/***
 * 3. Intersections
 ***/

void Mesh::intersects( glm::vec3 rayOrigin, glm::vec3 rayDirection, float& minT, unsigned int* triangle ) const
{
    const float MAX_T = 999999.9f;
    float t = MAX_T;

    minT = MAX_T;

    glm::vec3 intersection;

    // Normalize the direction of the ray.
    rayDirection = glm::normalize( rayDirection );

    // Get a transformation matrix from world coordinates to objet
    // coordinates (of this Mesh).
    glm::mat4 worldToObjetTransform = glm::inverse( transformationMatrix );

    // Transform ray from world to object coordinates.
    rayOrigin = glm::vec3( worldToObjetTransform * glm::vec4( rayOrigin, 1.0f ) );
    rayDirection = glm::normalize( glm::vec3( worldToObjetTransform * glm::vec4( rayDirection, 1.0f ) ) );

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
            if( ( t >= 0.0f ) && ( t <= 1.0f ) && ( t < minT ) ){
                minT = t;
                if( triangle != nullptr ){
                    *triangle = i;
                }
            }
        }
    }

    if( minT >= MAX_T ){
        minT = -1.0f;
    }
}

/***
 * 4. Update and drawing.
 ***/

void Mesh::update()
{
    GLfloat* transformedVertices = nullptr;
    glm::vec4 transformedVertex;

    // Update mesh's orientation.
    Drawable::update();

    // Update mesh's centroid.
    transformedCentroid = transformationMatrix * originalCentroid;

    // Set Mesh's VAO and VBO as the current ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );

    // Map the OpenGL's VBO for transformed vertices to client memory, so we can update it.
    transformedVertices = (GLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    // Recompute each transformed vertex by multiplying its corresponding original vertex
    // by transformation matrix.
    for( GLuint i = 0; i<originalVertices.size(); i++ ){
        transformedVertex = transformationMatrix * glm::vec4( originalVertices[i], 1.0f );

        transformedVertices[i*N_TRIANGLE_VERTICES+X] = transformedVertex.x;
        transformedVertices[i*N_TRIANGLE_VERTICES+Y] = transformedVertex.y;
        transformedVertices[i*N_TRIANGLE_VERTICES+Z] = transformedVertex.z;
    }
    // We finished updating the VBO, unmap it so OpenGL can take control over it.
    glUnmapBuffer( GL_ARRAY_BUFFER );
}

void Mesh::draw( const glm::mat4& viewProjectionMatrix, bool selected ) const
{
    // Default color for selected drawables' contour.
    GLfloat selectecContourColor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // Set ModelViewProjection matrix to shader.
    sendMvpMatrixToShader( viewProjectionMatrix * transformationMatrix );

    // Feed uniform shader variable "color" with Mesh inner color.
    // I fallen in a common mistake D:.
    // http://www.opengl.org/wiki/GLSL_:_common_mistakes
    // The problem is that for count, you set it to 4 while it should be 1 because you
    // are sending 1 vec4​ to the shader. The count is the number of that type
    // (4f, which corresponds to vec4​) that you are setting.
    glUniform4fv( uniformColorLocation, 1, innerColor );

    // Bind Mesh VAO and VBOs as the active ones.
    glBindVertexArray( vao );
    glBindBuffer( GL_ARRAY_BUFFER, vbo );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebo );

    // Draw Mesh's interior.
    glDrawElements( GL_TRIANGLES, nElements, GL_UNSIGNED_BYTE, NULL );

    // Feed uniform shader variable "color" with one color or another
    // depends on whether current Mesh is selected by user or not.
    if( selected ){
        // Mesh selected by user. Set a default color.
        glUniform4fv( uniformColorLocation, 1, selectecContourColor );
    }else{
        // Mesh not selected by user. Set Mesh's own contour color.
        glUniform4fv( uniformColorLocation, 1, contourColor );
    }

    // Now we'll draw mesh's contour. Set polygon mode for rendering
    // lines.
    // TODO: I still have to use polygon offset.
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // Draw Mesh's contour
    glDrawElements( GL_TRIANGLES, nElements, GL_UNSIGNED_BYTE, NULL );

    // Return polygon mode to previos GL_FILL.
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

} // namespace como
