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

#ifndef TEXTURE_WALL_HANDLER_HPP
#define TEXTURE_WALL_HANDLER_HPP

#include <client/managers/utilities/server_writer.hpp>
#include <client/models/3d/textures/texture_wall.hpp>

namespace como {

class TextureWallHandler : public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallHandler( ServerInterfacePtr server, const ResourceID& textureWallID, TextureWall& textureWall );
        TextureWallHandler() = delete;
        TextureWallHandler( const TextureWallHandler& ) = delete;
        TextureWallHandler( TextureWallHandler&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TextureWallHandler() = default;


        /***
         * 3. Getters
         ***/
        glm::vec2 getTextureOffset() const;
        glm::vec2 getTextureScale() const;


        /***
         * 4. Setters
         ***/
        void setTextureID( const ResourceID& textureID );


        /***
         * 5. Operators
         ***/
        TextureWallHandler& operator = ( const TextureWallHandler& ) = delete;
        TextureWallHandler& operator = ( TextureWallHandler&& ) = delete;


    private:
        ResourceID textureWallID_;
        TextureWall* textureWall_;
};

} // namespace como

#endif // TEXTURE_WALL_HANDLER_HPP
