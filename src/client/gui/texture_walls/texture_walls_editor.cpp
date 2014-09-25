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

#include <QGridLayout>
#include <QLabel>
#include "texture_walls_editor.hpp"
#include <client/gui/utilities/resources_dropdown_list.hpp>
#include "texture_wall_editor.hpp"


namespace como {

/***
 * 1. Construction
 ***/

TextureWallsEditor::TextureWallsEditor( TextureWallsManager* textureWallsManager, TexturesManager* texturesManager ) :
    textureWallsManager_( textureWallsManager )
{
    QGridLayout* layout = nullptr;
    ResourcesDropdownList* textureWallsSelector =
            new ResourcesDropdownList( *textureWallsManager );
    TextureWallEditor* textureWallEditor =
            new TextureWallEditor( textureWallsManager, texturesManager );

    QObject::connect( textureWallsSelector,
                      &ResourcesDropdownList::resourceSelected,
                      [=]( ResourceID textureWallID ){
            textureWallEditor->setTextureWall( textureWallsManager->selectTextureWall( textureWallID ) );
    });

    // Set this widget's layout.
    layout = new QGridLayout();
    layout->addWidget( new QLabel( "Texture walls editor" ), 0, 0, 1, -1 );

    layout->addWidget( new QLabel( "Texture wall: " ), 1, 0 );
    layout->addWidget( textureWallsSelector, 1, 1 );

    layout->addWidget( textureWallEditor, 2, 0, 1, -1 );
    setLayout( layout );

    textureWallsManager_->Observable::addObserver( this );
    update();
}


/***
 * 3. Updating (observer pattern)
 ***/

void TextureWallsEditor::update()
{
    setVisible( textureWallsManager_->thereIsSelectableTextureWalls() );
}

} // namespace como
