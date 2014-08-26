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

#ifndef ABSTRACT_MESH_HPP
#define ABSTRACT_MESH_HPP

#include "entity.hpp"

namespace como {

class AbstractMesh {

    public:
        /***
         * 1. Construction
         ***/
        AbstractMesh() = default;
        AbstractMesh( const AbstractMesh& ) = default;
        AbstractMesh( AbstractMesh&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~AbstractMesh() = default;


        /***
         * 3. Setters
         ***/
        virtual void displayVertexNormals( bool display ) = 0;
        virtual void displayEdges( bool display ) = 0;


        /***
         * 5. Operators
         ***/
        AbstractMesh& operator = ( const AbstractMesh& ) = default;
        AbstractMesh& operator = ( AbstractMesh&& ) = default;
};

} // namespace como

#endif // ABSTRACT_MESH_HPP
