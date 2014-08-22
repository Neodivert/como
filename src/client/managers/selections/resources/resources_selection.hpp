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

#ifndef RESOURCES_SELECTION_HPP
#define RESOURCES_SELECTION_HPP

#include <map>
#include <common/ids/resource_id.hpp>
#include <common/utilities/observable_container/observable_container.hpp>
#include <memory>

namespace como {

template <class ResourceType>
class ResourcesSelection : public ObservableContainer<ResourceID> {
    public:
        /***
         * 1. Construction
         ***/
        ResourcesSelection() = default;
        ResourcesSelection( const ResourcesSelection& ) = default;
        ResourcesSelection( ResourcesSelection&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~ResourcesSelection() = default;


        /***
         * 3. Operators
         ***/
        ResourcesSelection& operator = ( const ResourcesSelection& ) = delete;
        ResourcesSelection& operator = ( ResourcesSelection&& ) = delete;


        /***
         * 4. Resources insertion / removal
         ***/
        void addResource( ResourceID id, std::shared_ptr< ResourceType > resource );
        void removeResource( ResourceID id );


        /***
         * 5. Resources movement
         ***/
        void moveResource( ResourceID resourceID, ResourcesSelection<ResourceType>& dstSelection );
        void moveAll( ResourcesSelection<ResourceType>& dstSelection );


        /***
         * 6. Clearing
         ***/
        void clear();


        /***
         * 7. Getters
         ***/
        virtual unsigned int size() const;
        bool containsResource( const ResourceID& resourceID ) const;


    protected:
        std::map< ResourceID, std::shared_ptr< ResourceType > > resources_;
};


/***
 * 4. Resources insertion / removal
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::addResource( ResourceID id, std::shared_ptr< ResourceType > resource )
{
    this->resources_[id] = resource;

    this->notifyElementInsertion( id );
}


template <class ResourceType>
void ResourcesSelection<ResourceType>::removeResource( ResourceID id )
{
    this->resources_.erase( id );

    this->notifyElementDeletion( id );
}


/***
 * 5. Resources movement
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::moveResource( ResourceID resourceID, ResourcesSelection<ResourceType>& dstSelection )
{
    dstSelection.addResource( resourceID, this->resources_.at( resourceID ) );
    this->removeResource( resourceID );
}


template <class ResourceType>
void ResourcesSelection<ResourceType>::moveAll( ResourcesSelection<ResourceType>& dstSelection )
{
    for( auto& resourcePair : this->resources_ ){
        this->moveResource( resourcePair.first, dstSelection );
    }
}


/***
 * 6. Clearing
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::clear()
{
    for( auto resourcePair : resources_ ){
        this->notifyElementDeletion( resourcePair.first );
    }

    this->resources_.clear();
}


/***
 * 7. Getters
 ***/

template <class ResourceType>
unsigned int ResourcesSelection<ResourceType>::size() const
{
    return this->resources_.size();
}


template <class ResourceType>
bool ResourcesSelection<ResourceType>::containsResource(const ResourceID &resourceID) const
{
    return ( this->resources_.count( resourceID ) != 0 );
}

}

#endif // RESOURCES_SELECTION_HPP
