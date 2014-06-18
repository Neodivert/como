#include "primitive_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveCreationCommand::PrimitiveCreationCommand() :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, 0, 0 )
{
    addPackable( &category_ );
    addPackable( &file_ );
}


PrimitiveCreationCommand::PrimitiveCreationCommand( PackableString< NAME_SIZE > filePath, UserID userID, PrimitiveID primitiveID, ResourceID categoryID ) :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, userID, primitiveID ),
    category_( categoryID ),
    file_( filePath )
{
    addPackable( &category_ );
    addPackable( &file_ );
}


PrimitiveCreationCommand::PrimitiveCreationCommand( const PrimitiveCreationCommand& b ) :
    PrimitiveCommand( b ),
    file_( b.file_ )
{
    addPackable( &category_ );
    addPackable( &file_ );
}


/***
 * 3. Getters
 ***/

std::string PrimitiveCreationCommand::getPrimitiveName() const
{
    return file_.getFileName();
}


ResourceID PrimitiveCreationCommand::getCategoryID() const
{
    return category_.getValue();
}


const PackableFile* PrimitiveCreationCommand::getFile() const
{
    return &file_;
}



} // namespace como
