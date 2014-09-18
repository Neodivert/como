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

#ifndef TEXTURE_WALL_EDITOR_HPP
#define TEXTURE_WALL_EDITOR_HPP

#include <QFrame>
#include <client/managers/managers/textures/texture_wall_handler.hpp>
#include <common/utilities/observer_pattern/observer.hpp>

namespace como {

class TextureWallEditor : public QFrame, public Observer
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallEditor( TextureWallHandler* textureWallHandler );
        TextureWallEditor() = delete;
        TextureWallEditor( const TextureWallEditor& ) = delete;
        TextureWallEditor( TextureWallEditor&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TextureWallEditor() = default;


        /***
         * 3. Updating (observer pattern)
         ***/
        virtual void update();


        /***
         * 4. Operators
         ***/
        TextureWallEditor& operator = ( const TextureWallEditor& ) = delete;
        TextureWallEditor& operator = ( TextureWallEditor&& ) = delete;


    private:
        TextureWallHandler* textureWallHandler_;
};

} // namespace como

#endif // TEXTURE_WALL_EDITOR_HPP
