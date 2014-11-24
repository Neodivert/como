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

#include <common/primitives/primitive_data/imported_primitive_data.hpp>
#include "primitives_importer.hpp"
#include <map>

enum class FaceType
{
    TRIANGLE,
    QUAD,
    OTHER
};

enum class FaceComponents
{
    ONLY_VERTICES,
    VERTICES_AND_NORMALS,
    VERTICES_AND_UVS,
    VERTICES_NORMALS_AND_UVS
};

typedef std::array< GLuint, 3 > FaceTriangle;
typedef std::array< GLuint, 4 > FaceQuad;

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
        virtual ~OBJPrimitivesImporter() = default;


        /***
         * 3. Primitives import
         ***/

        /*!
         * \brief Rename the file <srcFilePath> and the files referenced by it
         * as <name>.original_extension and copy them all to <destDirectory>.
         */
        virtual PrimitiveInfo importPrimitive( std::string srcFilePath, std::string dstDirectory, std::string nameSuffix );

    private:
        virtual void processMeshFile( std::string filePath, PrimitiveInfo& primitiveInfo, ImportedPrimitiveData& primitiveData );
        void processMeshFileLine( std::string filePath, std::string line, PrimitiveInfo& primitiveInfo, ImportedPrimitiveData& primitiveData );
        void generateMeshVertexData( ImportedPrimitiveData& primitiveData );
        void computeNormalData( const MeshVertexData& vertexData, MeshNormalData& normalData );

    private:
        void processMaterialFile( std::string filePath, bool& includesUV, MaterialsInfoVector& materials );
        void processMaterialFileLine( std::string filePath, std::string fileLine, MaterialsInfoVector& materials, bool &includesUV );

        void processTextureFile( std::string filePath, std::unique_ptr< TextureInfo >& textureInfo );


        /***
         * 4. Operators
         ***/
        OBJPrimitivesImporter& operator = ( const OBJPrimitivesImporter& ) = delete;
        OBJPrimitivesImporter& operator = ( OBJPrimitivesImporter&& ) = delete;


        /***
         * 5. Auxiliar methods
         ***/
        void readLine( std::ifstream& file, std::string& fileLine );
        FaceType getFaceType( const std::string& faceDefinition );
        FaceComponents getFaceComponents( const std::string& faceDefinition );
        void processTriangleFaceStr( const std::string& lineBody, ImportedPrimitiveData& primitiveData );
        void processQuadFaceStr( const std::string& lineBody, ImportedPrimitiveData& primitiveData );
        void triangulateQuad( const FaceQuad& quad,
                              FaceTriangle& triangle1,
                              FaceTriangle& triangle2 );
        void insertQuad( std::vector< FaceTriangle >& triangles, FaceQuad& quad );
        void splitFileLine( const std::string& line, std::string& lineHeader, std::string& lineBody );
        bool supportedImageFile( const std::string& filePath );
};

} // namespace como

#endif // OBJ_PRIMITIVES_IMPORTER_HPP
