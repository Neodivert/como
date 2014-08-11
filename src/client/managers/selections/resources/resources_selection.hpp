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
        ~ResourcesSelection() = default;


        /***
         * 3. Operators
         ***/
        ResourcesSelection& operator = ( const ResourcesSelection& ) = delete;
        ResourcesSelection& operator = ( ResourcesSelection&& ) = delete;


    protected:
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


    private:
        std::map< ResourceID, std::shared_ptr< ResourceType > > resources_;
};


/***
 * 4. Resources insertion / removal
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::addResource( ResourceID id, std::shared_ptr< ResourceType > resource )
{
    resources_[id] = resource;

    notifyElementInsertion( id );
}


template <class ResourceType>
void ResourcesSelection<ResourceType>::removeResource( ResourceID id )
{
    resources_.erase( id );

    notifyElementDeletion( id );
}


/***
 * 5. Resources movement
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::moveResource( ResourceID resourceID, ResourcesSelection<ResourceType>& dstSelection )
{
    dstSelection.addResource( resourceID, resources_.at( resourceID ) );
    removeResource( resourceID );
}


template <class ResourceType>
void ResourcesSelection<ResourceType>::moveAll( ResourcesSelection<ResourceType>& dstSelection )
{
    for( auto resourcePair : resources_ ){
        moveResource( resourcePair.first, dstSelection );
    }
}


/***
 * 6. Clearing
 ***/

template <class ResourceType>
void ResourcesSelection<ResourceType>::clear()
{
    for( auto resourcePair : resources_ ){
        notifyElementDeletion( resourcePair.first );
    }

    resources_.clear();
}

}

#endif // RESOURCES_SELECTION_HPP
