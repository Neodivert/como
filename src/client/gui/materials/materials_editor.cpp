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
    QHBoxLayout* layout = new QHBoxLayout();

    layout->addWidget( new QLabel( "Materials Editor" ) );
    layout->addWidget( new MaterialsList( materialsManager ) );

    setLayout( layout );
}

} // namespace como
