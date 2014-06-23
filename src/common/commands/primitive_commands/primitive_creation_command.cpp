#include "primitive_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveCreationCommand::PrimitiveCreationCommand() :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, 0, {0, 0} )
{
    addPackable( &category_ );
    addPackable( &meshFile_ );
    addPackable( &materialFile_ );
}


PrimitiveCreationCommand::PrimitiveCreationCommand( UserID userID, ResourceID primitiveID, ResourceID categoryID, PackableString< NAME_SIZE > meshFilePath, PackableString< NAME_SIZE > materialFilePath ) :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, userID, primitiveID ),
    category_( categoryID ),
    meshFile_( meshFilePath ),
    materialFile_( materialFilePath )
{
    addPackable( &category_ );
    addPackable( &meshFile_ );
    addPackable( &materialFile_ );
}


PrimitiveCreationCommand::PrimitiveCreationCommand( const PrimitiveCreationCommand& b ) :
    PrimitiveCommand( b ),
    meshFile_( b.meshFile_ ),
    materialFile_( b.materialFile_ )
{
    addPackable( &category_ );
    addPackable( &meshFile_ );
    addPackable( &materialFile_ );
}


/***
 * 3. Getters
 ***/

std::string PrimitiveCreationCommand::getPrimitiveName() const
{
    return getMeshFileName();
}


std::string PrimitiveCreationCommand::getMeshFileName() const
{
    return meshFile_.getFileName();
}


std::string PrimitiveCreationCommand::getMaterialFileName() const
{
    return materialFile_.getFileName();
}


ResourceID PrimitiveCreationCommand::getCategoryID() const
{
    return category_.getValue();
}


const PackableFile* PrimitiveCreationCommand::getMeshFile() const
{
    return &meshFile_;
}

const PackableFile* PrimitiveCreationCommand::getMaterialFile() const
{
    return &materialFile_;
}

} // namespace como
