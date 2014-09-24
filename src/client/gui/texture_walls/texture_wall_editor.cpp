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

#include <QPushButton>
#include <QFormLayout>
#include <QFileDialog>
#include "texture_wall_editor.hpp"

namespace como {


/***
 * 1. Construction
 ***/

TextureWallEditor::TextureWallEditor( TextureWallsManager* textureWallsManager, TexturesManager* texturesManager ) :
    currentTextureWall_( nullptr ),
    texturesManager_( texturesManager ),
    textureWallsManager_( textureWallsManager ),
    texturesViewer_( new TexturesViewer( texturesManager, this ) )
{
    QFormLayout* layout = new QFormLayout;
    textureOffsetXSpinBox_ = new QDoubleSpinBox;
    textureOffsetYSpinBox_ = new QDoubleSpinBox;
    QPushButton* textureInput = new QPushButton;

    // Set the parameters for the widget used for modifying
    // texture offset (X)
    textureOffsetXSpinBox_->setDecimals( 2 );
    textureOffsetXSpinBox_->setSingleStep( 1.0 );
    textureOffsetXSpinBox_->setRange( 0.0, 100.0 );

    // Set the parameters for the widget used for modifying
    // texture offset (Y)
    textureOffsetYSpinBox_->setDecimals( 2 );
    textureOffsetYSpinBox_->setSingleStep( 1.0 );
    textureOffsetYSpinBox_->setRange( 0.0, 100.0 );

    // When user click on file path input button, open a dialog for selecting
    // a file.
    QObject::connect( textureInput, &QPushButton::clicked, [=](){
        // Before executing the textures viewer, make a signal / slot
        // connection so whenever a texture is selected, display a
        // message to user.
        QObject::connect( texturesViewer_,
                          &TexturesViewer::textureSelected,
                          [this]( ResourceID textureID ){

            assert( currentTextureWall_ != nullptr );

            // TODO: Call to currentTextureWall_->addObserver( this ), but
            // being careful to not repeat observers:

            currentTextureWall_->setTextureID( textureID );
        });

        texturesViewer_->exec();
    });

    // Set this widget's layout.
    layout->addRow( "Texture offset % (X)", textureOffsetXSpinBox_ );
    layout->addRow( "Texture offset % (Y)", textureOffsetYSpinBox_ );
    layout->addRow( "Texture: ", textureInput );
    setLayout( layout );
}


/***
 * 3. Setters
 ***/

void TextureWallEditor::setTextureWall(TextureWallHandler *textureWall)
{
    currentTextureWall_ = textureWall;
}


/***
 * 4. Updating (observer pattern)
 ***/

void TextureWallEditor::update()
{
    glm::vec2 textureOffset;

    if( currentTextureWall_ != nullptr ){
        // Retrieve texture offset.
        textureOffset = currentTextureWall_->getTextureOffset();
        textureOffsetXSpinBox_->setValue( textureOffset.x );
        textureOffsetYSpinBox_->setValue( textureOffset.y );

        // TODO: Retrieve texture offset.
    }
}


} // namespace como
