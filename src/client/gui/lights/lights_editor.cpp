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
#include "lights_list.hpp"
#include "light_panel.hpp"

namespace como {

LightsEditor::LightsEditor( LightsManagerPtr lightsManager )
{
    QVBoxLayout* layout = new QVBoxLayout;
    LightsList* lightsList = new LightsList( lightsManager );
    QPushButton* directionalLightCreationButton = new QPushButton( "Create directional light" );
    LightPanel* lightPanel = new LightPanel;

    layout->addWidget( new QLabel( "Lights editor" ) );
    layout->addWidget( lightsList );
    layout->addWidget( directionalLightCreationButton );
    layout->addWidget( lightPanel );

    setLayout( layout );

    QObject::connect( lightsManager.get(), &LightsManager::lightSelected,
                      lightPanel, &LightPanel::openLight );

    QObject::connect( directionalLightCreationButton, &QPushButton::pressed,
                      lightsManager.get(), &LightsManager::createDirectionalLight );

}

} // namepsace como
