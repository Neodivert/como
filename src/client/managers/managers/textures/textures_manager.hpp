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

#ifndef TEXTURE_MANAGER_HPP
#define TEXTURE_MANAGER_HPP

#include <common/managers/textures/abstract_textures_manager.hpp>
#include <client/managers/utilities/server_writer.hpp>
#include <common/ids/resource_id.hpp>
#include <client/models/3d/textures/texture_wall.hpp>
#include <map>
#include <client/models/utilities/open_gl.hpp>
#include <client/managers/managers/resources/resources_ownership_requester.hpp>
#include <common/resources/resource_header.hpp>
#include <common/utilities/observable_container/observable_container.hpp>
#include <common/commands/texture_commands/texture_commands.hpp>

namespace como {

class TexturesManager;
typedef std::unique_ptr< TexturesManager > TexturesManagerPtr;

class TexturesManager : public AbstractTexturesManager, public ServerWriter, public ObservableContainer<ResourceID>
{
    public:
        /***
         * 1. Construction
         ***/
        TexturesManager( OpenGL& openGL, ServerInterfacePtr server, const std::string& sceneDirPath, const std::string& tempDirPath );
        TexturesManager() = delete;
        TexturesManager( const TexturesManager& ) = delete;
        TexturesManager( TexturesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TexturesManager() = default;


        /***
         * 4. Local textures management
         ***/
        ResourceID loadTexture( std::string imagePath );


        /***
         * 5. Getters
         ***/
        std::list< TextureData > getTexturesData() const;
        TextureData getTextureData( const ResourceID& textureID ) const;


        /***
         * 6. Command execution
         ***/
        void executeRemoteCommand( const TextureCommand& command );


        /***
         * 7. Shader communication
         ***/
        void sendTextureToShader( const ResourceID& resourceID,
                                  glm::vec2 textureOffset = glm::vec2( 0.0f ),
                                  glm::vec2 textureScale = glm::vec2( 1.0f ) ) const;


        /***
         * 7. Operators
         ***/
        TexturesManager& operator = ( const TexturesManager& ) = delete;
        TexturesManager& operator = ( TexturesManager&& ) = delete;


    private:
        /***
         * 8. Remote textures management
         ***/
        void loadTexture( const ResourceID& textureID, std::string imagePath );

        std::map< ResourceID, TexturePtr > textures_;
        std::string tempDirPath_;
};

} // namespace como

#endif // TEXTURE_MANAGER_HPP
