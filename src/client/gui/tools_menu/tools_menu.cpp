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

#include "tools_menu.hpp"
#include "creation_tab.hpp"
#include "transformation_tab.hpp"
#include "properties_tab/properties_tab.hpp"
#include <QVBoxLayout>

namespace como {


/***
 * 1. Initialization and destruction
 ***/

ToolsMenu::ToolsMenu( QWidget* parent, shared_ptr< ComoApp > comoApp ) :
    QTabWidget( parent ),
    comoApp_( comoApp ),
    currentColor_( 255, 0, 0, 255 ),
    currentDirectionalLightColor_( 255, 0, 0, 255 )
{
    addTab( new CreationTab( comoApp_->getScene() ), "Creation" );
    addTab( new TransformationTab( comoApp_ ), "Transformation" );

    connect( comoApp->getScene().get(), &Scene::connectedToScene, [this](){
        // TODO: Remove tabs when disconnected from scene.
        addTab( new PropertiesTab( comoApp_->getScene()->getDrawablesManager()->getLocalUserSelection() ), "Properties" );
        addTab( new MaterialsEditor( comoApp_->getScene()->getMaterialsManager() ), "Materials" );
        addTab( new LightsEditor( comoApp_->getScene()->getLightsManager() ), "Lights" );
    });
}


/***
 * 2. Getters
 ***/

QColor ToolsMenu::getCurrentColor() const
{
    return currentColor_;
}




} // namespace como
