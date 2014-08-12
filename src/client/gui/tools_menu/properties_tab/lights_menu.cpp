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

#include "lights_menu.hpp"
#include <QFormLayout>

namespace como {

LightsMenu::LightsMenu( LocalLightsSelection* lights ) :
    QFrame(),
    lights_( lights )
{
    // Create the widgets for modifying light properties.
    QFormLayout* layout = new QFormLayout();
    lightColorButton_ = new ColorButton( QColor( 255, 0, 0, 255 ) );
    lightAmbientCoefficientSpinBox_ = new QDoubleSpinBox;
    lightAmbientCoefficientSpinBox_->setRange( 0.0f, 1.0f );
    lightAmbientCoefficientSpinBox_->setSingleStep( 0.01f );
    lightAmbientCoefficientSpinBox_->setValue( 0.05f );

    // Set this panel's layout.
    layout->addWidget( new QLabel( "MeshLight panel" ) );
    layout->addRow( "MeshLight color", lightColorButton_ );
    layout->addRow( "Ambient coefficient", lightAmbientCoefficientSpinBox_ );
    setLayout( layout );

    // Connect the signals emitted when user changes a material parameter to
    // the corresponding methods which change those parameters.
    QObject::connect( lightColorButton_, &ColorButton::colorChanged, [=]( PackableColor color )
    {
        lights_->setLightColor( color );
    });

    void (QDoubleSpinBox::*ambientCoefficientChangeSignal)( double ) = &QDoubleSpinBox::valueChanged;
    QObject::connect( lightAmbientCoefficientSpinBox_, ambientCoefficientChangeSignal, [this]( double value ){
        lights_->setAmbientCoefficient( static_cast< float >( value ) );
    } );

    // Initially there is no light selected, so disable this panel.
    setEnabled( false );

    lights_->Observable::addObserver( this );
}


/***
 * 4. Update(ContainerAction lastContainerAction, ResourceID lastElementModified)
 ***/

void LightsMenu::update()
{
    // TODO: Complete.
    if( lights_->size() ){
        setEnabled( true );

        lightColorButton_->setColor( lights_->getLightColor() );

        // We don't want to emmit signals from lightAmbientCoefficientSpinBox_ when
        // we call setValue() directly, so we block them temporarily.
        // TODO: is there a more elegant way?
        bool oldBlockingState = lightAmbientCoefficientSpinBox_->blockSignals( true );
        lightAmbientCoefficientSpinBox_->setValue( lights_->getAmbientCoefficient() );
        lightAmbientCoefficientSpinBox_->blockSignals( oldBlockingState );

    }else{
        setEnabled( false );
    }
}

} // namespace como
