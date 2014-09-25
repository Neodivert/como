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
#include <QDoubleSpinBox>
#include <client/managers/managers/textures/textures_manager.hpp>
#include <common/utilities/observer_pattern/observer.hpp>
#include <client/gui/textures/textures_viewer.hpp>
#include <client/managers/managers/textures/texture_walls_manager.hpp>

namespace como {

class TextureWallEditor : public QFrame, public Observer
{
    public:
        /***
         * 1. Construction
         ***/
        TextureWallEditor( TextureWallsManager* textureWallsManager, TexturesManager* texturesManager );
        TextureWallEditor() = delete;
        TextureWallEditor( const TextureWallEditor& ) = delete;
        TextureWallEditor( TextureWallEditor&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TextureWallEditor() = default;


        /***
         * 3. Setters
         ***/
        void setTextureWall( TextureWallHandler* textureWall );


        /***
         * 4. Updating (observer pattern)
         ***/
        virtual void update();


        /***
         * 5. Operators
         ***/
        TextureWallEditor& operator = ( const TextureWallEditor& ) = delete;
        TextureWallEditor& operator = ( TextureWallEditor&& ) = delete;


    private:
        TextureWallHandler* currentTextureWall_;
        TexturesManager* texturesManager_;
        TextureWallsManager* textureWallsManager_;
        TexturesViewer* texturesViewer_;

        QDoubleSpinBox* textureOffsetXSpinBox_;
        QDoubleSpinBox* textureOffsetYSpinBox_;
        QDoubleSpinBox* textureScaleXSpinBox_;
        QDoubleSpinBox* textureScaleYSpinBox_;
};

} // namespace como

#endif // TEXTURE_WALL_EDITOR_HPP
