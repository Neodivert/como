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

#ifndef IMPORTED_MESH_HPP
#define IMPORTED_MESH_HPP

#include <client/models/3d/mesh.hpp>
#include <common/primitives/primitive_data/imported_primitive_data.hpp>

namespace como {

class ImportedMesh : public Mesh
{        
    public:
        /***
         * 1. Construction
         ***/
        ImportedMesh( const ResourceID& id, const ResourceID& firstMaterialID, const ImportedPrimitiveData& primitiveData, MaterialsManager& materialsManager, bool displayVertexNormals = false );
        ImportedMesh() = delete;
        ImportedMesh( const ImportedMesh& ) = delete;
        ImportedMesh( ImportedMesh&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~ImportedMesh() = default;


        /***
         * 3. Drawing
         ***/
        virtual void draw( OpenGLPtr openGL, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, const glm::vec4* contourColor = nullptr ) const;


        /***
         * 4. Operators
         ***/
        ImportedMesh& operator = ( const ImportedMesh& ) = delete;
        ImportedMesh& operator = ( ImportedMesh&& ) = delete;


    protected:
        /***
         * 5. Protected construction
         ***/
        // TODO: This constructor is needed by Light and Camera classes. Create
        // a new base class XMesh for Light and Camera which doesn't use
        // materials manager and / or ResourceIDs.
        ImportedMesh( const std::string& filePath );


        // TODO: Make this private and remove direct access from DirectionalLight.
        std::vector< TrianglesGroupWithMaterial > trianglesGroups_;
};

} // namespace como

#endif // IMPORTED_MESH_HPP
