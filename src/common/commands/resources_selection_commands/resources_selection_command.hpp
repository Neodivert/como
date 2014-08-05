#ifndef RESOURCES_SELECTION_COMMAND_HPP
#define RESOURCES_SELECTION_COMMAND_HPP

#include <common/commands/type_command.hpp>

namespace como {

enum class ResourcesSelectionCommandType : std::uint8_t {
    SELECTION_UNLOCK = 0,
    SELECTION_DELETION
};


class ResourcesSelectionCommand : public TypeCommand< ResourcesSelectionCommandType >
{
    public:
        /***
         * 1. Construction
         ***/
        ResourcesSelectionCommand( ResourcesSelectionCommandType commandType );
    protected:
        ResourcesSelectionCommand( ResourcesSelectionCommandType commandType, UserID userID );
    public:
        ResourcesSelectionCommand() = delete;
        ResourcesSelectionCommand( const ResourcesSelectionCommand& ) = delete;
        ResourcesSelectionCommand( ResourcesSelectionCommand&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~ResourcesSelectionCommand() = default;


        /***
         * 3. Operators
         ***/
        ResourcesSelectionCommand& operator = ( const ResourcesSelectionCommand& ) = delete;
        ResourcesSelectionCommand& operator = ( ResourcesSelectionCommand&& ) = delete;
};


typedef std::shared_ptr< const ResourcesSelectionCommand > ResourcesSelectionCommandConstPtr;

} // namespace como

#endif // RESOURCES_SELECTION_COMMAND_HPP
