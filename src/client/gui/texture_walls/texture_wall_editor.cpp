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

#include <QDoubleSpinBox>
#include <QPushButton>
#include <QFormLayout>
#include <QFileDialog>
#include <QMessageBox>
#include "texture_wall_editor.hpp"

namespace como {


/***
 * 1. Construction
 ***/

TextureWallEditor::TextureWallEditor( TexturesManager* texturesManager ) :
    texturesManager_( texturesManager ),
    texturesViewer_( new TexturesViewer( texturesManager, this ) )
{
    QFormLayout* layout = new QFormLayout;
    QDoubleSpinBox* textureOffsetXSpinBox = new QDoubleSpinBox;
    QDoubleSpinBox* textureOffsetYSpinBox = new QDoubleSpinBox;
    QPushButton* textureInput = new QPushButton;

    // Set the parameters for the widget used for modifying
    // texture offset (X)
    textureOffsetXSpinBox->setDecimals( 2 );
    textureOffsetXSpinBox->setSingleStep( 1.0 );
    textureOffsetXSpinBox->setRange( 0.0, 10.0 );

    // Set the parameters for the widget used for modifying
    // texture offset (Y)
    textureOffsetYSpinBox->setDecimals( 2 );
    textureOffsetYSpinBox->setSingleStep( 1.0 );
    textureOffsetYSpinBox->setRange( 0.0, 10.0 );

    // When user click on file path input button, open a dialog for selecting
    // a file.
    QObject::connect( textureInput, &QPushButton::clicked, [=](){
        // Before executing the textures viewer, make a signal / slot
        // connection so whenever a texture is selected, display a
        // message to user.
        QObject::connect( texturesViewer_,
                          &TexturesViewer::textureSelected,
                          [this]( ResourceID textureID ){

            std::string infoMessage =
                    "Texture selected (" +
                    to_string( textureID.getCreatorID() ) +
                    ", " +
                    to_string( textureID.getResourceIndex() ) +
                    ")";

            QMessageBox::information( this,
                                      "Texture selected",
                                      infoMessage.c_str()
                                      );
        });

        texturesViewer_->exec();
    });

    // Set this widget's layout.
    layout->addRow( "Texture offset (X)", textureOffsetXSpinBox );
    layout->addRow( "Texture offset (Y)", textureOffsetYSpinBox );
    layout->addRow( "Texture: ", textureInput );
    setLayout( layout );
}


/***
 * 3. Updating (observer pattern)
 ***/

void TextureWallEditor::update()
{
    // TODO: Complete

    /*
    if( textureWallHandler_ != nullptr ){
        // TODO: Complete
    }
    */
}


} // namespace como
