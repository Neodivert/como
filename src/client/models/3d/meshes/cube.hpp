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

#ifndef CUBE_HPP
#define CUBE_HPP

#include <client/models/3d/meshes/system_mesh.hpp>

namespace como {

class Cube
{
    public:
        /***
         * 1. Construction
         ***/
        Cube( float width, float height, float depth );
        Cube() = delete;
        Cube( const Cube& ) = delete;
        Cube( Cube&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~Cube() = default;


        /***
         * 3. Operators
         ***/
        Cube& operator = ( const Cube& ) = delete;
        Cube& operator = ( Cube&& ) = delete;


    private:
        /***
         * 4. Initialization
         ***/
        static MeshVertexData generateVertexData( float width, float height, float depth );
        static MeshNormalData generateNormalData( const MeshVertexData& meshVertexData );
        static MeshTextureData generateTextureData();
};

} // namespace como

#endif // CUBE_HPP
