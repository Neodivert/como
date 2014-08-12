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

#include "light_panel.hpp"
#include <QFormLayout>

namespace como {

LightPanel::LightPanel( LightsManagerPtr lightsManager ) :
    QFrame(),
    lightsManager_( lightsManager )
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
        currentLight_->setLightColor( color );
    });

    void (QDoubleSpinBox::*ambientCoefficientChangeSignal)( double ) = &QDoubleSpinBox::valueChanged;
    QObject::connect( lightAmbientCoefficientSpinBox_, ambientCoefficientChangeSignal, [this]( double value ){
        currentLight_->setAmbientCoefficient( static_cast< float >( value ) );
    } );

    // Initially there is no light selected, so disable this panel.
    setEnabled( false );

    lightsManager_->Observable::addObserver( this );
    lightsManager_->getLocalResourcesSelection()->addObserver( this );
}


/***
 * 4. Update(ContainerAction lastContainerAction, ResourceID lastElementModified)
 ***/

void LightPanel::update( ContainerAction lastContainerAction, ResourceID lastElementModified )
{
    if( ( lastContainerAction == ContainerAction::ELEMENT_DELETION ) &&
        currentLight_ &&
        ( lastElementModified == currentLight_->getResourceID() ) ){
        closeLight();
    }else{
        if( ( lightsManager_->getCurrentLight() != nullptr ) &&
                ( ( !currentLight_ ) ||
                  ( currentLight_->getResourceID() != lightsManager_->getCurrentLight()->getResourceID() ) ) ){
            openLight( lightsManager_->getCurrentLight() );
        }
    }
}


void LightPanel::update()
{
    if( currentLight_ != nullptr ){
        lightColorButton_->setColor( currentLight_->getLightColor() );

        // We don't want to emmit signals from lightAmbientCoefficientSpinBox_ when
        // we call setValue() directly, so we block them temporarily.
        // TODO: is there a more elegant way?
        bool oldBlockingState = lightAmbientCoefficientSpinBox_->blockSignals( true );
        lightAmbientCoefficientSpinBox_->setValue( currentLight_->getAmbientCoefficient() );
        lightAmbientCoefficientSpinBox_->blockSignals( oldBlockingState );
    }
}


/***
 * 5. 5. Light opening / closing
 ***/

void LightPanel::openLight( LightHandlerPtr light )
{
    currentLight_ = light;

    setEnabled( true );
}


void LightPanel::closeLight()
{
    currentLight_ = nullptr;

    setEnabled( false );
}

} // namespace como
