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

ImportedMesh::ImportedMesh( const ResourceID& id, const ResourceID& firstMaterialID, const ImportedPrimitiveData& primitiveData, MaterialsManager& materialsManager, bool displayVertexNormals ) :
    Mesh( id, firstMaterialID, primitiveData, materialsManager, displayVertexNormals ),
    trianglesGroups_( primitiveData.trianglesGroups_ )
{}


/***
 * 3. Drawing
 ***/

void ImportedMesh::draw( OpenGLPtr openGL, const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix, const glm::vec4 *contourColor ) const
{
    if( includesTextures() ){
        openGL->setShadingMode( ShadingMode::SOLID_LIGHTING_AND_TEXTURING );
    }else{
        openGL->setShadingMode( ShadingMode::SOLID_LIGHTING );
    }

    sendToShader( *openGL, viewMatrix, projectionMatrix );

    for( const auto& trianglesGroup : trianglesGroups_ ){
        // Send this mesh's material to shader.
        sendMaterialToShader( trianglesGroup.materialIndex );

        drawTriangles( trianglesGroup.firstTriangleIndex, trianglesGroup.nTriangles );
    }

    drawEdges( openGL, viewMatrix, projectionMatrix, contourColor );

    if( displaysVertexNormals() ){
        drawVertexNormals( openGL, viewMatrix, projectionMatrix, glm::vec4( 1.0f, 0.0f, 0.0f, 0.0f ) );
    }
}


/***
 * 5. Protected construction
 ***/

// TODO: This dangerous MaterialsManager is created so next constructor can be
// called by derived classes which MUSN'T create materials or use materials
// manager in any way. FIX THIS.
MaterialsManager fooMaterialsManager( nullptr, nullptr );

ImportedMesh::ImportedMesh( const ResourceID &id, const std::string &filePath ) :
    Mesh( id, PrimitiveData::getNameFromFile( filePath.c_str() ), NO_RESOURCE, MeshType::MESH, filePath.c_str(), fooMaterialsManager )
{}

} // namespace como
