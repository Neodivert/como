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
 * 2. File reading / writing
 ***/

void ImportedPrimitiveData::read( std::ifstream &file )
{
    PrimitiveData::read( file );
    readTrianglesGroups( file );
}


void ImportedPrimitiveData::write( std::ofstream &file ) const
{
    PrimitiveData::write( file );
    writeTrianglesGroups( file );
}


/***
 * 3. File reading (auxiliar methods)
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
 * 4. File writting (auxiliar methods)
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
