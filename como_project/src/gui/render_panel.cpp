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

void LoadShaders()
{

}

RenderPanel::RenderPanel( QWidget* parent )
    /*(parent)*/ /*, QOpenGLWidget(parent)*/
{
    //hide();
    //setSurfaceType( QSurface::OpenGLSurface );




    /*
    glEnableClientState( GL_VERTEX_ARRAY );
    */
    // Load shaders.


    // Load empty scene.
    /*
    scene = new Scene;
    scene->addCube( new Cube );
    scene->selectAll();

    h1Splitter = new QSplitter;
    h2Splitter = new QSplitter;

    QSplitter *vSplitter = new QSplitter;
    vSplitter->setOrientation(Qt::Vertical);
    vSplitter->addWidget(h1Splitter);
    vSplitter->addWidget(h2Splitter);

    ViewFrame *viewFrame = new ViewFrame( "Top left view", glContext, scene );
    h1Splitter->addWidget( viewFrame );
    cout << "Context is valid?: " << glContext->isValid() << endl;
    /*
    viewFrame = new ViewFrame("Top right view", glContext, scene );
    h1Splitter->addWidget( viewFrame );
    cout << "Context is valid?: " << glContext->isValid() << endl;

    viewFrame = new ViewFrame("Bottom left view", glContext, scene );
    h2Splitter->addWidget( viewFrame );
    cout << "Context is valid?: " << glContext->isValid() << endl;

    viewFrame = new ViewFrame("Bottom right view", glContext, scene );
    h2Splitter->addWidget( viewFrame );
    cout << "Context is valid?: " << glContext->isValid() << endl;
    */
    /*
    QHBoxLayout *layout = new QHBoxLayout;
    //layout->addWidget(vSplitter);
    layout->addWidget( viewFrame );
    setLayout(layout);
    */
    cout << "Render panel constructor OK" << endl;
}

/*
void RenderPanel::initializeGL()
{
    cout << "RenderPanel::initializeGL" << endl;
    glContext->makeCurrent(this);
    initializeOpenGLFunctions();

    ShaderLoader* shaderLoader = ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    shaderLoader->destroy();
}

/*

QSurfaceFormat RenderPanel::format() const
{
    return &format;
}

QPlatformSurface* surfaceHandle() const
{
    return QSurface::OpenGLSurface;
}


QSurface::SurfaceType RenderPanel::surfaceType() const
{
    return QSurface::OpenGLSurface;
}


virtual QSize size() const;
*/

/*
void RenderPanel::initializeGL()
{
    //LoadShaders();
    ShaderLoader* shaderLoader = ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    ShaderLoader::destroy();
    //

    // Set OpenGL depth test.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    // Initialize geometry.
    scene = new Scene;
    scene->addCube( new Cube );
    scene->selectAll();


    ViewFrame* viewFrame = new ViewFrame("Top right view", this->context(), scene );

    QHBoxLayout *layout = new QHBoxLayout;
    layout->addWidget( viewFrame );
    setLayout(layout);
}
*/
