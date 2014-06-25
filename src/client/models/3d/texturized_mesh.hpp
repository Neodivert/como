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

#ifndef TEXTURIZED_MESH_HPP
#define TEXTURIZED_MESH_HPP

#include "mesh.hpp"

namespace como {

class TexturizedMesh : public Mesh
{
    public:
        /***
         * 1. Construction
         ***/
        TexturizedMesh() = delete;
        TexturizedMesh( const std::string& filePath, MaterialConstPtr material );
        TexturizedMesh( const TexturizedMesh& ) = delete;
        TexturizedMesh( TexturizedMesh&& ) = delete;


        /***
         * 2. Initialization
         ***/
        void loadFromFile( const std::string& filePath );
        virtual void initVAO();
        virtual unsigned int getBytesPerVertex() const;
        virtual unsigned int getComponentsPerVertex() const;
        virtual void setVertexData(GLfloat *vbo, GLint index);


        /***
         * 3. Destruction
         ***/
        ~TexturizedMesh() = default; // TODO: Or not default?


        /***
         * 4. Operators
         ***/
        TexturizedMesh& operator = ( const TexturizedMesh& ) = delete;
        TexturizedMesh& operator = ( TexturizedMesh&& ) = delete;
};

} // namespace como

#endif // TEXTURIZED_MESH_HPP
