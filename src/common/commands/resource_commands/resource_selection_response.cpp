#include "resource_selection_response.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourceSelectionResponse::ResourceSelectionResponse() :
    ResourceCommand( ResourceCommandType::RESOURCE_SELECTION_RESPONSE ),
    selectionResponse_( false )
{
    addPackable( &selectionResponse_ );
}


ResourceSelectionResponse::ResourceSelectionResponse( const ResourceID& resourceID, bool response ) :
    ResourceCommand( ResourceCommandType::RESOURCE_SELECTION_RESPONSE, NO_USER, resourceID ),
    selectionResponse_( response )
{
    addPackable( &selectionResponse_ );
}


ResourceSelectionResponse::ResourceSelectionResponse( const ResourceSelectionResponse& b ) :
    ResourceCommand( b ),
    selectionResponse_( b.selectionResponse_ )
{
    addPackable( &selectionResponse_ );
}


/***
 * 3. Getters
 ***/

bool ResourceSelectionResponse::getResponse() const
{
    return selectionResponse_.getValue();
}

} // namespace como
