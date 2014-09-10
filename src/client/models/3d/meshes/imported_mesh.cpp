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

#include "imported_mesh.hpp"

namespace como {

/***
 * 1. Construction
 ***/

ImportedMesh::ImportedMesh( const ImportedPrimitiveData& primitiveData, ConstMaterialsVector materials, bool displayVertexNormals ) :
    Mesh( primitiveData, materials, displayVertexNormals ),
    trianglesGroups_( primitiveData.trianglesGroups_ )
{}


ImportedMesh::ImportedMesh( const string &filePath ) :
    Mesh( MeshType::MESH, filePath.c_str() )
{}


/***
 * 3. Drawing
 ***/

void ImportedMesh::draw( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const glm::vec4 *contourColor ) const
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

    for( const auto& trianglesGroup : trianglesGroups_ ){
        // Send this mesh's material to shader.
        sendMaterialToShader( trianglesGroup.materialIndex );

        glDrawElements( GL_TRIANGLES,
                        trianglesGroup.nTriangles * 3,
                        GL_UNSIGNED_INT,
                        ( std::intptr_t* )( trianglesGroup.firstTriangleIndex * 3 * sizeof( GL_UNSIGNED_INT ) ) );
    }

    drawEdges( openGL, viewMatrix, projectionMatrix, contourColor );

    if( displayVertexNormals_ ){
        drawVertexNormals( openGL, viewMatrix, projectionMatrix, glm::vec4( 1.0f, 0.0f, 0.0f, 0.0f ) );
    }
}

} // namespace como
