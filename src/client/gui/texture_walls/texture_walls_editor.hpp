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

#ifndef TEXTURE_WALLS_EDITOR_HPP
#define TEXTURE_WALLS_EDITOR_HPP

#include <QFrame>
#include <client/managers/managers/textures/texture_walls_manager.hpp>

namespace como {

class TextureWallsEditor : public QFrame, public Observer
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        TextureWallsEditor( TextureWallsManager* textureWallsManager, TexturesManager* texturesManager );
        TextureWallsEditor() = delete;
        TextureWallsEditor( const TextureWallsEditor& ) = delete;
        TextureWallsEditor( TextureWallsEditor&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TextureWallsEditor() = default;


        /***
         * 3. Updating (observer pattern)
         ***/
        virtual void update();


        /***
         * 4. Operators
         ***/
        TextureWallsEditor& operator = ( const TextureWallsEditor& ) = delete;
        TextureWallsEditor& operator = ( TextureWallsEditor&& ) = delete;


    private:
        TextureWallsManager* textureWallsManager_;
};

} // namespace como

#endif // TEXTURE_WALLS_EDITOR_HPP
