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

#ifndef PRIMITIVE_DATA_HPP
#define PRIMITIVE_DATA_HPP

#include "material_info.hpp"
#include "mesh_vertex_data.hpp"
#include "mesh_normal_data.hpp"
#include "mesh_texture_data.hpp"
#include "mesh_opengl_data.hpp"
#include "polygon_group_data.hpp"
#include <fstream>
#include <common/exceptions/file_not_open_exception.hpp>
#include <map>

namespace como {

typedef GLuint VertexIndice;
typedef std::array< VertexIndice, 3 > CompoundVertex; // A vertex formed by a position vertex, a normal and an UV vertex (TODO: These are TRIANGLE INDICES, do I change the name?).
typedef std::map< CompoundVertex, VertexIndice > CompoundVerticesMap;


struct PrimitiveData
{
    public:
        MeshVertexData vertexData;
        MeshNormalData normalData;
        MeshTextureData uvData;

        MeshOpenGLData oglData;

        MaterialsInfoVector materialsInfo_;


        /***
         * 1. Getters
         ***/
        unsigned int getMaterialIndex( const std::string& name ) const;


        /***
         * 2. Data generation
         ***/
        void generateOGLData();


        /***
         * 3. Importing / exporting
         ***/
        virtual void importFromFile( const std::string &filePath );
        virtual void exportToFile( const std::string& filePath ) const;


    protected:
        /***
         * 3. File reading / writing
         ***/
        virtual void read( std::ifstream& file );
        virtual void write( std::ofstream& file ) const;


    private:
        /***
         * 4. File reading (auxiliar methods)
         ***/
        void readVertices( std::ifstream& file );
        void readTriangles( std::ifstream& file );
        void readOpenGLData( std::ifstream& file );
        void readMaterials( std::ifstream &file );


        /***
         * 5. File writting (auxiliar methods)
         ***/
        void writeVertices( std::ofstream& file ) const;
        void writeTriangles( std::ofstream& file ) const;
        void writeOpenGLData( std::ofstream& file ) const;
        void writeMaterials(std::ofstream &file) const;
};

} // namespace como


#endif // PRIMITIVE_DATA_HPP
