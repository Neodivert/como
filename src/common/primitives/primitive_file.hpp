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

#ifndef PRIMITIVE_FILE_HPP
#define PRIMITIVE_FILE_HPP

#include <common/mesh_info/mesh_info.hpp>
typedef std::vector< glm::vec3 > VerticesVector;
typedef std::vector< std::array< GLuint, 3 > > TrianglesVector;

namespace como {

class PrimitiveFile
{
    public:
        /***
         * 1. Construction
         ***/
        PrimitiveFile() = delete;
        PrimitiveFile( const PrimitiveFile& ) = delete;
        PrimitiveFile( PrimitiveFile&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~PrimitiveFile() = default;


        /***
         * 3. File writting
         ***/
        static void write( const MeshInfo meshInfo, std::string filePath );
    private:
        static void writeVertices( const VerticesVector& vertices, std::ofstream& file );
        static void writeTriangles( const VertexTrianglesVector& triangles, std::ofstream& file );
        static void writeOpenGLData( const MeshOpenGLData& oglData, std::ofstream& file );
        static void writeMaterials( const std::vector< MaterialInfo >& materials, std::ofstream& file );
        static void writeMaterial( const MaterialInfo& material, std::ofstream& file );


        /***
         * 4. File reading
         ***/
    public:
        static void read( MeshInfo& meshInfo, std::string filePath );
    private:
        static void readVertices( VerticesVector& vertices, std::ifstream& file );



        /***
         * 5. Operators
         ***/
        PrimitiveFile& operator = ( const PrimitiveFile& ) = delete;
        PrimitiveFile& operator = ( PrimitiveFile&& ) = delete;
};

} // namespace como

#endif // PRIMITIVE_FILE_WRITTER_HPP
