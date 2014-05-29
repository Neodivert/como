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
    QFrame()
{
    // Create the widgets for modifying material properties.
    QFormLayout* layout = new QFormLayout();
    nameInput_ = new QLineEdit();
    ColorButton* colorButton = new ColorButton( QColor( 255, 0, 0, 255 ) );
    ColorButton* diffuseReflectivityButton = new ColorButton( QColor( 255, 0, 0, 255 ) );
    ColorButton* specularReflectivityButton = new ColorButton( QColor( 255, 0, 0, 255 ) );
    QDoubleSpinBox* specularExponentSpinBox = new QDoubleSpinBox();

    // Set the parameters for the widget used for modifying material
    // specular exponent.
    specularExponentSpinBox->setDecimals( 2 );
    specularExponentSpinBox->setSingleStep( 1.0 );
    specularExponentSpinBox->setRange( 0.0, 100.0 );

    // Set this panel's layout.
    layout->addWidget( new QLabel( "Material panel" ) );
    layout->addRow( "Material name", nameInput_ );
    layout->addRow( "Color: ", colorButton );
    layout->addRow( "Diffuse reflectivity: ", diffuseReflectivityButton );
    layout->addRow( "Specular reflectivity: ", specularReflectivityButton );
    layout->addRow( "Specular exponent: ", specularExponentSpinBox );
    setLayout( layout );

    // Connect the signals emitted when user changes a material parameter to
    // the corresponding methods which change those parameters.
    QObject::connect( colorButton, &ColorButton::colorChanged, [=]( const PackableColor& color )
    {
        currentMaterial_->setColor( color );
    });

    QObject::connect( diffuseReflectivityButton, &ColorButton::colorChanged, [=]( const PackableColor& diffuseReflectivity )
    {
        currentMaterial_->setDiffuseReflectivity( diffuseReflectivity );
    });

    QObject::connect( diffuseReflectivityButton, &ColorButton::colorChanged, [=]( const PackableColor& diffuseReflectivity )
    {
        currentMaterial_->setDiffuseReflectivity( diffuseReflectivity );
    });

    QObject::connect( specularReflectivityButton, &ColorButton::colorChanged, [=]( const PackableColor& specularReflectivity )
    {
        currentMaterial_->setSpecularReflectivity( specularReflectivity );
    });

    void (QDoubleSpinBox::*spinBoxValueChanged)(double) = &QDoubleSpinBox::valueChanged;
    QObject::connect( specularExponentSpinBox, spinBoxValueChanged, [this]( double specularExponent ){
        currentMaterial_->setSpecularExponent( static_cast< float >( specularExponent ) );
    });

    setEnabled( false );
}


/***
 * 4. Slots
 ***/

void MaterialPanel::openMaterial( MaterialHandlerPtr material )
{
    currentMaterial_ = material;

    setEnabled( true );

    nameInput_->setText( currentMaterial_->getName().c_str() );
}

} // namespace como
