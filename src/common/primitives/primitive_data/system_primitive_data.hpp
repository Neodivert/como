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

#ifndef SYSTEM_PRIMITIVE_DATA_HPP
#define SYSTEM_PRIMITIVE_DATA_HPP
// TODO: Move this struct to client source code directory.

#include <common/primitives/primitive_data/primitive_data.hpp>

#include <common/primitives/primitive_data/named_triangles_group.hpp>

namespace como {

// TODO: Rename?
struct SystemPrimitiveData : public PrimitiveData
{
    std::vector< NamedTrianglesGroup > trianglesGroups;

    SystemPrimitiveData(){ materialsInfo_.push_back( MaterialInfo() ); }

    void generatePerVertexNormals( unsigned int wallIndex );
    void generatePerFaceNormals( unsigned int wallIndex );
};

} // namespace como

#endif // SYSTEM_PRIMITIVE_DATA_HPP
