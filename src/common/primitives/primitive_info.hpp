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

#ifndef PRIMITIVE_INFO_HPP
#define PRIMITIVE_INFO_HPP

#include <string>
#include <common/ids/resource_id.hpp>

namespace como {

struct PrimitiveInfo
{
    std::string name;

    ResourceID category;
    std::string directory;

    std::string meshFileName;
    std::string materialFileName;
    std::string textureFileName;
};

} // namespace como

#endif // PRIMITIVE_INFO_HPP
