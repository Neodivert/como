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

#ifndef TRIANGLES_GROUP_HPP
#define TRIANGLES_GROUP_HPP

namespace como {

// TODO: Rename this and derived structs as TrianglesRange<X>?
// TODO: Move this and derived structs to a directory "triangles".
struct TrianglesGroup
{
    unsigned int firstTriangleIndex;
    unsigned int nTriangles;
    bool includesUV;

    TrianglesGroup( unsigned int firstTriangleIndex = 0, unsigned int nTriangles = 0, bool includesUV = false ) :
        firstTriangleIndex( firstTriangleIndex ),
        nTriangles( nTriangles ),
        includesUV( includesUV ){}
};

} // namespace como

#endif // TRIANGLES_GROUP_HPP
