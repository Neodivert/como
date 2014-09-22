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

    // Whenever the user double-click over a texture, emit a "textureSelected"
    // signal.
    QObject::connect( this, &TexturesGallery::itemDoubleClicked,
                      [this]( QListWidgetItem* listItem ){
        TexturesGalleryItem* textureItem =
                dynamic_cast< TexturesGalleryItem* >( listItem );

        emit textureSelected( textureItem->textureID() );
    });
}


/***
 * 3. Updating (observer pattern)
 ***/

void TexturesGallery::update()
{
    const std::list< TextureData > texturesDataList =
            texturesManager_->getTexturesData();

    clear();

    for( const TextureData& textureData : texturesDataList ){
        // TODO: Retrieve real ID and name.
        addItem( new TexturesGalleryItem(
                     NO_RESOURCE,
                     "<texture-name>",
                     textureData ) );
    }
}

} // namespace como
