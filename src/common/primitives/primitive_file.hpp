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

// TODO: Move readVec3() to another class and remove this class.
#include <glm/vec3.hpp>
#include <string>

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
        /*
        static void write( const PrimitiveData primitiveData, std::string filePath );
    private:
        static void writeVertices( const VerticesVector& vertices, std::ofstream& file );
        static void writeTriangles( const IndicesTrianglesVector& triangles, std::ofstream& file );
        static void writePolygonsGroups( const std::vector< PolygonGroupData >& polygonsGroupsData, std::ofstream& file );
        static void writeOpenGLData( const MeshOpenGLData& oglData, std::ofstream& file );
        static void writeMaterials( const std::vector< MaterialData >& materials, std::ofstream& file );
        static void writeMaterial( const MaterialData& material, std::ofstream& file );
        */


        /***
         * 4. File reading
         ***/
    public:



        /***
         * 5. Auxiliar methods
         ***/
    public:
        static glm::vec3 readVec3( std::string str );


        /***
         * 6. Operators
         ***/
        PrimitiveFile& operator = ( const PrimitiveFile& ) = delete;
        PrimitiveFile& operator = ( PrimitiveFile&& ) = delete;
};

} // namespace como

#endif // PRIMITIVE_FILE_WRITTER_HPP
