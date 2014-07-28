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

#include "viewport_tool_bar.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ViewportToolBar::ViewportToolBar( Viewport* viewport ) :
    QToolBar( nullptr )
{
    addAction( createMaximizeAction() );
    addWidget( createViewSelector( viewport ) );
    addAction( createPerspectiveAction( viewport ) );
}


/***
 * 2. Initialization
 ***/

QAction* ViewportToolBar::createMaximizeAction() const
{
    QAction* maximizeAction = nullptr;

    maximizeAction = new QAction( QString( "Maximize" ), nullptr );
    maximizeAction->setCheckable( true );
    QObject::connect( maximizeAction, &QAction::triggered, [this]( bool checked ){
        if( checked ){
            emit viewFrameMaximizationRequested();
        }else{
            emit viewFrameMinimizationRequested();
        }
    });

    return maximizeAction;
}


QComboBox* ViewportToolBar::createViewSelector( Viewport* viewport ) const
{
    QComboBox* viewSelector = new QComboBox;

    for( auto viewString : viewStrings ){
        viewSelector->addItem( tr( viewString ) );
    }

    // Select the given view.
    viewSelector->setCurrentIndex( static_cast< int >( viewport->getView() ) );

    // When user change view in selector, call Viewport::setView().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( viewSelector, signal, [=]( int index ) {
        viewport->setView( static_cast< View >( index ) );
    });

    return viewSelector;
}


QAction *ViewportToolBar::createPerspectiveAction( Viewport* viewport ) const
{
    QAction* perspectiveAction = new QAction( "Perspective", nullptr );
    perspectiveAction->setCheckable( true );

    if( viewport->getProjection() == Projection::PERSPECTIVE ){
        perspectiveAction->setChecked( true );
    }

    QObject::connect( perspectiveAction, &QAction::triggered, [=]( bool checked ){
        if( checked ){
            viewport->setProjection( Projection::PERSPECTIVE );
        }else{
            viewport->setProjection( Projection::ORTHO );
        }
    });

    return perspectiveAction;
}

} // namespace como
