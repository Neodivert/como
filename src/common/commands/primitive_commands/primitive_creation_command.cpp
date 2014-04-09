#include "primitive_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveCreationCommand::PrimitiveCreationCommand() :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, 0, 0 )
{
    // Register the primitive's file as part of this CompositePackable.
    addPackable( &file_ );
}


PrimitiveCreationCommand::PrimitiveCreationCommand( PackableString< NAME_SIZE > filePath, UserID userID, PrimitiveID primitiveID ) :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, userID, primitiveID ),
    file_( filePath )
{
    // Register the primitive's file as part of this CompositePackable.
    addPackable( &file_ );
}


/***
 * 3. Getters
 ***/

const PackableFile* PrimitiveCreationCommand::getFile() const
{
    return &file_;
}



} // namespace como
