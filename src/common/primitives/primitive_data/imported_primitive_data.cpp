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

#include "imported_primitive_data.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ImportedPrimitiveData::ImportedPrimitiveData(std::string filePath)
{
    importFromFile( filePath );
}


/***
 * 2. File importing / exporting
 ***/

void ImportedPrimitiveData::importFromFile( const std::string &filePath )
{
    std::ifstream file;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    readVertices( file );
    readTriangles( file );
    readTrianglesGroups( file );
    readOpenGLData( file );
    readMaterials( file );

    file.close();
}


void ImportedPrimitiveData::exportToFile( const std::string& filePath ) const
{
    std::ofstream file;

    file.open( filePath );
    if( !file.is_open() ){
        throw FileNotOpenException( filePath );
    }

    writeVertices( file );
    writeTriangles( file );
    writeTrianglesGroups( file );
    writeOpenGLData( file );
    writeMaterials( file );

    file.close();
}


/***
 * 3. File reading
 ***/

void ImportedPrimitiveData::readTrianglesGroups( std::ifstream& file )
{
    std::string fileLine;

    // Read the number of triangles groups and reserve space for them.
    std::getline( file, fileLine );
    trianglesGroups_.resize( atoi( fileLine.c_str() ) );

    // Write all the triangles groups (one per line).
    for( auto& trianglesGroup : trianglesGroups_ ){
        std::getline( file, fileLine );

        sscanf( fileLine.c_str(),
                "%u %u %u",
                &( trianglesGroup.firstTriangleIndex ),
                &( trianglesGroup.nTriangles ),
                &( trianglesGroup.materialIndex ) );
    }
}


/***
 * 4. File writting
 ***/

void ImportedPrimitiveData::writeTrianglesGroups( std::ofstream &file ) const
{
    // Write the number of polygons groups.
    file << trianglesGroups_.size() << std::endl;

    // Write all the polygons groups (one per line).
    for( const auto& trianglesGroup : trianglesGroups_ ){
        file << trianglesGroup.firstTriangleIndex << " "
             << trianglesGroup.nTriangles << " "
             << trianglesGroup.materialIndex << std::endl;
    }
}

} // namespace como
