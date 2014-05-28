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

#include "material_panel.hpp"
#include <QFormLayout>

namespace como {

MaterialPanel::MaterialPanel() :
    QFrame(),
    currentMaterial_( nullptr )
{
    QFormLayout* layout = new QFormLayout();

    nameInput_ = new QLineEdit();

    layout->addWidget( new QLabel( "Material panel" ) );
    layout->addRow( new QLabel( "Material name" ), nameInput_ );

    setLayout( layout );
}


/***
 * 4. Slots
 ***/

void MaterialPanel::openMaterial( MaterialPtr material )
{
    currentMaterial_ = material;

    nameInput_->setText( currentMaterial_->getName().c_str() );
}

} // namespace como
