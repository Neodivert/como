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

#ifndef TEXTURES_GALLERY_HPP
#define TEXTURES_GALLERY_HPP

#include <QListWidget>
#include <client/managers/managers/textures/textures_manager.hpp>
#include <common/utilities/observer_pattern/observer.hpp>

namespace como {

class TexturesGallery : public QListWidget, public Observer
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        TexturesGallery( TexturesManager* texturesManager );
        TexturesGallery() = delete;
        TexturesGallery( const TexturesGallery& ) = delete;
        TexturesGallery( TexturesGallery&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~TexturesGallery() = default;


        /***
         * 3. Updating (observer pattern)
         ***/
        virtual void update();


        /***
         * 4. Operators
         ***/
        TexturesGallery& operator = ( const TexturesGallery& ) = delete;
        TexturesGallery& operator = ( TexturesGallery&& ) = delete;


    private:
        TexturesManager* texturesManager_;

        std::list<TextureData> texturesDataList_;
        std::list< std::unique_ptr< QImage > > textureImages_;
};

} // namespace como

#endif // TEXTURES_GALLERY_HPP
