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
#include <queue>
#include <common/resources/resource.hpp>
#include <common/utilities/lockable.hpp>

namespace como {

template <class ResourceType>
class ResourcesSelection : public virtual Observable, public Lockable {

    static_assert( std::is_base_of<Resource, ResourceType>::value,
                   "ResourcesSelection - T must be a descendant of Resource" );

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
        virtual void addResource( ResourceID id, std::unique_ptr< ResourceType > resource, bool notifyObservers = true );
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
        ResourceHeadersList headers() const;


    protected:
        std::map< ResourceID, std::unique_ptr< ResourceType > > resources_;
};


/***
 * 4. Resources insertion / removal
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::addResource( ResourceID id, std::unique_ptr< ResourceType > resource, bool notifyObservers )
{
    lock();
    this->resources_[id] = std::move( resource );

    if( notifyObservers ){
        this->notifyObservers();
    }
}


template <class ResourceType>
void ResourcesSelection<ResourceType>::removeResource( ResourceID id )
{
    lock();
    this->resources_.erase( id );

    this->notifyObservers();
}


/***
 * 5. Resources movement
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::moveResource( ResourceID resourceID, ResourcesSelection<ResourceType>& dstSelection )
{
    // TODO: lock(); or deadlock?
    // Don't notify observers when adding resource to destiny.
    // Observers may query source selection, which holds a
    // null "moved" pointer at that time.
    dstSelection.addResource( resourceID, std::move( this->resources_.at( resourceID ) ), false );
    this->removeResource( resourceID );
    dstSelection.notifyObservers();
}


template <class ResourceType>
void ResourcesSelection<ResourceType>::moveAll( ResourcesSelection<ResourceType>& dstSelection )
{
    // TODO: lock(); or deadlock?
    // TODO: Maybe a more efficient way?
    for( auto& resourcePair : this->resources_ ){
        dstSelection.addResource( resourcePair.first, std::move( resourcePair.second ), false );
    }
    this->clear();
}


/***
 * 6. Clearing
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::clear()
{
    this->resources_.clear();

    this->notifyObservers();
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


template <class ResourceType>
ResourceHeadersList ResourcesSelection<ResourceType>::headers() const
{
    ResourceHeadersList headers;

    for( const Resource& resource : resources_ ){
        headers.push_back( resource.header() );
    }

    return headers;
}

}

#endif // RESOURCES_SELECTION_HPP
