/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "mesh_info_menu.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MeshInfoMenu::MeshInfoMenu( DrawablesSelectionPtr userSelection ) :
    userSelection_( userSelection )
{
    // Create the layout for this tab.
    QFormLayout* layout = new QFormLayout;

    // Create a label for displaying the centroid of the user's selection.
    centroidPosition_ = new QLabel( "Undefined" );

    // Create a button for selecting the color of the meshes selected by user.
    colorInput_ = new QPushButton("");
    colorInput_->setStyleSheet( "background-color: red" );

    // Signal / Slot connection. When previous button is clicked, we invoke the
    // changeCurrentColor method.
    QObject::connect( colorInput_, &QPushButton::clicked, [=,this](){
        // Open a dialog for selecting a new color.
        QColor meshColor = QColorDialog::getColor( meshColor );

        // If the selected color is not valid, change it to red.
        //if( !( meshColor.isValid() ) ){
        //    meshColor.setRgb( 255, 0, 0 );
        //}

        QString qss = QString("background-color: %1").arg(meshColor.name());
        colorInput_->setStyleSheet( qss );

        // Emit a signal indicating that the current color has changed.
        //emit meshColorChanged( meshColor_ );
    });

    // Add widgets to the layout and set it as the current one.
    layout->addRow( "Centroid position:", centroidPosition_ );
    layout->addRow( "Mesh color:", colorInput_ );
    setLayout( layout );
}

/*
QColor col = QColorDialog::getColor(Qt::white, this);
if(col.isValid()) {
    QString qss = QString("background-color: %1").arg(col.name());
    ui->pushButton->setStyleSheet(qss);
}
*/

/***
 * 3. Refreshing
 ***/

void MeshInfoMenu::refresh()
{
    char centroidStr[50] = {0};
    glm::vec4 centroid = userSelection_->getCentroid();

    // Convert the requested centroid into a string.
    sprintf( centroidStr, "(%.3f, %.3f, %.3f)", centroid.x, centroid.y, centroid.z );

    // Write the previous "centroid string" to its corresponding label.
    centroidPosition_->setText( centroidStr );
}

} // namespace como
