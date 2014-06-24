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
        TexturizedMesh();
        TexturizedMesh( const TexturizedMesh& ) = delete;
        TexturizedMesh( TexturizedMesh&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TexturizedMesh() = default; // TODO: Or not default?


        /***
         * 3. Operators
         ***/
        TexturizedMesh& operator = ( const TexturizedMesh& ) = delete;
        TexturizedMesh& operator = ( TexturizedMesh&& ) = delete;
};

} // namespace como

#endif // TEXTURIZED_MESH_HPP
