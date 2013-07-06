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

#include "opengl_context.hpp"

OpenGLContext::OpenGLContext()
{
    create();

    // http://stackoverflow.com/questions/11000014/cant-set-desired-opengl-version-in-qglwidget
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(2);
    format.setProfile( QSurfaceFormat::CoreProfile );
    //qglFormat.setSampleBuffers(true);

    oglContext = new QOpenGLContext();
    oglContext->setFormat( format );
    cout << "oglContext->create() : " << oglContext->create() << endl;
    cout << "oglContext->makeCurrent() : " << oglContext->makeCurrent( this ) << endl;

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

    // Load shaders.
    ShaderLoader* shaderLoader = ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    shaderLoader->destroy();
}


bool OpenGLContext::makeCurrent(QSurface * surface)
{
    return oglContext->makeCurrent( surface );
}
