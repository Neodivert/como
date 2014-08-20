#include "coloured_user.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ColouredUser::ColouredUser( const UserConnectionCommand& command ) :
    User( command.getUserID(), command.getName() ),
    color_( command.getSelectionColor().toVec4() )
{}


/***
 * 3. Getters
 ***/

glm::vec4 ColouredUser::color() const
{
    return color_;
}

} // namespace como
