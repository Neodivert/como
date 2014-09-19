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
    texturesManager_( texturesManager )
{
    QFormLayout* layout = new QFormLayout;
    QDoubleSpinBox* textureOffsetXSpinBox = new QDoubleSpinBox;
    QDoubleSpinBox* textureOffsetYSpinBox = new QDoubleSpinBox;
    QPushButton* textureFilePathInput = new QPushButton;

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
    QObject::connect( textureFilePathInput, &QPushButton::clicked, [=](){
        try {
            std::string filePath = QFileDialog::getOpenFileName( this,
                                                                 tr("Open file"),
                                                                 "~",
                                                                 tr("All files (*)") ).toStdString();
            if( filePath.size() > 0 ){
                texturesManager_->loadTexture( filePath );
                textureFilePathInput->setText( filePath.c_str() );
            }
        }catch( std::exception& ex ){
            QMessageBox::warning( this,
                                  "Couldn't load texture",
                                  ex.what() );
        }
    });

    // Set accept and reject buttons behaviour.
    //QObject::connect( okButton_, &QPushButton::clicked, this, &PrimitiveImportDialog::validate );
    //QObject::connect( cancelButton_, &QPushButton::clicked, this, &PrimitiveImportDialog::reject );

    // Set this widget's layout.
    layout->addRow( "Texture offset (X)", textureOffsetXSpinBox );
    layout->addRow( "Texture offset (Y)", textureOffsetYSpinBox );
    layout->addRow( "Texture: ", textureFilePathInput );
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
