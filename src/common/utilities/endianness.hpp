/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
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
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef ENDIANNESS_HPP
#define ENDIANNESS_HPP

#include <cstdint>

namespace como {

/***
 * 1. Uint16 translation
 ***/
std::uint16_t translateToNetworkOrder( const std::uint16_t& value );
std::uint16_t translateFromNetworkOrder( const std::uint16_t& value );


/***
 * 2. Uint32 translation
 ***/
std::uint32_t translateToNetworkOrder( const std::uint32_t& value );
std::uint32_t translateFromNetworkOrder( const std::uint32_t& value );

} // namespace como

#endif // ENDIANNESS_HPP
