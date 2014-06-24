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

#include "material_panel.hpp"
#include <QFormLayout>

namespace como {

MaterialPanel::MaterialPanel() :
    QFrame()
{
    // Create the widgets for modifying material properties.
    QFormLayout* layout = new QFormLayout();
    nameInput_ = new QLineEdit();
    colorButton_ = new ColorButton( QColor( 255, 0, 0, 255 ) );
    ambientReflectivityButton_ = new ColorButton( QColor( 255, 0, 0, 255 ) );
    diffuseReflectivityButton_ = new ColorButton( QColor( 255, 0, 0, 255 ) );
    specularReflectivityButton_ = new ColorButton( QColor( 255, 0, 0, 255 ) );
    specularExponentSpinBox_ = new QDoubleSpinBox();

    // Set the parameters for the widget used for modifying material
    // specular exponent.
    specularExponentSpinBox_->setDecimals( 2 );
    specularExponentSpinBox_->setSingleStep( 1.0 );
    specularExponentSpinBox_->setRange( 0.0, 1000.0 );

    // Set this panel's layout.
    layout->addWidget( new QLabel( "Material panel" ) );
    layout->addRow( "Material name", nameInput_ );
    layout->addRow( "Color: ", colorButton_ );
    layout->addRow( "Ambient reflectivity: ", ambientReflectivityButton_ );
    layout->addRow( "Diffuse reflectivity: ", diffuseReflectivityButton_ );
    layout->addRow( "Specular reflectivity: ", specularReflectivityButton_ );
    layout->addRow( "Specular exponent: ", specularExponentSpinBox_ );
    setLayout( layout );

    // Connect the signals emitted when user changes a material parameter to
    // the corresponding methods which change those parameters.
    QObject::connect( colorButton_, &ColorButton::colorChanged, [=]( PackableColor color )
    {
        currentMaterial_->setColor( color );
    });

    QObject::connect( ambientReflectivityButton_, &ColorButton::colorChanged, [=]( PackableColor ambientReflectivity )
    {
        currentMaterial_->setAmbientReflectivity( ambientReflectivity );
    });

    QObject::connect( diffuseReflectivityButton_, &ColorButton::colorChanged, [=]( PackableColor diffuseReflectivity )
    {
        currentMaterial_->setDiffuseReflectivity( diffuseReflectivity );
    });

    QObject::connect( diffuseReflectivityButton_, &ColorButton::colorChanged, [=]( PackableColor diffuseReflectivity )
    {
        currentMaterial_->setDiffuseReflectivity( diffuseReflectivity );
    });

    QObject::connect( specularReflectivityButton_, &ColorButton::colorChanged, [=]( PackableColor specularReflectivity )
    {
        currentMaterial_->setSpecularReflectivity( specularReflectivity );
    });

    void (QDoubleSpinBox::*spinBoxValueChanged)(double) = &QDoubleSpinBox::valueChanged;
    QObject::connect( specularExponentSpinBox_, spinBoxValueChanged, [this]( double specularExponent ){
        currentMaterial_->setSpecularExponent( static_cast< float >( specularExponent ) );
    });

    setEnabled( false );
}


/***
 * 3. Getters
 ***/

MaterialID MaterialPanel::getMaterialID() const
{
    return currentMaterial_->getID();
}


/***
 * 4. Refreshing
 ***/

void MaterialPanel::refresh()
{
    nameInput_->setText( currentMaterial_->getName().c_str() );

    colorButton_->setColor( currentMaterial_->getColor() );
    ambientReflectivityButton_->setColor( currentMaterial_->getAmbientReflectivity() );
    diffuseReflectivityButton_->setColor( currentMaterial_->getDiffuseReflectivity() );
    specularReflectivityButton_->setColor( currentMaterial_->getSpecularReflectivity() );
    specularExponentSpinBox_->setValue( currentMaterial_->getSpecularExponent() );
}


/***
 * 5. Slots
 ***/

void MaterialPanel::openMaterial( MaterialHandlerPtr material )
{
    currentMaterial_ = material;

    setEnabled( true );

    refresh();
}

} // namespace como
