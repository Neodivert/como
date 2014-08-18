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

#include "menu_bar.hpp"

namespace como {

/***
 * 1. Construction
 ***/

MenuBar::MenuBar( EntitiesManagerPtr entitiesManager ) :
    QMenuBar( nullptr ),
    entitiesManager_( entitiesManager )
{
    addMenu( createViewMenu( entitiesManager.get() ) );

    setVisible( true );
}


/***
 * 2. Initialization
 ***/

QMenu* MenuBar::createViewMenu( EntitiesManager* entitiesManager )
{
    QMenu* viewMenu = new QMenu( "View" );

    viewMenu->addMenu( createDisplayEdgesMenu( entitiesManager ) );
    viewMenu->addMenu( createDisplayVertexNormalsMenu( entitiesManager->getMeshesManager().get() ) );

    return viewMenu;
}


QMenu* MenuBar::createDisplayEdgesMenu( EntitiesManager* entitiesManager )
{
    (void)( entitiesManager ); // TODO: Remove this and make use of entitiesManager.
    QMenu* displayEdgesMenu = nullptr;
    QActionGroup* displayEdgesActionGroup = nullptr;
    QAction* displayEdgesAlways = nullptr;
    QAction* displayEdgesOnlyWhenSelected = nullptr;

    // Create a menu action for displaying the edges always.
    displayEdgesAlways = new QAction( QString( "Always" ), nullptr );
    displayEdgesAlways->setCheckable( true );
    QObject::connect( displayEdgesAlways, &QAction::triggered, [=](){
        //entitiesManager->displayEdges( MeshEdgesDisplayFrequency::ALWAYS );
    });

    // Create a menu action for displaying the edges only when selected.
    displayEdgesOnlyWhenSelected = new QAction( QString( "Only when selected" ), nullptr );
    displayEdgesOnlyWhenSelected->setCheckable( true );
    QObject::connect( displayEdgesOnlyWhenSelected, &QAction::triggered, [=](){
        //entitiesManager->displayEdges( MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED );
    });

    // Include previous actions in a exclusive group of actions and set the
    // default action.
    displayEdgesActionGroup = new QActionGroup( this );
    displayEdgesActionGroup->addAction( displayEdgesAlways );
    displayEdgesActionGroup->addAction( displayEdgesOnlyWhenSelected );
    displayEdgesOnlyWhenSelected->setChecked( true );

    // Create a "Display edges" menu including previous actions.
    displayEdgesMenu = new QMenu( "Display edges" );
    displayEdgesMenu->addActions( displayEdgesActionGroup->actions() );
    return displayEdgesMenu;
}


QMenu* MenuBar::createDisplayVertexNormalsMenu( MeshesManager* meshesManager )
{
    QMenu* displayVertexNormalsMenu = nullptr;
    QActionGroup* displayVertexNormalsActionGroup = nullptr;

    // Update this menu whenever the local user's meshes selection changes.
    meshesManager->getLocalResourcesSelection()->Observable::addObserver( this );

    // Create a menu action for displaying the edges always.
    displayVertexNormalsAlways_ = new QAction( QString( "Always" ), nullptr );
    displayVertexNormalsAlways_->setCheckable( true );
    QObject::connect( displayVertexNormalsAlways_, &QAction::triggered, [=](){
        meshesManager->displayVertexNormals( true );
    });

    // Create a menu action for displaying the edges only when selected.
    displayVertexNormalsNever_ = new QAction( QString( "Never" ), nullptr );
    displayVertexNormalsNever_->setCheckable( true );
    QObject::connect( displayVertexNormalsNever_, &QAction::triggered, [=](){
        meshesManager->displayVertexNormals( false );
    });

    // Include previous actions in a exclusive group of actions and set the
    // default action.
    displayVertexNormalsActionGroup = new QActionGroup( this );
    displayVertexNormalsActionGroup->addAction( displayVertexNormalsAlways_ );
    displayVertexNormalsActionGroup->addAction( displayVertexNormalsNever_ );
    displayVertexNormalsNever_->setChecked( true );

    // Create a "Display edges" menu including previous actions.
    displayVertexNormalsMenu = new QMenu( "Display vertex normals" );
    displayVertexNormalsMenu->addActions( displayVertexNormalsActionGroup->actions() );
    return displayVertexNormalsMenu;
}


/***
 * 4. Updating
 ***/

void MenuBar::update()
{
    switch( entitiesManager_->getMeshesManager()->displaysVertexNormals() ){
        case ElementsMeetingCondition::ALL:
            displayVertexNormalsAlways_->setChecked( true );
        break;
        case ElementsMeetingCondition::NONE:
            displayVertexNormalsNever_->setChecked( true );
        break;
        case ElementsMeetingCondition::SOME:
            displayVertexNormalsAlways_->setChecked( false );
            displayVertexNormalsNever_->setChecked( false );
        break;
    }
}

} // namespace como
