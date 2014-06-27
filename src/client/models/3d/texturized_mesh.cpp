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

#include "texturized_mesh.hpp"
#include <cstdint>

namespace como {

const GLint SHADER_TEXTURE_ATTR_LOCATION = 2; // FIXME: This value depends on SHADER_* constants in mesh.cpp (high coupling!)

TexturizedMesh::TexturizedMesh( MaterialConstPtr material, TextureConstPtr texture ) :
    Mesh( material ),
    texture_( texture )
{}


/***
 * 2. Initialization
 ***/

bool TexturizedMesh::processFileLine( const string &line )
{
    glm::vec2 textureCoordinates;
    std::array< GLuint, 3 > vertexTriangle;
    std::array< GLuint, 3 > textureTriangle;
    unsigned int i = 0;

    if( line.substr( 0, 2 ) == "vt" ){
        // Extract the UV coordinates from the line and add it to the Mesh.
        sscanf( line.c_str(), "vt %f %f", &textureCoordinates[0], &textureCoordinates[1] );

        std::cout << "UV[" << textureCoordinates_.size() << "]: ("
                  << textureCoordinates[0] << ", " << textureCoordinates[1] << ")" << std::endl;

        textureCoordinates_.push_back( textureCoordinates );

        return true;
    }else if( line[0] == 'f' ){
        // Extract the UV coordinates from the line and add it to the Mesh.
        std::cout << "sscanf: " << sscanf( line.c_str(), "f %u/%u %u/%u %u/%u",
                &vertexTriangle[0], &textureTriangle[0],
                &vertexTriangle[1], &textureTriangle[1],
                &vertexTriangle[2], &textureTriangle[2]) << std::endl;

        for( i=0; i<3; i++ ){
            // Decrement every vertex index because they are 1-based in the .obj file.
            vertexTriangle[i] -= 1;
            textureTriangle[i] -= 1;
        }

        std::cout << "Vertex triangle: (" << vertexTriangle[0] << ", " << vertexTriangle[1] << ", " << vertexTriangle[2] << ")" << std::endl;
        std::cout << "Texture triangle: " << textureTriangle[0] << ", " << textureTriangle[1] << ", " << textureTriangle[2] << ")" << std::endl;

        triangles.push_back( vertexTriangle );
        textureTriangles_.push_back( textureTriangle );

        return true;
    }else{
        return Mesh::processFileLine( line );
    }
}


void TexturizedMesh::initVAO()
{
    Mesh::initVAO();

    int byteOffset = 6 * sizeof( GL_FLOAT ); //getBytesPerVertex() - Mesh::getBytesPerVertex();

    // Include texture coordinates info in VAO.
    glVertexAttribPointer( SHADER_TEXTURE_ATTR_LOCATION, 2, GL_FLOAT, GL_FALSE, (int)getBytesPerVertex(), (void *)( (std::uintptr_t)( byteOffset ) ) ); // TODO: Fix the std::uintptr_t cast?

    // Enable previous vertex data arrays.
    glEnableVertexAttribArray( SHADER_TEXTURE_ATTR_LOCATION );
}


unsigned int TexturizedMesh::getOwnBytesPerVertex() const
{
    return getOwnComponentsPerVertex() * sizeof( GL_FLOAT );
}


unsigned int TexturizedMesh::getOwnComponentsPerVertex() const
{
    return 2; // UV coordinates.
}

unsigned int TexturizedMesh::getComponentsPerVertex() const
{
    return Mesh::getOwnComponentsPerVertex() + getOwnComponentsPerVertex();
}


void TexturizedMesh::setVertexData( GLint index )
{   
    GLfloat textureCoordinates[] =
    {
        textureCoordinates_[index][X],
        textureCoordinates_[index][Y]
    };


    Mesh::setVertexData( index );

    std::cout << "getBytesPerVertex(): " << getBytesPerVertex() << std::endl
              << "Mesh::getBytesPerVertex(): " << Mesh::getBytesPerVertex() << std::endl;
    std::cout << "TexturizedMesh::setVertexData(" << ( index * getBytesPerVertex() + Mesh::getOwnBytesPerVertex()) << ", " << getOwnBytesPerVertex() << ")" << std::endl;

    OpenGL::checkStatus( "TexturizedMesh::setVertexData - 1" );
    glBufferSubData( GL_ARRAY_BUFFER, ( index * getBytesPerVertex() + Mesh::getOwnBytesPerVertex()), getOwnBytesPerVertex(), textureCoordinates );
    OpenGL::checkStatus( "TexturizedMesh::setVertexData - 2" );
}


void TexturizedMesh::draw( OpenGLPtr openGL, const glm::mat4& viewProjMatrix, const GLfloat* contourColor ) const
{
    openGL->enableTexturing();

    //texture_->sendToShader();

    Mesh::draw( openGL, viewProjMatrix, contourColor );

    openGL->disableTexturing();
}

} // namespace como
