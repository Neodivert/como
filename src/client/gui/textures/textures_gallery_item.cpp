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

#include "textures_gallery_item.hpp"

namespace como {

/***
 * 1. Construction
 ***/

TexturesGalleryItem::TexturesGalleryItem( TextureData textureData ) :
    QListWidgetItem( textureData.name.c_str() ),
    textureData_( textureData )
{
    const QImage::Format imageFormat =
            ( textureData_.format == GL_RGBA ) ?
                QImage::Format_RGBA8888 :
                QImage::Format_RGB888;

    textureImage_ =
            std::unique_ptr< QImage >(
                new QImage( &( textureData_.pixels[0] ),
                            textureData_.width,
                            textureData_.height,
                            imageFormat ) );

    setIcon( QPixmap::fromImage( *textureImage_ ) );
}


/***
 * 3. Getters
 ***/

ResourceID TexturesGalleryItem::textureID() const
{
    return textureData_.id;
}


} // namespace como
