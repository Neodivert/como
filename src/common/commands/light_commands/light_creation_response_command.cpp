#include "light_creation_response_command.hpp"

namespace como {


/***
 * 1. Construction
 ***/

LightCreationResponseCommand::LightCreationResponseCommand() :
    LightCommand( LightCommandType::LIGHT_CREATION_RESPONSE, NO_USER, NO_RESOURCE ),
    response_( false )
{
    addPackable( &response_ );
}


LightCreationResponseCommand::LightCreationResponseCommand( const ResourceID& lightID, bool response, UserID userID ) :
    LightCommand( LightCommandType::LIGHT_CREATION_RESPONSE, userID, lightID ),
    response_( response )
{
    addPackable( &response_ );
}


LightCreationResponseCommand::LightCreationResponseCommand( const LightCreationResponseCommand& b ) :
    LightCommand( b ),
    response_( b.response_ )
{
    addPackable( &response_ );
}


/***
 * 3. Getters
 ***/

bool LightCreationResponseCommand::getResponse() const
{
    return response_.getValue();
}


} // namespace como
