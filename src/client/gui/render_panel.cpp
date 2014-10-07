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

#include <QSplitter>
#include "render_panel.hpp"


namespace como {

/***
 * 1. Initialization
 ***/

RenderPanel::RenderPanel( QWidget* parent, std::shared_ptr< ComoApp > comoApp ) :
    QFrame( parent ),
    viewFrames_{ nullptr },
    forceRender_( true )
{
    QSplitter* vSplitter = nullptr;
    QSplitter* h1Splitter = nullptr;
    QSplitter* h2Splitter = nullptr;
    unsigned int i = 0;

    // Make this render panel share the given app's state.
    this->comoApp = comoApp;

    // Create splitters for arranging the four ViewFrame instances we are about to create.
    vSplitter = new QSplitter( Qt::Vertical );
    h1Splitter = new QSplitter( Qt::Horizontal );
    h2Splitter = new QSplitter( Qt::Horizontal );

    // Create the top left ViewFrame.
    viewFrames_[0] = new ViewFrame( View::FRONT, Projection::ORTHO, comoApp );
    h1Splitter->addWidget( viewFrames_[0] );

    // Create the top right ViewFrame.
    viewFrames_[1] = new ViewFrame( View::RIGHT, Projection::ORTHO, comoApp );
    h1Splitter->addWidget( viewFrames_[1] );

    // Create the bottom left ViewFrame.
    viewFrames_[2] = new ViewFrame( View::TOP, Projection::ORTHO, comoApp );
    h2Splitter->addWidget( viewFrames_[2] );

    // Create the Bottom right ViewFrame.
    viewFrames_[3] = new ViewFrame( View::CAMERA, Projection::PERSPECTIVE, comoApp );
    h2Splitter->addWidget( viewFrames_[3] );

    // Create the splitters hierarchy.
    vSplitter->addWidget( h1Splitter );
    vSplitter->addWidget( h2Splitter );

    // Maximize a viewport or another if requested.
    for( i = 0; i < 4; i++ ){
        QObject::connect(
                    viewFrames_[i],
                    &ViewFrame::viewFrameMaximizationRequested,
                    this,
                    &RenderPanel::maximizeViewFrame );

        QObject::connect(
                    viewFrames_[i],
                    &ViewFrame::viewFrameMinimizationRequested,
                    this,
                    &RenderPanel::minimizeViewFrames );
    }

    // Set the render panel layout by using previous splitters.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget( vSplitter );
    setLayout( layout );

    // Set a timer and use it for rendering the scene on all viewports every
    // X miliseconds.
    QTimer* timer = new QTimer( this );
    connect( timer, &QTimer::timeout, this, &RenderPanel::renderIfNeeded );
    timer->start( 22 );

    // Start observing the scene.
    this->comoApp->getScene()->addObserver( this );
}


RenderPanel::~RenderPanel()
{
    comoApp->getLog()->debug( "RenderPanel destroyed\n" );
}


/***
 * 3. Slots
 ***/

void RenderPanel::renderIfNeeded()
{
    unsigned int i;

    if( forceRender_ ){
        for( i = 0; i < 4; i++ ){
            viewFrames_[i]->render();
        }
    }else{
        for( i = 0; i < 4; i++ ){
            viewFrames_[i]->renderIfNeeded();
        }
    }
}


void RenderPanel::maximizeViewFrame( ViewFrame* viewFrame )
{
    unsigned int i;

    for( i = 0; i < 4; i++ ){
        if( viewFrames_[i] != viewFrame ){
            viewFrames_[i]->hide();
        }
    }
}


void RenderPanel::minimizeViewFrames()
{
    unsigned int i;

    for( i = 0; i < 4; i++ ){
        viewFrames_[i]->show();
    }
}


/***
 * 4. Updating (Observer)
 ***/

void RenderPanel::update()
{
    forceRender_ = true;
}

} // namespace como
