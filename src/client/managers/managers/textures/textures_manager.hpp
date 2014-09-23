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

namespace como {

class TexturesManager;
typedef std::unique_ptr< TexturesManager > TexturesManagerPtr;

class TexturesManager : public AbstractTexturesManager, public ServerWriter, public ObservableContainer<ResourceID>
{
    public:
        /***
         * 1. Construction
         ***/
        TexturesManager( ServerInterfacePtr server, const std::string& sceneDirPath );
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


        /***
         * 6. Shader communication
         ***/
        void sendTextureToShader( const ResourceID& resourceID ) const;


        /***
         * 7. Operators
         ***/
        TexturesManager& operator = ( const TexturesManager& ) = delete;
        TexturesManager& operator = ( TexturesManager&& ) = delete;


    private:
        std::map< ResourceID, TexturePtr > textures_;
};

} // namespace como

#endif // TEXTURE_MANAGER_HPP
