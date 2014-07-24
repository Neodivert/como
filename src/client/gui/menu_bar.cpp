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

MenuBar::MenuBar( DrawablesManagerPtr drawablesManager ) :
    QMenuBar( nullptr )
{
    addMenu( createViewMenu( drawablesManager.get() ) );

    setVisible( true );
}


/***
 * 2. Initialization
 ***/

QMenu* MenuBar::createViewMenu( DrawablesManager* drawablesManager )
{
    QMenu* viewMenu = new QMenu( "View" );

    viewMenu->addMenu( createDisplayEdgesMenu( drawablesManager ) );
    viewMenu->addMenu( createDisplayVertexNormalsMenu( drawablesManager ) );

    return viewMenu;
}


QMenu* MenuBar::createDisplayEdgesMenu( DrawablesManager* drawablesManager )
{
    QMenu* displayEdgesMenu = nullptr;
    QActionGroup* displayEdgesActionGroup = nullptr;
    QAction* displayEdgesAlways = nullptr;
    QAction* displayEdgesOnlyWhenSelected = nullptr;

    // Create a menu action for displaying the edges always.
    displayEdgesAlways = new QAction( QString( "Always" ), nullptr );
    displayEdgesAlways->setCheckable( true );
    QObject::connect( displayEdgesAlways, &QAction::triggered, [=](){
        drawablesManager->displayEdges( MeshEdgesDisplayFrequency::ALWAYS );
    });

    // Create a menu action for displaying the edges only when selected.
    displayEdgesOnlyWhenSelected = new QAction( QString( "Only when selected" ), nullptr );
    displayEdgesOnlyWhenSelected->setCheckable( true );
    QObject::connect( displayEdgesOnlyWhenSelected, &QAction::triggered, [=](){
        drawablesManager->displayEdges( MeshEdgesDisplayFrequency::ONLY_WHEN_SELECTED );
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


QMenu* MenuBar::createDisplayVertexNormalsMenu(DrawablesManager* drawablesManager)
{
    QMenu* displayVertexNormalsMenu = nullptr;
    QActionGroup* displayVertexNormalsActionGroup = nullptr;
    QAction* displayVertexNormalsAlways = nullptr;
    QAction* displayVertexNormalsNever = nullptr;

    // Create a menu action for displaying the edges always.
    displayVertexNormalsAlways = new QAction( QString( "Always" ), nullptr );
    displayVertexNormalsAlways->setCheckable( true );
    QObject::connect( displayVertexNormalsAlways, &QAction::triggered, [=](){
        drawablesManager->displayVertexNormals( true );
    });

    // Create a menu action for displaying the edges only when selected.
    displayVertexNormalsNever = new QAction( QString( "Never" ), nullptr );
    displayVertexNormalsNever->setCheckable( true );
    QObject::connect( displayVertexNormalsNever, &QAction::triggered, [=](){
        drawablesManager->displayVertexNormals( false );
    });

    // Include previous actions in a exclusive group of actions and set the
    // default action.
    displayVertexNormalsActionGroup = new QActionGroup( this );
    displayVertexNormalsActionGroup->addAction( displayVertexNormalsAlways );
    displayVertexNormalsActionGroup->addAction( displayVertexNormalsNever );
    displayVertexNormalsNever->setChecked( true );

    // Create a "Display edges" menu including previous actions.
    displayVertexNormalsMenu = new QMenu( "Display vertex normals" );
    displayVertexNormalsMenu->addActions( displayVertexNormalsActionGroup->actions() );
    return displayVertexNormalsMenu;
}

} // namespace como
