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

#include "system_mesh.hpp"

namespace como {

TexturesManager* SystemMesh::texturesManager_ = nullptr;

/***
 * 1. Construction
 ***/

SystemMesh::SystemMesh( const SystemPrimitiveData& primitiveData, MaterialConstPtr material, bool displayVertexNormals ) :
    Mesh( primitiveData, { material }, displayVertexNormals ),
    trianglesGroups_( primitiveData.trianglesGroups )
{}


/***
 * 3. Setters
 ***/

void SystemMesh::setTexturesManager( TexturesManager& texturesManager)
{
    texturesManager_ = &texturesManager;
}


/***
 * 4. Drawing
 ***/

void SystemMesh::draw( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const glm::vec4 *contourColor ) const
{
    sendToShader( *openGL, viewMatrix, projectionMatrix );
    sendMaterialToShader( 0 );

    for( const TrianglesGroupWithTexture& trianglesGroup : trianglesGroups_ ){
        if( trianglesGroup.includesTexture() ){
            openGL->setShadingMode( ShadingMode::SOLID_LIGHTING_AND_TEXTURING );
            texturesManager_->sendTextureToShader( trianglesGroup.textureID );
        }else{
            openGL->setShadingMode( ShadingMode::SOLID_LIGHTING );
        }

        drawTriangles( trianglesGroup.firstTriangleIndex, trianglesGroup.nTriangles );
    }

    drawEdges( openGL, viewMatrix, projectionMatrix, contourColor );

    if( displaysVertexNormals() ){
        drawVertexNormals( openGL, viewMatrix, projectionMatrix, glm::vec4( 1.0f, 0.0f, 0.0f, 0.0f )  );
    }
}

} // namespace como
