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

#include "primitive_info.hpp"

namespace como {

/***
 * 3. Getters
 ***/

bool PrimitiveInfo::includesTexture() const
{
    // TODO: Return real value.
    return false;
}


/***
 * 4. Primitive file management
 ***/

PrimitiveInfo PrimitiveInfo::copy( std::string dstFilePath )
{
    PrimitiveInfo primitiveCopy( *this );

    boost::filesystem::copy( filePath, dstFilePath );

    primitiveCopy.filePath = dstFilePath;

    return primitiveCopy;
}


void PrimitiveInfo::move( std::string dstFilePath )
{
    boost::filesystem::copy( filePath, dstFilePath );
    boost::filesystem::remove( filePath );

    filePath = dstFilePath;
}

} // namespace como
