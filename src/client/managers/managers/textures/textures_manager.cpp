/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "textures_manager.hpp"
#include <glm/gtc/type_ptr.hpp>

namespace como {


/***
 * 1. Construction
 ***/

TexturesManager::TexturesManager( OpenGL& openGL, ServerInterfacePtr server, const std::string& sceneDirPath, const std::string& tempDirPath ) :
    AbstractTexturesManager( sceneDirPath ),
    ServerWriter( server ),
    tempDirPath_( tempDirPath )
{
    textureOffsetShaderLocation_ =
            openGL.getShaderVariableLocation( "textureOffset" );
    assert( textureOffsetShaderLocation_ != -1 );

    textureScaleShaderLocation_ =
            openGL.getShaderVariableLocation( "textureScale" );
    assert( textureScaleShaderLocation_ != -1 );
}


/***
 * 4. Local textures management
 ***/

ResourceID TexturesManager::loadTexture( std::string imagePath )
{
    ResourceID textureID = reserveResourceIDs( 1 );

    loadTexture( textureID, imagePath );

    sendCommandToServer(
                CommandConstPtr(
                    new TextureCreationCommand(
                        textureID,
                        tempDirPath_,
                        imagePath ) ) );

    return textureID;
}


/***
 * 5. Getters
 ***/

std::list<TextureData> TexturesManager::getTexturesData() const
{
    std::list<TextureData> texturesDataList;

    for( const auto& texturePair : textures_ ){
        texturesDataList.push_back( texturePair.second->data() );
    }

    return texturesDataList;
}


/***
 * 6. Command execution
 ***/

void TexturesManager::executeRemoteCommand( const TextureCommand &command )
{
    switch( command.getType() ){
        case TextureCommandType::TEXTURE_CREATION:{
            const TextureCreationCommand& textureCreationCommand =
                dynamic_cast< const TextureCreationCommand& >( command );
            loadTexture( textureCreationCommand.textureID(),
                         textureCreationCommand.textureFilePath() );
        }break;
    }
}


/***
 * 7. Shader communication
 ***/

void TexturesManager::sendTextureToShader( const ResourceID& resourceID, glm::vec2 textureOffset, glm::vec2 textureScale ) const
{   
    // Texture offset is expressed as a %, so we multiply it by 0.01 for
    // transforming it to range [0.0f, 1.0f]
    textureOffset *= 0.01f;

    // Send texture's offset and scale to shader.
    glUniform2fv( textureOffsetShaderLocation_,
                  1,
                  glm::value_ptr( textureOffset ) );
    glUniform2fv( textureScaleShaderLocation_,
                  1,
                  glm::value_ptr( textureScale ) );

    // Send texture's pixels to shader.
    textures_.at( resourceID )->sendToShader();
}


/***
 * 8. Remote textures management
 ***/

void TexturesManager::loadTexture( const ResourceID &textureID, string imagePath )
{
    boost::system::error_code errorCode;
    std::string dstPath =
            TEXTURES_DIR_PATH_ +
            "/" +
            boost::filesystem::basename( imagePath ) +
            boost::filesystem::extension( imagePath );

    // TODO: If this is a remote command, file should be moved instead of being
    // copied.
    boost::filesystem::copy( imagePath, dstPath, errorCode );

    if( errorCode ){
        throw std::runtime_error( "ERROR copying file [" +
                                  imagePath +
                                  "] to [" +
                                  dstPath +
                                  "] - " +
                                  errorCode.message() );
    }

    textures_[textureID] = std::move( TexturePtr( new Texture( textureID, dstPath ) ) );

    notifyElementInsertion( textureID );
}

} // namespace como
