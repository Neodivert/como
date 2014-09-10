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

#include "primitive_file.hpp"
#include <common/exceptions/file_not_open_exception.hpp>
#include <fstream>

namespace como {

/***
 * 5. Auxiliar methods
 ***/

glm::vec3 PrimitiveFile::readVec3( std::string str )
{
    glm::vec3 v;

    sscanf( str.c_str(), "%f %f %f", &v[0], &v[1], &v[2] );

    return v;
}

} // namespace como
