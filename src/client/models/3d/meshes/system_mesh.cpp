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

TextureWallsManager* SystemMesh::textureWallsManager_ = nullptr;


/***
 * 1. Construction
 ***/

// TODO: Create a "Resource" class with a ResourceID and a name (std::string)?
SystemMesh::SystemMesh( const ResourceID& meshID, const SystemPrimitiveData& primitiveData, MaterialConstPtr material, bool displayVertexNormals ) :
    Mesh( primitiveData, { material }, displayVertexNormals )
{
    for( const NamedTrianglesGroup& namedTrianglesGroup : primitiveData.trianglesGroups ){
        trianglesGroups_.push_back( TrianglesGroupWithTextureWall(
                                        textureWallsManager_->createTextureWall( namedTrianglesGroup.name, meshID ),
                                        namedTrianglesGroup.firstTriangleIndex,
                                        namedTrianglesGroup.nTriangles ) );
    }
}


/***
 * 3. Setters
 ***/

void SystemMesh::setTextureWallsManager( TextureWallsManager& textureWallsManager )
{
    textureWallsManager_ = &textureWallsManager;
}


/***
 * 4. Drawing
 ***/

void SystemMesh::draw( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const glm::vec4 *contourColor ) const
{
    sendToShader( *openGL, viewMatrix, projectionMatrix );
    sendMaterialToShader( 0 );

    for( const TrianglesGroupWithTextureWall& trianglesGroup : trianglesGroups_ ){
        if( textureWallsManager_->textureWallIncludesTexture( trianglesGroup.textureWallID ) ){
            openGL->setShadingMode( ShadingMode::SOLID_LIGHTING_AND_TEXTURING );
            textureWallsManager_->sendTextureWallToShader( trianglesGroup.textureWallID );
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
