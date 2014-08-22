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

#ifndef PICKABLES_SET_HPP
#define PICKABLES_SET_HPP

#include "glm/glm.hpp"
#include <common/ids/resource_id.hpp>

namespace como {

class PickablesSet
{
    public:
        /***
         * 1. Construction
         ***/
        PickablesSet() = default;
        PickablesSet( const PickablesSet& ) = default;
        PickablesSet( PickablesSet&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~PickablesSet() = default;


        /***
         * 3. Object picking
         ***/
        virtual bool pick( const glm::vec3& rayOrigin,
                           glm::vec3 rayDirection,
                           ResourceID& pickedElement,
                           float& t,
                           const float& MAX_T = FLT_MAX ) const = 0;


        /***
         * 4. Operators
         ***/
        PickablesSet& operator = ( const PickablesSet& ) = default;
        PickablesSet& operator = ( PickablesSet&& ) = default;
};

} // namespace como

#endif // PICKABLES_SET_HPP
