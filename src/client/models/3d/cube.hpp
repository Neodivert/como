/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef CUBE_HPP
#define CUBE_HPP

#include "mesh.hpp"

namespace como {

const GLuint N_CUBE_VERTICES = 8;

class Cube : public Mesh
{
    public:
        /***
         * 1. Initialization and destruction
         ***/
        Cube( const Cube& ) = delete;
        Cube( Cube&& ) = delete;
        Cube( const std::uint8_t* color = nullptr );

        // TODO: Is this problematic with Drawable's virtual destructor?
        ~Cube() = default;


        /***
         * 2. Operators
         ***/
        Cube& operator=( const Cube& ) = delete ;
        Cube& operator=( Cube&& ) = delete;
};


} // namespace como.

#endif // CUBE_HPP
