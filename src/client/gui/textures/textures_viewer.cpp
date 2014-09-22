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

#include <QVBoxLayout>
#include <QFileDialog>
#include <QMessageBox>
#include <QPushButton>
#include "textures_viewer.hpp"
#include "textures_gallery.hpp"

namespace como {


/***
 * 1. Construction
 ***/

TexturesViewer::TexturesViewer( TexturesManager *texturesManager, QWidget* parent ) :
    QDialog( parent )
{
    QPushButton* loadTextureButton = new QPushButton( "Load new texture" );
    QVBoxLayout* layout = new QVBoxLayout;
    TexturesGallery* texturesGallery = new TexturesGallery( texturesManager );

    setWindowTitle( "Textures viewer" );

    // When fileInput button is clicked, open a QFileDialog.
    QObject::connect( loadTextureButton, &QPushButton::clicked, [=](){
        try {
            std::string textureFilePath =
                    QFileDialog::getOpenFileName( this,
                                                  tr("Open file"),
                                                  "",
                                                  tr("All files (*)" /* TODO: Filter only supported image formats */ ) ).toStdString();

            if( textureFilePath.size() > 0 ){
                texturesManager->loadTexture( textureFilePath );
            }
        }catch( std::exception& ex ){
            QMessageBox::warning( this,
                                  "Couldn't load texture",
                                  ex.what() );
        }
    });

    // Whenever a "textureSelected" signal is received from TexturesGallery,
    // simply forward it to the outside.
    QObject::connect( texturesGallery,
                      &TexturesGallery::textureSelected,
                      [this]( ResourceID textureID ){
        emit textureSelected( textureID );
    });

    // Set this widget's layout.
    layout->addWidget( texturesGallery );
    layout->addWidget( loadTextureButton );
    setLayout( layout );
}


} // namespace como
