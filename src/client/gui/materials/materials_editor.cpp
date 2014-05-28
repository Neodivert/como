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

#include "materials_editor.hpp"

namespace como {

MaterialsEditor::MaterialsEditor( MaterialsManagerPtr materialsManager ) :
    QFrame(),
    materialsManager_( materialsManager )
{
    QVBoxLayout* layout = new QVBoxLayout();
    MaterialsList* materialsList = new MaterialsList( materialsManager );
    MaterialPanel* materialPanel = new MaterialPanel();

    layout->addWidget( new QLabel( "Materials Editor" ) );
    layout->addWidget( materialsList );
    layout->addWidget( materialPanel );

    setLayout( layout );
}

} // namespace como
