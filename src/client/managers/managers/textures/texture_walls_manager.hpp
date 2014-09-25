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
#include <client/managers/managers/textures/texture_wall_handler.hpp>

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
        TextureWallHandler* getCurrentTextureWall() const;


        /***
         * 4. Local texture walls management
         ***/
        ResourceID createTextureWall( std::string name, const ResourceID& meshID );
        void createTextureWall( std::string name, const ResourceID& textureWallID, const ResourceID& meshID );
        void toggleTextureWallSeletable( const ResourceID& textureWallID, bool selectable );
        void toggleMeshTextureWallsSeletable( const ResourceID& meshID, bool selectable );
        void unlockSelectableTextureWalls();
        void removeSelectableTextureWalls();
        TextureWallHandler* selectTextureWall( const ResourceID& textureWallID );
        bool thereIsSelectableTextureWalls() const;


        /***
         * 5. Resources ownership requesting
         ***/
        virtual void requestResourceLock( const ResourceID &resourceID );


        /***
         * 6. Remote command execution
         ***/
        void executeRemoteCommand( const TextureWallCommand& command );


        /***
         * 7. Shader communication
         ***/
        void sendTextureWallToShader( const ResourceID& resourceID ) const;


        /***
         * 8. Operators
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

        // TODO: Rename this and related methods so they refer to TextureWalls'
        // "parent resources" instead of meshes.
        std::map< ResourceID, std::list< ResourceID > > meshesTextureWalls_;

        std::unique_ptr< TextureWallHandler > currentTextureWallHandler_;

        std::list< ResourceID > selectableTextureWalls_;
};

} // namespace como

#endif // TEXTURE_WALLS_MANAGER_HPP
