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

#include "auxiliar_lines_renderer.hpp"
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/gtc/type_ptr.hpp>

namespace como {

/***
 * 1. Construction
 ***/

AuxiliarLinesRenderer::AuxiliarLinesRenderer( OpenGL &openGL, const Color& userColor ) :
    openGL_( &openGL ),
    colorShaderLocation_( openGL.getShaderVariableLocation( "material.color" ) )
{
    initGuideRectsColor( userColor );

    // Set a VAO and a VBO for every line rendered by this renderer.
    initWorldAxesData();
    initGuideAxisData();
    initTransformGuideLineData();
}


/***
 * 2. Destruction
 ***/

AuxiliarLinesRenderer::~AuxiliarLinesRenderer()
{
    // Tell OpenGL we are done with allocated buffer objects and
    // vertex attribute arrays.
    glDeleteVertexArrays( 1, &worldAxesVAO_ );
    glDeleteBuffers( 1, &worldAxesVBO_ );

    glDeleteBuffers( 1, &guideAxisVAO_ );
    glDeleteBuffers( 1, &guideAxisVBO_ );

    glDeleteBuffers( 1, &transformGuideLineVAO_ );
    glDeleteBuffers( 1, &transformGuideLineVBO_ );
}


/***
 * 3. Setters
 ***/

void AuxiliarLinesRenderer::setTransformGuideLine( const glm::vec3& origin, const glm::vec3& destiny )
{
    GLfloat* guideRectsBuffer = nullptr;
    unsigned int i = 0;

    glBindVertexArray( transformGuideLineVAO_ );
    glBindBuffer( GL_ARRAY_BUFFER, transformGuideLineVBO_ ); // TODO: Is this needed?
    guideRectsBuffer = (GLfloat *)glMapBuffer( GL_ARRAY_BUFFER,
                                               GL_WRITE_ONLY );
    for( ; i<3; i++ ){
        guideRectsBuffer[i] = origin[i];
    }
    for( ; i<6; i++ ){
        guideRectsBuffer[i] = destiny[i-3];
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );

    OpenGL::checkStatus( "AuxiliarLinesRenderer::setTransformGuideLine" );
}


void AuxiliarLinesRenderer::setGuideAxis( Axis axis, const glm::vec3 &center )
{
    const float AXIS_HALF_SIZE = 50.0f;
    GLfloat* vboData = nullptr;

    glBindVertexArray( guideAxisVAO_ );
    glBindBuffer( GL_ARRAY_BUFFER, guideAxisVBO_ ); // TODO: Is this needed?
    vboData = (GLfloat *)glMapBuffer( GL_ARRAY_BUFFER,
                                      GL_WRITE_ONLY );

    switch( axis ){
        case Axis::X:
            // X guide axis origin
            vboData[0] = center.x - AXIS_HALF_SIZE;
            vboData[1] = center.y;
            vboData[2] = center.z;

            // X guide axis end
            vboData[3] = center.x + AXIS_HALF_SIZE;
            vboData[4] = center.y;
            vboData[5] = center.z;
        break;
        case Axis::Y:
            // Y guide axis origin
            vboData[0] = center.x;
            vboData[1] = center.y - AXIS_HALF_SIZE;
            vboData[2] = center.z;

            // Y guide axis end
            vboData[3] = center.x;
            vboData[4] = center.y + AXIS_HALF_SIZE;
            vboData[5] = center.z;
        break;
        case Axis::Z:
            // Z guide axis origin
            vboData[0] = center.x;
            vboData[1] = center.y;
            vboData[2] = center.z - AXIS_HALF_SIZE;

            // Z guide axis end
            vboData[3] = center.x;
            vboData[4] = center.y;
            vboData[5] = center.z + AXIS_HALF_SIZE;
        break;
    }

    glUnmapBuffer( GL_ARRAY_BUFFER );

    OpenGL::checkStatus( "AuxiliarLinesRenderer::setGuideAxisCenter" );
}


/***
 * 5. Drawing
***/

void AuxiliarLinesRenderer::drawGuideAxis( Axis axis,
                                           const glm::vec3& centroid,
                                           const glm::mat4& viewMatrix,
                                           const glm::mat4& projectionMatrix )
{
    // TODO: Don't update everytime if not required!
    setGuideAxis( axis, centroid );

    openGL_->setShadingMode( ShadingMode::SOLID_PLAIN );
    openGL_->setMVPMatrix( glm::mat4( 1.0f ), viewMatrix, projectionMatrix );

    // Set the draw color
    glUniform4fv( colorShaderLocation_, 1, glm::value_ptr( guideRectsColor_ ) );

    // Bind guide axes' VAO and VBO as the active ones.
    glBindVertexArray( guideAxisVAO_ );
    glBindBuffer( GL_ARRAY_BUFFER, guideAxisVBO_ ); // TODO: Is this needed?

    // Draw the guide axis.
    glDrawArrays( GL_LINES, 0, 2 );
}


void AuxiliarLinesRenderer::drawWorldAxes( const glm::mat4& viewMatrix ) const
{
    openGL_->setShadingMode( ShadingMode::SOLID_PLAIN );
    openGL_->setMVPMatrix( glm::mat4( 1.0f ),
                           glm::mat4( glm::mat3( viewMatrix ) ),
                           glm::mat4( 1.0f ) );
    glDisable( GL_DEPTH_TEST );

    GLfloat worldAxisColors[3][4] =
    {
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 0.0f, 0.0f, 1.0f, 1.0f }
    };

    // Bind world axis rects' VAO and VBO as the active ones.
    glBindVertexArray( worldAxesVAO_ );
    glBindBuffer( GL_ARRAY_BUFFER, worldAxesVBO_ ); // TODO: Is this needed?

    // Draw each world axis with its corresponding color.
    for( unsigned int i=0; i<3; i++ ){
        glUniform4fv( colorShaderLocation_, 1, worldAxisColors[i] );
        glDrawArrays( GL_LINES, (i << 1), 2 );
    }

    glEnable( GL_DEPTH_TEST );
}


void AuxiliarLinesRenderer::drawTransformGuideLine( const glm::mat4& viewMatrix,
                                                    const glm::mat4& projectionMatrix ) const
{
    openGL_->setShadingMode( ShadingMode::SOLID_PLAIN );
    openGL_->setMVPMatrix( glm::mat4( 1.0f ), viewMatrix, projectionMatrix );

    // Bind selection centroid VAO and VBO as the active ones.
    glBindVertexArray( transformGuideLineVAO_ );
    glBindBuffer( GL_ARRAY_BUFFER, transformGuideLineVBO_ );

    // Set selection centroid color.
    glUniform4fv( colorShaderLocation_, 1, glm::value_ptr( guideRectsColor_ ) );

    // Draw selection centroid point.
    // TODO: The range of point sizes are implementation-dependent. Also I have to
    // check whether point size mode is enabled or not.
    glDisable( GL_DEPTH_TEST );
    glPointSize( 3.0f );
    glDrawArrays( GL_POINTS, 0, 1 );
    glPointSize( 1.0f );
    glDrawArrays( GL_LINES, 0, 2 );
    glEnable( GL_DEPTH_TEST );
}


/***
 * 6. Initialization
 ***/


void AuxiliarLinesRenderer::initGuideRectsColor( const Color &userColor )
{
    const Color RED_COLOR( 255, 0, 0 );
    const Color BLUE_COLOR( 0, 0, 255 );

    // Make sure that the color used for drawing guide rects isn't the same
    // as user's selection color.
    if( userColor == RED_COLOR ){
        guideRectsColor_ = BLUE_COLOR.toVec4();
    }else{
        guideRectsColor_ = RED_COLOR.toVec4();
    }
}


void AuxiliarLinesRenderer::initWorldAxesData()
{
    std::vector<GLfloat> vboData =
    {
        // World X axis
        0.0f, 0.0f, 0.0f,
        0.5f, 0.0f, 0.0f,

        // World Y axis
        0.0f, 0.0f, 0.0f,
        0.0f, 0.5f, 0.0f,

        // World Z axis
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.5f,
    };

    worldAxesVBO_ = generateVBO( vboData, GL_STATIC_DRAW );
    worldAxesVAO_ = generateVAO();
}


void AuxiliarLinesRenderer::initGuideAxisData()
{
    std::vector<GLfloat> vboData =
    {
        -100.0f, 0.0f, 0.0f,
        100.0f, 0.0f, 0.0f,
    };

    guideAxisVBO_ = generateVBO( vboData, GL_DYNAMIC_DRAW );
    guideAxisVAO_ = generateVAO();
}


void AuxiliarLinesRenderer::initTransformGuideLineData()
{
    std::vector<GLfloat> vboData =
    {
        // Auxiliar line for rotations and scales
        0.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f
    };

    transformGuideLineVBO_ = generateVBO( vboData, GL_DYNAMIC_DRAW );
    transformGuideLineVAO_ = generateVAO();
}


GLuint AuxiliarLinesRenderer::generateVBO( const std::vector<GLfloat> &vboData, GLenum usage )
{
    GLuint vboName = 0;

    glGenBuffers( 1, &vboName );
    assert( vboName != 0 );

    glBindBuffer( GL_ARRAY_BUFFER, vboName );
    glBufferData( GL_ARRAY_BUFFER,
                  vboData.size() * sizeof( GL_FLOAT ),
                  vboData.data(),
                  usage );

    return vboName;
}


GLuint AuxiliarLinesRenderer::generateVAO()
{
    GLuint vaoName = 0;
    GLint currentShaderProgram;
    GLint vPosition;

    // Get the position of the vertex shader variable "vPosition"
    // for the current shader program.
    // TODO: Use OpenGL class.
    glGetIntegerv( GL_CURRENT_PROGRAM, &currentShaderProgram );
    vPosition = glGetAttribLocation( currentShaderProgram, "vPosition" );
    if( vPosition == GL_INVALID_OPERATION ){
        throw std::runtime_error( "Error getting layout of \"position\"\n" );
    }

    glGenVertexArrays( 1, &vaoName );
    assert( vaoName != 0 );
    glBindVertexArray( vaoName );

    // By using the previous "vPosition" position, specify the location and data format of
    // the array of vertex positions.
    glVertexAttribPointer( vPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );
    glEnableVertexAttribArray( vPosition );

    return vaoName;
}

} // namespace como
