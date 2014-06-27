#include "primitive_creation_command.hpp"

namespace como {

#define ADD_PACKABLES \
    addPackable( &category_ ); \
    addPackable( &name_ ); \
    addPackable( &meshFile_ ); \
    addPackable( &materialFile_ ); \
    addPackable( &includesTexture_ ); \
    addPackable( &textureFile_ );

/***
 * 1. Construction
 ***/

PrimitiveCreationCommand::PrimitiveCreationCommand() :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, 0, {0, 0} ),
    textureFile_( std::unique_ptr<PackableFile>( new PackableFile ), [&](){ return includesTexture_.getValue(); } )
{
    ADD_PACKABLES
}


PrimitiveCreationCommand::PrimitiveCreationCommand( UserID userID, ResourceID primitiveID, PrimitiveInfo primitive ) :
    PrimitiveCommand( PrimitiveCommandType::PRIMITIVE_CREATION, userID, primitiveID ),
    category_( primitive.category ),
    name_( primitive.name.c_str() ),
    meshFile_( ( primitive.directory + "/" + primitive.meshFileName ).c_str() ),
    materialFile_( ( primitive.directory + "/" + primitive.materialFileName ).c_str() ),
    includesTexture_( ( primitive.textureFileName.size() ) && primitive.textureFileName != "." ),
    textureFile_( std::unique_ptr<PackableFile>( new PackableFile( ( primitive.directory + "/" + primitive.textureFileName ).c_str() ) ), [&](){ return includesTexture_.getValue(); } )
{
    ADD_PACKABLES
}


PrimitiveCreationCommand::PrimitiveCreationCommand( const PrimitiveCreationCommand& b ) :
    PrimitiveCommand( b ),
    category_( b.category_ ),
    name_( b.name_ ),
    meshFile_( b.meshFile_ ),
    materialFile_( b.materialFile_ ),
    includesTexture_( b.includesTexture_ ),
    textureFile_( std::unique_ptr<PackableFile>( new PackableFile( ( b.getPrimitiveInfo().directory + "/" + b.getPrimitiveInfo().textureFileName ).c_str() ) ), [&](){ return includesTexture_.getValue(); } ) // TODO: Copying a fstream? Dangerous?
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
    primitive.meshFileName = meshFile_.getFileName().c_str();
    primitive.directory = meshFile_.getFilePath().substr( 0, meshFile_.getFilePath().rfind( '/' ) );
    std::cout << "primitive.directory: [" << primitive.directory << "]" << std::endl;
    primitive.materialFileName = materialFile_.getFileName().c_str();

    const PackableFile* textureFile = textureFile_.getInnerPackable();
    if( textureFile ){
        primitive.textureFileName = textureFile->getFileName().c_str();
    }else{
        primitive.textureFileName = "";
    }

    return primitive;
}


bool PrimitiveCreationCommand::includesTexture() const
{
    return includesTexture_.getValue();
}

} // namespace como
