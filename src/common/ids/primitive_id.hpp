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

#ifndef PRIMITIVE_ID_HPP
#define PRIMITIVE_ID_HPP

#include <cstdint>
#include <common/packables/packable_uint8.hpp>

namespace como {

typedef std::uint8_t ResourceID;
typedef PackableUint8< ResourceID > PackableResourceID;

} // namespace como

#endif // PRIMITIVE_ID_HPP
