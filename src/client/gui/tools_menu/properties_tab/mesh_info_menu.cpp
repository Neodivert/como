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

MeshInfoMenu::MeshInfoMenu( LocalDrawablesSelectionPtr userSelection ) :
    userSelection_( userSelection )
{
    // Create the layout for this tab.
    QFormLayout* layout = new QFormLayout;

    // Create a label for displaying the centroid of the user's selection.
    centroidPosition_ = new QLabel( "Undefined" );

    // Add widgets to the layout and set it as the current one.
    layout->addRow( "Centroid position:", centroidPosition_ );
    setLayout( layout );
}


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
