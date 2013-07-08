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

#include "main_window.hpp"
#include "ui_main_window.h"
#include "opengl_initializer.hpp"
#include "render_panel.hpp"


/***
 * 1. Initialization and destruction
 ***/

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create an retrieve an OpenGL context.
    OpenGLInitializer openGLInitializer;
    oglContext = openGLInitializer.context();

    // Get empty scene from OpenGL initializer.
    scene = openGLInitializer.getScene();

    // Set a QFrame as the central widget. This frame will hold all others widgets.
    setCentralWidget( new QFrame );

    // Add to window a RenderPanel (Set of four OpenGL canvas).
    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget( new RenderPanel( oglContext, scene )  );
    centralWidget()->setLayout( layout );
}


MainWindow::~MainWindow()
{
    delete ui;
}
