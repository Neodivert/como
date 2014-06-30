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

#include "primitive_file.hpp"
#include <common/exceptions/file_not_open_exception.hpp>
#include <fstream>

namespace como {

/***
 * 3. File management
 ***/

void PrimitiveFile::write( const MeshInfo meshInfo, std::string filePath )
{
    std::ofstream file;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    writeVertices( meshInfo.vertexData.vertices, file );
    writeTriangles( meshInfo.vertexData.vertexTriangles, file );
}


void PrimitiveFile::writeVertices( const VerticesVector& vertices, std::ofstream& file )
{
    // Write the number of vertices in the mesh.
    file << vertices.size() << std::endl;

    // Write all the vertices to the file (one vertex per line).
    for( auto vertex : vertices ){
        file << vertex[0] << " " << vertex[1] << " " << vertex[2] << std::endl;
    }
}


void PrimitiveFile::writeTriangles( const TrianglesVector& triangles, std::ofstream& file )
{
    // Write the number of triangles in the mesh.
    file << triangles.size() << std::endl;

    // Write all the triangles to the file (one triangle per line).
    for( auto triangle : triangles ){
        file << triangle[0] << " " << triangle[1] << " " << triangle[2] << std::endl;
    }
}


} // namespace como
