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

#include "lights_editor.hpp"
#include <QVBoxLayout>
#include "light_panel.hpp"

namespace como {

LightsEditor::LightsEditor( LightsManagerPtr lightsManager ) :
    lightsManager_( lightsManager )
{
    QVBoxLayout* layout = new QVBoxLayout;
    lightsList_ = new LightsList( lightsManager );
    QPushButton* directionalLightCreationButton = new QPushButton( "Create directional light" );
    LightPanel* lightPanel = new LightPanel( lightsManager );

    layout->addWidget( new QLabel( "Lights editor" ) );
    layout->addWidget( lightsList_ );
    layout->addWidget( directionalLightCreationButton );
    layout->addWidget( lightPanel );

    setLayout( layout );

    QObject::connect( directionalLightCreationButton, &QPushButton::pressed,
                      lightsManager.get(), &LightsManager::createDirectionalLight );
}


/***
 * 3. Events
 ***/

void LightsEditor::enterEvent( QEvent *event )
{
    (void)( event );

    LightsListItem* lightItem = dynamic_cast< LightsListItem* >( lightsList_->currentItem() );

    if( lightItem ){
        lightsManager_->highlightLight( lightItem->getResourceID() );
    }
}

void LightsEditor::leaveEvent(QEvent *event)
{
    (void)( event );

    lightsManager_->removeHighlights();
}


} // namespace como
