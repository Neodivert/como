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

#include "render_panel.hpp"
#include <QSplitter>

namespace como {

/***
 * 1. Initialization
 ***/

RenderPanel::RenderPanel( QWidget* parent, shared_ptr< ComoApp > comoApp ) :
    QFrame( parent ),
    viewFrames_{ nullptr }
{
    QSplitter* vSplitter;
    QSplitter* h1Splitter;
    QSplitter* h2Splitter;

    // Make this render panel share the given app's state.
    this->comoApp = comoApp;

    // Create splitters for arranging the four ViewFrame instances we are about to create.
    vSplitter = new QSplitter;
    h1Splitter = new QSplitter;
    h2Splitter = new QSplitter;

    // Create the splitters hierarchy.
    vSplitter->setOrientation( Qt::Vertical );
    vSplitter->addWidget( h1Splitter );
    vSplitter->addWidget( h2Splitter );

    // Create the top left ViewFrame.
    viewFrames_[0] = new ViewFrame( View::FRONT, comoApp );
    h1Splitter->addWidget( viewFrames_[0] );

    // Create the top right ViewFrame.
    viewFrames_[1] = new ViewFrame( View::RIGHT, comoApp );
    h1Splitter->addWidget( viewFrames_[1] );

    // Create the bottom left ViewFrame.
    viewFrames_[2] = new ViewFrame( View::TOP, comoApp );
    h2Splitter->addWidget( viewFrames_[2] );

    // Create the Bottom right ViewFrame.
    viewFrames_[3] = new ViewFrame( View::FRONT, comoApp );
    h2Splitter->addWidget( viewFrames_[3] );

    // Set the render panel layout by using previous splitters.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget( vSplitter );
    setLayout( layout );

    // Set a timer and use it for rendering the scene on all viewports every
    // X miliseconds.
    QTimer* timer = new QTimer( this );
    connect( timer, &QTimer::timeout, this, &RenderPanel::renderSceneIfChanged );
    timer->start( 22 );
}


RenderPanel::~RenderPanel()
{
    comoApp->getLog()->debug( "RenderPanel destroyed\n" );
}


/***
 * 3. Slots
 ***/

void RenderPanel::renderSceneIfChanged()
{
    if( comoApp->getScene()->hasChangedSinceLastQuery() ){
        for( unsigned int i=0; i<4; i++ ){
            viewFrames_[i]->render();
        }
    }
}

} // namespace como
