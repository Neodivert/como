#include "primitive_creation_command.hpp"

namespace como {

#define ADD_PACKABLES \
    addPackable( &category_ ); \
    addPackable( &name_ ); \
    addPackable( &primitiveFile_ );

/***
 * 1. Construction
 ***/

PrimitiveCreationCommand::PrimitiveCreationCommand( const std::string& unpackingDirPath ) :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, 0, {0, 0} ),
    primitiveFile_( unpackingDirPath )
{
    ADD_PACKABLES
}


PrimitiveCreationCommand::PrimitiveCreationCommand( UserID userID, ResourceID primitiveID, PrimitiveInfo primitive, const std::string& unpackingDirPath ) :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, userID, primitiveID ),
    category_( primitive.category ),
    name_( primitive.name.c_str() ),
    primitiveFile_( unpackingDirPath, primitive.filePath )

{
    ADD_PACKABLES
}


PrimitiveCreationCommand::PrimitiveCreationCommand( const PrimitiveCreationCommand& b ) :
    PrimitiveCommand( b ),
    category_( b.category_ ),
    name_( b.name_ ),
    primitiveFile_( b.primitiveFile_ )
{
    ADD_PACKABLES
}


/***
 * 3. Getters
 ***/

PrimitiveInfo PrimitiveCreationCommand::getPrimitiveInfo() const
{
    PrimitiveInfo primitive;

    primitive.category = category_.getValue();
    primitive.name = name_.getValue();
    primitive.filePath = primitiveFile_.getFilePath().c_str();

    return primitive;
}


bool PrimitiveCreationCommand::includesTexture() const
{
    // TODO: Change this.
    return false;
}


} // namespace como
