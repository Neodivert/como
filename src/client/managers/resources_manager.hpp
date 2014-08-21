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

#ifndef RESOURCES_MANAGER_HPP
#define RESOURCES_MANAGER_HPP

#include <common/utilities/observable_container/observable_container.hpp>
#include <common/commands/command.hpp>
#include <client/managers/selections/resources/resources_selection.hpp>
#include <client/managers/selections/resources/local_resources_selection.hpp>
#include <client/managers/resources/resource_commands_executer.hpp>


namespace como {

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
class ResourcesManager : public virtual ResourceCommandsExecuter, public Observable, public Observer
{
    //static_assert( std::is_base_of<ResourcesSelection<ResourceType>, ResourcesSelectionType>::value, "" );
    //static_assert( std::is_base_of<LocalResourcesSelection<ResourceType>, LocalResourcesSelectionType>::value, "" );
    //static_assert( std::is_base_of<ResourcesSelection<ResourceType>, LocalResourcesSelection<ResourceType>>::value, "" );


    public:
        /***
         * 1. Construction
         ***/
        ResourcesManager( ServerInterfacePtr server, LogPtr log );
        ResourcesManager() = delete;
        ResourcesManager( const ResourcesManager& ) = delete;
        ResourcesManager( ResourcesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesManager();


        /***
         * 3. Getters
         ***/
        std::shared_ptr< LocalResourcesSelectionType > getLocalResourcesSelection() const;
        virtual std::string getResourceName( const ResourceID& resourceID ) const = 0;


        /***
         * 5. Observer pattern
         ***/
        virtual void update();
        void addResourcesSelectionObserver( UserID userID, Observer* observer );
        void removeResourcesSelectionObserver( UserID userID, Observer* observer );


        /***
         * 6. Operators
         ***/
        ResourcesManager& operator = ( const ResourcesManager& ) = delete;
        ResourcesManager& operator = ( ResourcesManager&& ) = delete;


        /***
         * 7. Selections management
         ***/
        template <typename... Args>
        void createResourcesSelection( UserID userID, Args... args );

        void removeResourcesSelection( UserID userID );


    protected:
        /***
         * 8. Protected getters
         ***/
        std::shared_ptr< ResourcesSelectionType > getResourcesSelection( UserID userID ) const;


        /***
         * 9. Resource management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID newOwner );
        virtual void unlockResourcesSelection( UserID userID );
        virtual void clearResourcesSelection(UserID currentOwner);

    protected:
        std::map< UserID, std::shared_ptr< ResourcesSelectionType > > resourcesSelections_;

        std::shared_ptr< ResourcesSelectionType > nonSelectedResources_;
};


/***
 * 1. Construction
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::ResourcesManager( ServerInterfacePtr server, LogPtr log ) :
    ResourceCommandsExecuter( server )
{
    // TODO: Remove log (or not?).
    (void)(log);

    createResourcesSelection( NO_USER );
    nonSelectedResources_ = resourcesSelections_.at( NO_USER );

    std::shared_ptr< LocalResourcesSelectionType > localResourcesSelection( new LocalResourcesSelectionType( server ) );
    localResourcesSelection->Observable::addObserver( this );
    resourcesSelections_[localUserID()] = localResourcesSelection;
}


/***
 * 2. Destruction
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::~ResourcesManager()
{
    for( auto& resourcesSelectionPair : resourcesSelections_ ){
        resourcesSelectionPair.second->Observable::removeObserver( this );
    }
}


/***
 * 3. Getters
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
std::shared_ptr<LocalResourcesSelectionType> ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::getLocalResourcesSelection() const
{
    return std::static_pointer_cast< LocalResourcesSelectionType >( resourcesSelections_.at( localUserID() ) );
}


/***
 * 5. Observer pattern
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::update()
{
    // Simply forward the notification.
    notifyObservers();
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::addResourcesSelectionObserver( UserID userID, Observer *observer )
{
    resourcesSelections_.at( userID )->Observable::addObserver( observer );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::removeResourcesSelectionObserver( UserID userID, Observer *observer )
{
    resourcesSelections_.at( userID )->Observable::removeObserver( observer );
}


/***
 * 7. Selections management
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
template <typename... Args>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::createResourcesSelection( UserID userID, Args... args )
{
    resourcesSelections_[userID] = std::shared_ptr< ResourcesSelectionType >( new ResourcesSelectionType( args... ) );

    addResourcesSelectionObserver( userID, this );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::removeResourcesSelection(UserID userID)
{
    resourcesSelections_.erase( userID );
}


/***
 * 8. Protected getters
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
std::shared_ptr<ResourcesSelectionType> ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::getResourcesSelection( UserID userID ) const
{
    return resourcesSelections_.at( userID );
}


/***
 * 9. Resource management
 ***/

template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::lockResource( const ResourceID& resourceID, UserID newOwner )
{
    this->getResourcesSelection( NO_USER )->moveResource( resourceID, *( this->getResourcesSelection( newOwner ) ) );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::unlockResourcesSelection( UserID currentOwner )
{
    this->getResourcesSelection( currentOwner )->moveAll( *( this->getResourcesSelection( NO_USER ) ) );
}


template <class ResourceType, class ResourcesSelectionType, class LocalResourcesSelectionType>
void ResourcesManager<ResourceType, ResourcesSelectionType, LocalResourcesSelectionType>::clearResourcesSelection( UserID currentOwner )
{
    this->getResourcesSelection( currentOwner )->clear();
}

} // namespace como

#endif // RESOURCES_MANAGER_HPP
