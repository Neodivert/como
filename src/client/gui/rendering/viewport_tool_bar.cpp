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
#include "viewport_view_menu.hpp"
#include "viewport_projection_menu.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ViewportToolBar::ViewportToolBar( Viewport* viewport ) :
    QMenuBar( nullptr )
{
    addMenu( new ViewportViewMenu( viewport, this ) );
    addMenu( new ViewportProjectionMenu( viewport, this ) );
    addAction( createMaximizeAction() );
}


/***
 * 2. Initialization
 ***/

QAction* ViewportToolBar::createMaximizeAction()
{
    QAction* maximizeAction = nullptr;

    maximizeAction = new QAction( QString( "Maximize" ), this );
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

} // namespace como
