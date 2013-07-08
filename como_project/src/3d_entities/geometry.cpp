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

#include "geometry.hpp"

#include <X11/X.h>
#include <GL/gl.h>
#include <GL/glx.h>
#include "camera.hpp"

// Initialize the location of the uniform shader variable "color" as unitialized (-1).
GLint Geometry::uniformColorLocation = -1;


/***
 * 1. Initialization and destruction
 ***/

Geometry::Geometry()
{   
    GLint currentShaderProgram;

    // Generate a VAO for the geometry and bind it as the active one.
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
    setInnerColor( 0.5f, 0.0f, 0.0f, 1.0f );
    setContourColor( 1.0f, 0.0f, 0.0f, 1.0f );
}


Geometry::~Geometry()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteBuffers( 1, &vbo );
    glDeleteBuffers( 1, &ebo );
    glDeleteVertexArrays( 1, &vao );
}


/***
 * 2. Getters and setters
 ***/
void Geometry::setInnerColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a )
{
    innerColor[0] = r;
    innerColor[1] = g;
    innerColor[2] = b;
    innerColor[3] = a;
}


void Geometry::setContourColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a )
{
    contourColor[0] = r;
    contourColor[1] = g;
    contourColor[2] = b;
    contourColor[3] = a;
}


/***
 * 3. Update and drawing.
 ***/

void Geometry::update()
{
    GLfloat* transformedVertices = NULL;
    glm::vec4 transformedVertex;

    // Map the OpenGL's VBO for transformed vertices to client memory, so we can update it.
    transformedVertices = (GLfloat*)glMapBuffer( GL_ARRAY_BUFFER, GL_WRITE_ONLY );

    // Recompute each transformed vertex by multiplying its corresponding original vertex
    // by transformation matrix.
    for( GLuint i = 0; i<originalVertices.size(); i++ ){
        transformedVertex = transformationMatrix * glm::vec4( originalVertices[i], 1.0f );

        transformedVertices[i*3+X] = transformedVertex.x;
        transformedVertices[i*3+Y] = transformedVertex.y;
        transformedVertices[i*3+Z] = transformedVertex.z;
    }

    // We finished updating the VBO, unmap it so OpenGL can take control over it.
    glUnmapBuffer( GL_ARRAY_BUFFER );
}


void Geometry::draw( Camera* camera ) const
{
    // Set shader modelview matrix.
    camera->setShaderModelviewMatrix( &transformationMatrix );

    // Feed uniform shader variable "color" with geometry inner color.
    // I fallen in a common mistake D:.
    // http://www.opengl.org/wiki/GLSL_:_common_mistakes
    // The problem is that for count, you set it to 4 while it should be 1 because you
    // are sending 1 vec4​ to the shader. The count is the number of that type
    // (4f, which corresponds to vec4​) that you are setting.
    glUniform4fv( uniformColorLocation, 1, innerColor );

    // Bind Geometry VAO as the active one.
    glBindVertexArray( vao );

    // Draw geometry interior.
    glDrawElements( GL_TRIANGLES, nInnerElements, GL_UNSIGNED_BYTE, NULL );

    // Feed uniform shader variable "color" with geometry contour color.
    glUniform4fv( uniformColorLocation, 1, contourColor );

    // Draw geometry contour.
    // I fallen in another error D: (I was using glDrawElementsBaseIndex).
    // http://stackoverflow.com/questions/9431923/using-an-offset-with-vbos-in-opengl
    glDrawElements( GL_LINES, nContourElements, GL_UNSIGNED_BYTE, (GLvoid*)( sizeof( GLubyte )*nInnerElements ) );
}



