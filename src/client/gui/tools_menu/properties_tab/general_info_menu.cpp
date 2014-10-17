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

#include "general_info_menu.hpp"

#include <QLabel>
#include <QFormLayout>

namespace como {

GeneralInfoMenu::GeneralInfoMenu( LocalEntitiesSelection* userSelection ) :
    userSelection_( userSelection )
{
    // Create a layout for this widget.
    QFormLayout* layout = new QFormLayout;

    // Create name labels.
    objectName_ = new QLabel( "Unnamed" );
    objectTypeName_ = new QLabel( "Unnamed" );

    // Create a label for displaying the centroid of the user's selection.
    centroidPosition_ = new QLabel( "Undefined" );

    // Add widgets to the layout and set the latter as the current one.
    layout->addRow( new QLabel( "Name:" ), objectName_ );
    layout->addRow( new QLabel( "Type:" ), objectTypeName_ );
    layout->addRow( "Centroid position:", centroidPosition_ );
    setLayout( layout );

    userSelection_->addObserver( this );
    update();
}


/***
 * 4. Refreshing
 ***/

void GeneralInfoMenu::update()
{
    // Convert the requested centroid into a std::string.
    char centroidStr[50] = {0};
    glm::vec3 centroid = userSelection_->centroid();
    sprintf( centroidStr, "(%.3f, %.3f, %.3f)", centroid.x, centroid.y, centroid.z );

    // Write the previous "centroid std::string" to its corresponding label.
    centroidPosition_->setText( centroidStr );

    objectName_->setText( userSelection_->name().c_str() );
    objectTypeName_->setText( userSelection_->typeName().c_str() );
}

} // namespace como
