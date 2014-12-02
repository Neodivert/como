#ifndef ABSTRACT_RESOURCES_OWNERSHIP_MANAGER_HPP
#define ABSTRACT_RESOURCES_OWNERSHIP_MANAGER_HPP

#include <common/commands/resource_commands/resource_commands.hpp>
#include <common/commands/resources_selection_commands/resources_selection_command.hpp>
#include <common/utilities/log.hpp>
#include <common/utilities/lockable.hpp>

namespace como {

class AbstractResourcesOwnershipManager : public Lockable
{
    private:
        LogPtr log_;

    public:
        /***
         * 1. Construction
         ***/
        AbstractResourcesOwnershipManager( LogPtr log );
        AbstractResourcesOwnershipManager() = delete;
        AbstractResourcesOwnershipManager( const AbstractResourcesOwnershipManager& ) = default;
        AbstractResourcesOwnershipManager( AbstractResourcesOwnershipManager&& ) = default;


        /***
         * 2. Destruction
         ***/
        virtual ~AbstractResourcesOwnershipManager() = default;


        /***
         * 3. Command execution
         ***/
        void executeResourceCommand( const ResourceCommand& );
        void executeResourcesSelectionCommand( const ResourcesSelectionCommand& );


        /***
         * 4. Operators
         ***/
        AbstractResourcesOwnershipManager& operator = ( const AbstractResourcesOwnershipManager& ) = delete;
        AbstractResourcesOwnershipManager& operator = ( AbstractResourcesOwnershipManager&& ) = delete;


    protected:
        /***
         * 5. Getters
         ***/
        LogPtr log();


        /***
         * 6. Resource management
         ***/
        virtual void lockResource( const ResourceID& resourceID, UserID userID ) = 0;
        virtual void unlockResourcesSelection( UserID userID ) = 0;
        virtual void deleteResourcesSelection( UserID userID ) = 0;
        virtual void processLockDenial( const ResourceID& resourceID ) = 0;
};

} // namespace como

#endif // ABSTRACT_RESOURCES_OWNERSHIP_MANAGER_HPP
