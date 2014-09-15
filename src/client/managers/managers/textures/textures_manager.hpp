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

#include <client/managers/utilities/server_writer.hpp>
#include <common/ids/resource_id.hpp>
#include <client/models/3d/textures/texture_wall.hpp>
#include <map>

namespace como {

class TexturesManager;
typedef std::unique_ptr< TexturesManager > TexturesManagerPtr;

class TexturesManager : public ServerWriter
{
    public:
        /***
         * 1. Construction
         ***/
        TexturesManager( ServerInterfacePtr server );
        TexturesManager() = delete;
        TexturesManager( const TexturesManager& ) = delete;
        TexturesManager( TexturesManager&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TexturesManager() = default;


        /***
         * 3. Textures management
         ***/
        ResourceID loadTexture( std::string imagePath );


        /***
         * 4. Operators
         ***/
        TexturesManager& operator = ( const TexturesManager& ) = delete;
        TexturesManager& operator = ( TexturesManager&& ) = delete;
};

} // namespace como

#endif // TEXTURE_MANAGER_HPP
