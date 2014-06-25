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

#include "texturized_mesh.hpp"

namespace como {

TexturizedMesh::TexturizedMesh( const string &filePath, MaterialConstPtr material ) :
    Mesh( MeshType::MESH, filePath.c_str(), material )
{
    loadFromFile( filePath );
}


/***
 * 2. Initialization
 ***/

void TexturizedMesh::loadFromFile( const string &filePath )
{
    (void)( filePath );
    /*
    std::ifstream file( filePath.c_str() );
    std::string line;
    glm::vec2 uvCoordinates;

    // Vertex coordinates and another datahas been read in
    // Mesh::loadFromFile(). We only read texture coordinates here.
    while( !file.eof() ){
        std::getline( file, line );

        if( line.substr( 0, 2 ) == "vt" ){
            // Set '.' as the float separator (for parsing floats from a text
            // line).
            setlocale( LC_NUMERIC, "C" );

            // Extract the UV coordinates from the line and add it to the Mesh.
            sscanf( line, "vt %f %f", &uvCoordinates[0], &uvCoordinates[1] );

            originalVertices.push_back( vertex );

            // TODO: Oh no, we have to mix UV coordinates with more vertex data
            // in VBO! It can't be as simple as calling Mesh constructor and
            // then Texturized mesh. Noooooo!.
        }
    }


    file.close();
    */
}



} // namespace como
