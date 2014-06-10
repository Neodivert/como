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

    // Set this panel's layout.
    layout->addWidget( new QLabel( "MeshLight panel" ) );
    layout->addRow( "MeshLight color", lightColorButton_ );
    setLayout( layout );

    // Connect the signals emitted when user changes a material parameter to
    // the corresponding methods which change those parameters.
    QObject::connect( lightColorButton_, &ColorButton::colorChanged, [=]( PackableColor color )
    {
        currentLight_->setLightColor( color );
    });


    QObject::connect( lightsManager_.get(), &LightsManager::lightRemoved, [this]( PackableDrawableID lightID ){
        if( currentLight_ && ( lightID == currentLight_->getLightID() ) ){
            closeLight();
        }
    });

    QObject::connect( lightsManager_.get(), &LightsManager::lightModified, [this]( PackableDrawableID lightID ){
        if( currentLight_ && ( lightID == currentLight_->getLightID() ) ){
            refresh();
        }
    });

    // Initially there is no light selected, so disable this panel.
    setEnabled( false );
}

/***
 * 4. Refreshing
 ***/

void LightPanel::refresh()
{
    lightColorButton_->setColor( currentLight_->getLightColor() );
}


/***
 * 5. Slots
 ***/

void LightPanel::openLight( LightHandlerPtr light )
{
    currentLight_ = light;

    setEnabled( true );

    refresh();
}


void LightPanel::closeLight()
{
    currentLight_ = nullptr;

    setEnabled( false );
}

} // namespace como
