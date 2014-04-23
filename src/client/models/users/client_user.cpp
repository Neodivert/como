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

#include "client_user.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ClientUser::ClientUser( UserID id, std::string name, const std::uint8_t* selectionColor ) :
    BasicUser( id, name )
{
    unsigned int i = 0;

    // Retrieve user's selection color.
    for( i=0; i<4; i++ ){
        selectionColor_[i] = selectionColor[i] / 255.0f;
    }
}


/***
 * 3. Getters
 ***/

const GLfloat* ClientUser::getSelectionColor() const
{
    return selectionColor_;
}


} // namespace como
