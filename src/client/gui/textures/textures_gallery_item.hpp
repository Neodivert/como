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

#ifndef TEXTURES_GALLERY_ITEM_HPP
#define TEXTURES_GALLERY_ITEM_HPP

#include <QListWidgetItem>
#include <common/ids/resource_id.hpp>
#include <string>
#include <client/models/3d/textures/texture.hpp>

namespace como {

class TexturesGalleryItem : public QObject, public QListWidgetItem
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        TexturesGalleryItem( const ResourceID& textureID,
                             const std::string& textureName,
                             TextureData textureData );
        TexturesGalleryItem() = delete;
        TexturesGalleryItem( const TexturesGalleryItem& ) = delete;
        TexturesGalleryItem( TexturesGalleryItem&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~TexturesGalleryItem() = default;


        /***
         * 3. Getters
         ***/
        ResourceID textureID() const;


        /***
         * 4. Operators
         ***/
        TexturesGalleryItem& operator = ( const TexturesGalleryItem& ) = delete;
        TexturesGalleryItem& operator = ( TexturesGalleryItem&& ) = delete;


    private:
        const ResourceID textureID_;
        const TextureData textureData_;
        std::unique_ptr< QImage > textureImage_;
};

} // namespace como

#endif // TEXTURES_GALLERY_ITEM_HPP
