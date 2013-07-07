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

MainWindow::MainWindow( QWidget* parent ) :
    QMainWindow( parent ),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Create an OpenGL context.
    oglContext = shared_ptr<OpenGLContext>( new OpenGLContext );

    // Use the previous OpenGL context's format as the format of this window.
    //setFormat( oglContext->format() );

    /*
    // Set this surface to be rendered by OpenGL. This is a "cheat" so we can make
    // previous OpenGL context current for this surface and so load the shaders.
    setSurfaceType( QSurface::OpenGLSurface );
    create();

    // Make previous OpenGL context current for this window.
    cout << "Making OGL context current for AppWindow: " << oglContext->makeCurrent( this ) << endl;

    // Load shaders.
    ShaderLoader* shaderLoader = ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    shaderLoader->destroy();

    // We are done with the OpenGL context for this window.
    oglContext->doneCurrent();

    // Set this window to be rendered by Qt engine again.
    setSurfaceType( QSurface::RasterSurface );
    create();
    */

    setCentralWidget( new QWidget );

    /*
    QHBoxLayout *layout = new QHBoxLayout;
    //layout->addWidget(vSplitter);
    layout->addWidget( new QFrame );
    setLayout(layout);
    */
}

MainWindow::~MainWindow()
{
    delete ui;
}
