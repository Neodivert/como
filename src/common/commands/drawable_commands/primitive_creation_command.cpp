#include "primitive_creation_command.hpp"

namespace como {

/***
 * 1. Construction
 ***/

PrimitiveCreationCommand::PrimitiveCreationCommand() :
    DrawableCommand( DrawableCommandType::PRIMITIVE_CREATION, 0, NULL_DRAWABLE_ID )
{
}


PrimitiveCreationCommand::PrimitiveCreationCommand( PackableString< NAME_SIZE > filePath, UserID userID, PackableDrawableID drawableID ) :
    DrawableCommand( DrawableCommandType::PRIMITIVE_CREATION, userID, drawableID ),
    file_( filePath )
{
}


/***
 * 3. Getters
 ***/

const PackableFile* PrimitiveCreationCommand::getFile() const
{
    return &file_;
}



} // namespace como