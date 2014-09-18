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

#ifndef TEXTURE_WALLS_MANAGER_HPP
#define TEXTURE_WALLS_MANAGER_HPP

#include <map>
#include <list>
#include <common/resources/resource_header.hpp>
#include <client/models/3d/textures/texture_wall.hpp>
#include <client/managers/utilities/server_writer.hpp>
#include <client/managers/managers/textures/textures_manager.hpp>
#include <common/resources/selectable_resources_container.hpp>

namespace como {

class TextureWallsManager;
typedef std::unique_ptr< TextureWallsManager > TextureWallsManagerPtr;

class TextureWallsManager : public ResourcesOwnershipRequester, public SelectableResourcesContainer
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallsManager( ServerInterfacePtr server, TexturesManager& texturesManager );
        TextureWallsManager() = delete;
        TextureWallsManager( const TextureWallsManager& ) = delete;
        TextureWallsManager( TextureWallsManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TextureWallsManager() = default;


        /***
         * 3. Getters
         ***/
        bool textureWallIncludesTexture( const ResourceID& textureWallID ) const;
        virtual ResourceHeadersList getSelectableResourcesHeaders() const;
        virtual bool isResourceSelectable( const ResourceID& resourceID ) const;
        virtual string getResourceName( const ResourceID &resourceID ) const;


        /***
         * 4. Local texture walls management
         ***/
        ResourceID createTextureWall( std::string name );
        void toggleTextureWallSeletable( const ResourceID& textureWallID, bool selectable );


        /***
         * 5. Resources ownership requesting
         ***/
        virtual void requestResourceLock( const ResourceID &resourceID );


        /***
         * 6. Shader communication
         ***/
        void sendTextureWallToShader( const ResourceID& resourceID ) const;


        /***
         * 7. Operators
         ***/
        TextureWallsManager& operator = ( const TextureWallsManager& ) = delete;
        TextureWallsManager& operator = ( TextureWallsManager&& ) = delete;


    protected:
        /***
         * 8. Resources ownership management
         ***/
        virtual void lockResource( const ResourceID &resourceID, UserID newOwner );
        virtual void unlockResourcesSelection( UserID currentOwner );
        virtual void clearResourcesSelection( UserID currentOwner );


    private:
        TexturesManager* texturesManager_;

        std::map< ResourceID, TextureWall > textureWalls_;
        std::list< ResourceID > selectableTextureWalls_;
};

} // namespace como

#endif // TEXTURE_WALLS_MANAGER_HPP
