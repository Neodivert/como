#ifndef ABSTRACT_RESOURCES_OWNERSHIP_MANAGER_HPP
#define ABSTRACT_RESOURCES_OWNERSHIP_MANAGER_HPP

#include <common/commands/resource_commands/resource_commands.hpp>
#include <common/commands/resources_selection_commands/resources_selection_command.hpp>

namespace como {

class AbstractResourcesOwnershipManager
{
    public:
        /***
         * 1. Construction
         ***/
        AbstractResourcesOwnershipManager() = default;
        AbstractResourcesOwnershipManager( const AbstractResourcesOwnershipManager& ) = default;
        AbstractResourcesOwnershipManager( AbstractResourcesOwnershipManager&& ) = default;


        /***
         * 2. Destruction
         ***/
        ~AbstractResourcesOwnershipManager() = default;


        /***
         * 3. Command execution
         ***/
        void executeResourceCommand( ResourceCommandConstPtr );
        void executeResourcesSelectionCommand( ResourcesSelectionCommandConstPtr );


        /***
         * 4. Operators
         ***/
        AbstractResourcesOwnershipManager& operator = ( const AbstractResourcesOwnershipManager& ) = delete;
        AbstractResourcesOwnershipManager& operator = ( AbstractResourcesOwnershipManager&& ) = delete;


    protected:
        /***
         * 6. Resource management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID userID ) = 0;
        virtual void unlockResourcesSelection( UserID userID ) = 0;
        virtual void deleteResourcesSelection( UserID userID ) = 0;
};

} // namespace como

#endif // ABSTRACT_RESOURCES_OWNERSHIP_MANAGER_HPP
