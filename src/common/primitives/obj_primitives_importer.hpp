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

#ifndef OBJ_PRIMITIVES_IMPORTER_HPP
#define OBJ_PRIMITIVES_IMPORTER_HPP

#include <common/mesh_info/mesh_info.hpp>
#include "primitives_importer.hpp"
/*
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GL/gl.h>
#include <array>
*/

namespace como {

class OBJPrimitivesImporter : PrimitivesImporter {
    public:
        /***
         * 1. Construction
         ***/
        OBJPrimitivesImporter() = default;
        OBJPrimitivesImporter( OBJPrimitivesImporter& ) = delete;
        OBJPrimitivesImporter( OBJPrimitivesImporter&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~OBJPrimitivesImporter() = default;


        /***
         * 3. Primitives import
         ***/

        /*!
         * \brief Rename the file <srcFilePath> and the files referenced by it
         * as <name>.original_extension and copy them all to <destDirectory>.
         */
        virtual PrimitiveInfo importPrimitive( std::string srcFilePath, std::string dstDirectory, std::string nameSuffix );

    private:
        virtual void processMeshFile( std::string filePath, PrimitiveInfo& primitiveInfo, MeshInfo& meshInfo );
        void processMeshFileLine( std::string filePath, std::string line, PrimitiveInfo& primitiveInfo, MeshInfo& meshInfo );
        void generateMeshVertexData( MeshInfo& meshInfo );
        void computeVertexNormals( const MeshVertexData& vertexData, MeshNormalData& normalData );

    private:
        void processMaterialFile( std::string filePath, std::vector< MaterialInfo >& materials );
        void processMaterialFileLine( std::string fileLine, std::vector< MaterialInfo >& materials );
        void importTextureFile( PrimitiveInfo& primitive, std::string srcFilePath, std::string dstDirectory );


        /***
         * 4. Operators
         ***/
        OBJPrimitivesImporter& operator = ( const OBJPrimitivesImporter& ) = delete;
        OBJPrimitivesImporter& operator = ( OBJPrimitivesImporter&& ) = delete;
};

} // namespace como

#endif // OBJ_PRIMITIVES_IMPORTER_HPP
