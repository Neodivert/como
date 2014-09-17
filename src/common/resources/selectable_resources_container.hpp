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

#ifndef SELECTABLE_RESOURCES_CONTAINER_HPP
#define SELECTABLE_RESOURCES_CONTAINER_HPP

#include <common/resources/resource_header.hpp>
#include <common/utilities/observable_container/observable_container.hpp>

namespace como {

class SelectableResourcesContainer : public ObservableContainer< ResourceID >
{
    public:
        /***
         * 1. Construction
         ***/
        SelectableResourcesContainer() = default;
        SelectableResourcesContainer( SelectableResourcesContainer& ) = delete;
        SelectableResourcesContainer( SelectableResourcesContainer&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SelectableResourcesContainer() = default;


        /***
         * 3. Getters
         ***/
        virtual ResourceHeadersList getSelectableResourcesHeaders() const = 0;
        virtual bool isResourceSelectable( const ResourceID& resourceID ) const = 0;



        /***
         * 4. Operators
         ***/
        SelectableResourcesContainer& operator = ( const SelectableResourcesContainer& ) = delete;
        SelectableResourcesContainer& operator = ( SelectableResourcesContainer&& ) = delete;
};

} // namespace como

#endif // SELECTABLE_RESOURCES_CONTAINER_HPP
