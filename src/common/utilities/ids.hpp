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

#ifndef IDS_HPP
#define IDS_HPP

#include "../packables/packables.hpp"
#include <cstdint>

namespace como {

// TODO: Move to a config file.
const char SCENES_PRIMITIVES_DIR[] = "data/primitives/scenes";

typedef std::uint16_t UserID;
typedef PackableUint16< UserID > PackableUserID;


typedef std::uint32_t DrawableIndex;
typedef PackableUint32< DrawableIndex > PackableDrawableIndex;

/*
 * A drawable is uniquely identified by a pair of values:
 * - The ID of the user who created it.
 * - A index to differ the drawable from others created by the same user.
 */

struct PackableDrawableID : public CompositePackable {
    PackableUserID creatorID;
    PackableDrawableIndex drawableIndex;


    /***
     * 1. Construction
     ***/
    PackableDrawableID();
    PackableDrawableID( const PackableDrawableID& b );
    PackableDrawableID( PackableDrawableID&& b );


    /***
     * 2. Destruction
     ***/
    ~PackableDrawableID() = default;


    /***
     * 3. Operators
     ***/
    bool operator < ( const PackableDrawableID& b ) const ;
    bool operator == ( const PackableDrawableID& b ) const ;

    PackableDrawableID& operator = (const PackableDrawableID& b );
    PackableDrawableID& operator = ( PackableDrawableID&& ) = delete;
};

const PackableDrawableID NULL_DRAWABLE_ID;

} // namespace como


#endif // IDS_HPP
