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
#include <client/gui/utilities/color_button.hpp>


/***
 * 1. Construction
 ***/

namespace como {

MaterialPanel::MaterialPanel( LocalDrawablesSelectionPtr userSelection ) :
    userSelection_( userSelection )
{
    QFormLayout* layout = new QFormLayout;
    ColorButton* diffuseReflectivityButton = new ColorButton( QColor( 255, 0, 0, 255 ) );

    layout->addWidget( new QLabel( "Material properties" ) );
    layout->addRow( "Diffuse reflexivity: ", diffuseReflectivityButton );

    setLayout( layout );

    QObject::connect( diffuseReflectivityButton, &ColorButton::colorChanged, [=,this]( const PackableColor& diffuseReflectivity )
    {
        userSelection->setMaterialDiffuseReflexivity( diffuseReflectivity );
    });
}


/***
 * 3. Refreshing
 ***/

void MaterialPanel::refresh()
{

}

} // namespace como
