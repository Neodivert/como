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

#include "primitives_importer.hpp"
#include <boost/filesystem.hpp>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <GL/gl.h>
#include <array>

namespace como {

struct MeshInfo {
    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec3 > normals;
    std::vector< glm::vec2 > uvCoordinates;

    std::vector< std::array< GLuint, 3 > > vertexTriangles;
    std::vector< std::array< GLuint, 3 > > normalTriangles;
    std::vector< std::array< GLuint, 3 > > uvTriangles;

    std::vector< GLfloat > vboData;
    std::vector< GLuint > eboData;
};

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
        void processMeshFileLine( std::string line, PrimitiveInfo& primitiveInfo, MeshInfo& meshInfo );
        void generateMeshVertexData( MeshInfo& meshInfo );

    private:
        void importMaterialFile( PrimitiveInfo& primitive, std::string srcFilePath, std::string dstDirectory );
        void importTextureFile( PrimitiveInfo& primitive, std::string srcFilePath, std::string dstDirectory );


        /***
         * 4. Operators
         ***/
        OBJPrimitivesImporter& operator = ( const OBJPrimitivesImporter& ) = delete;
        OBJPrimitivesImporter& operator = ( OBJPrimitivesImporter&& ) = delete;
};

} // namespace como

#endif // OBJ_PRIMITIVES_IMPORTER_HPP
