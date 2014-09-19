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

#include "textures_gallery.hpp"

namespace como {

/***
 * 1. Construction
 ***/

TexturesGallery::TexturesGallery( TexturesManager* texturesManager ) :
    texturesManager_( texturesManager )
{
    texturesManager_->Observable::addObserver( this );

    // We are displaying images (big icons)
    // http://stackoverflow.com/a/14114878/3917854
    setViewMode( QListWidget::IconMode );
    setIconSize( QSize( 200,200 ) );
    setResizeMode( QListWidget::Adjust );
}


/***
 * 3. Updating (observer pattern)
 ***/

void TexturesGallery::update()
{
    texturesDataList_.clear();
    clear();

    texturesDataList_ = texturesManager_->getTexturesData();

    std::list<TextureData>::const_iterator textureDataIterator =
            texturesDataList_.begin();
    QImage::Format imageFormat;

    for( ; textureDataIterator != texturesDataList_.end(); textureDataIterator++ ){
        imageFormat = ( textureDataIterator->format == GL_RGBA ) ?
                    QImage::Format_RGBA8888 :
                    QImage::Format_RGB888;

        textureImages_.push_back(
                    std::unique_ptr< QImage >(
                        new QImage( textureDataIterator->pixels.get(),
                                    textureDataIterator->width,
                                    textureDataIterator->height,
                                    imageFormat ) ) );

        addItem( new QListWidgetItem( QIcon( QPixmap::fromImage( *( textureImages_.back() ) ) ), "Texture" ) );
    }
}

} // namespace como
