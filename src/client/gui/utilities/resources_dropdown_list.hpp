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

#ifndef RESOURCES_DROP_DOWNLIST_HPP
#define RESOURCES_DROP_DOWNLIST_HPP

#include <QComboBox>
#include <map>
#include <common/ids/resource_id.hpp>
#include <common/utilities/observable_container/container_observer.hpp>
#include <common/resources/selectable_resources_container.hpp>

namespace como {

class ResourcesDropdownList : public QComboBox, public ContainerObserver<ResourceID>
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        ResourcesDropdownList( SelectableResourcesContainer& resourcesContainer );
        ResourcesDropdownList() = delete;
        ResourcesDropdownList( const ResourcesDropdownList& ) = delete;
        ResourcesDropdownList( ResourcesDropdownList&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesDropdownList() = default;


        /***
         * 3. Updating (observer pattern)
         ***/
        virtual void update( ContainerAction lastContainerAction, ResourceID lastElementModified );


        /***
         * 4. Operators
         ***/
        ResourcesDropdownList& operator = ( const ResourcesDropdownList& ) = delete;
        ResourcesDropdownList& operator = ( ResourcesDropdownList&& ) = delete;


    signals:
        /***
         * 5. Signals
         ***/
        void resourceSelected( ResourceID id );


    private:
        /***
         * 6. Resources management
         ***/
        void addResource( const ResourceID& id, const std::string& name );
        void removeResource( const ResourceID& id );


        // Base class QComboBox works with integer (int) indices, so we need
        // to map them to resource IDs.
        std::map< int, ResourceID > indexToID_;

        SelectableResourcesContainer* resourcesContainer_;
};

} // namespace como

#endif // RESOURCES_DROP_DOWNLIST_HPP
