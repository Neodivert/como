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

#ifndef MATERIAL_ID_HPP
#define MATERIAL_ID_HPP

#include <common/utilities/ids.hpp>
#include <common/packables/packable_integer.hpp>
#include <stdexcept>

namespace como {

typedef std::uint16_t MaterialIndex;
typedef PackableUint16< MaterialIndex > PackableMaterialIndex;

// TODO: Inherit this class and DrawableID from a common PairID base class.
class MaterialID
{
    private:
        UserID creatorID_;
        MaterialIndex materialIndex_;

    public:
        /***
         * 1. Construction
         ***/
        MaterialID();
        MaterialID( UserID creatorID, MaterialIndex materialIndex );
        MaterialID( const MaterialID& ) = default;
        MaterialID( MaterialID&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~MaterialID() = default;


        /***
         * 3. Getters
         ***/
        UserID getCreatorID() const;
        MaterialIndex getMaterialIndex() const;


        /***
         * 4. Operators
         ***/
        MaterialID& operator = ( const MaterialID& ) = default;
        MaterialID& operator = ( MaterialID&& ) = default;

        bool operator == ( const MaterialID& b ) const;
        bool operator < ( const MaterialID& b ) const;

        MaterialID operator ++ ();      // Prefix
        MaterialID operator ++ (int);   // Postfix
};

/***
 * 5. Console output
 ***/
std::ostream& operator << ( std::ostream& os, const MaterialID& id );

} // namespace como

#endif // MATERIAL_ID_HPP
