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

#ifndef LOCAL_RESOURCES_SELECTION_HPP
#define LOCAL_RESOURCES_SELECTION_HPP

#include <client/managers/selections/resources/resources_selection.hpp>
#include <client/managers/server_interface/server_interface.hpp>
#include <client/managers/utilities/server_writer.hpp>
#include <common/commands/command.hpp>

namespace como {

// TODO: Remove this class?
template <class ResourceType>
class LocalResourcesSelection : public virtual ResourcesSelection< ResourceType >, public ServerWriter {
    public:
        /***
         * 1. Construction
         ***/
        LocalResourcesSelection( ServerInterfacePtr server );
        LocalResourcesSelection() = delete;
        LocalResourcesSelection( const LocalResourcesSelection& ) = default;
        LocalResourcesSelection( LocalResourcesSelection&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~LocalResourcesSelection() = default;


        /***
         * 3. Operators
         ***/
        LocalResourcesSelection& operator = ( const LocalResourcesSelection& ) = delete;
        LocalResourcesSelection& operator = ( LocalResourcesSelection&& ) = delete;


    public:
        /***
         * 4. Local resources insertion
         ***/
        ResourceID addResource( std::unique_ptr<ResourceType> resource );
};


/***
 * 1. Construction
 ***/

template <class ResourceType>
LocalResourcesSelection<ResourceType>::LocalResourcesSelection( ServerInterfacePtr server ) :
    ServerWriter( server )
{}


/***
 * 4. Local resources insertion
 ***/

template <class ResourceType>
ResourceID LocalResourcesSelection<ResourceType>::addResource( std::unique_ptr<ResourceType> resource )
{
    this->lock();
    ResourceID resourceID = this->reserveResourceIDs( 1 );

    this->ResourcesSelection<ResourceType>::addResource( resourceID, std::move( resource ) );

    return resourceID;
}

} // namespace como

#endif // LOCAL_RESOURCES_SELECTION_HPP
