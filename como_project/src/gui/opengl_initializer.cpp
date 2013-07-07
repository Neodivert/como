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

#include "opengl_initializer.hpp"

OpenGLInitializer::OpenGLInitializer()
{
    create();

    // Create a surface format for OpenGL 4.2 Core.
    // http://stackoverflow.com/questions/11000014/cant-set-desired-opengl-version-in-qglwidget
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(2);
    format.setProfile( QSurfaceFormat::CoreProfile );
    //format.setSampleBuffers(true);

    // Take previous format for this OpenGL context.
    oglContext = shared_ptr< QOpenGLContext >( new QOpenGLContext );
    oglContext->setFormat( format );

    // Set this surface's format.
    setFormat( format );

    // Create the OpenGL context and make it the current one.
    cout << "oglContext->create() : " << oglContext->create() << endl;
    //cout << "oglContext->makeCurrent() : " << oglContext->makeCurrent( this ) << endl;
    cout << "Context is valid?: " << oglContext->isValid() << endl;
    cout << "Context->version: " << oglContext->format().majorVersion() << " . " << oglContext->format().minorVersion() << endl;

    // TODO: Do I have to do this or call glewInit()?

    // Obtain a functions object and resolve all entry points
    QAbstractOpenGLFunctions* oglFunctions = oglContext->versionFunctions();
    if ( !oglFunctions ) {
        qWarning( "Could not obtain OpenGL versions object" );
        exit( 1 );
    }
    oglFunctions->initializeOpenGLFunctions();

    // Make previous OpenGL context current for this window.
    cout << "Making OGL context current for AppWindow: " << oglContext->makeCurrent( this ) << endl;

    // Load shaders
    ShaderLoader* shaderLoader = ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    shaderLoader->destroy();
}


shared_ptr< QOpenGLContext > OpenGLInitializer::context()
{
    return oglContext;
}
