/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "render_panel.hpp"
#include <QSplitter>

namespace como {

/***
 * 1. Initialization
 ***/

RenderPanel::RenderPanel( shared_ptr< ComoApp > comoApp, QWidget *parent ) :
    QFrame( parent )
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
    ViewFrame *viewFrame = new ViewFrame( "Top left view", comoApp );
    h1Splitter->addWidget( viewFrame );

    // Create the top right ViewFrame.
    viewFrame = new ViewFrame( "Top right view", comoApp );
    h1Splitter->addWidget( viewFrame );

    // Create the bottom left ViewFrame.
    viewFrame = new ViewFrame( "Bottom left view", comoApp );
    h2Splitter->addWidget( viewFrame );

    // Create the Bottom right ViewFrame.
    viewFrame = new ViewFrame( "Bottom right view", comoApp );
    h2Splitter->addWidget( viewFrame );

    // Set the render panel layout by using previous splitters.
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget( vSplitter );
    setLayout( layout );
}

} // namespace como
