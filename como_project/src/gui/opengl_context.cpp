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

    // Create a surface format for OpenGL 4.2 Core.
    // http://stackoverflow.com/questions/11000014/cant-set-desired-opengl-version-in-qglwidget
    QSurfaceFormat format;
    format.setMajorVersion(4);
    format.setMinorVersion(2);
    format.setProfile( QSurfaceFormat::CoreProfile );
    //format.setSampleBuffers(true);

    // Take previous format for this OpenGL context.
    setFormat( format );

    // Create the OpenGL context and make it the current one.
    cout << "oglContext->create() : " << create() << endl;
    //cout << "oglContext->makeCurrent() : " << oglContext->makeCurrent( this ) << endl;
    cout << "Context is valid?: " << isValid() << endl;
    cout << "Context->version: " << this->format().majorVersion() << " . " << this->format().minorVersion() << endl;

    // TODO: Do I have to do this or call glewInit()?

    // Obtain a functions object and resolve all entry points
    QAbstractOpenGLFunctions* oglFunctions = versionFunctions();
    if ( !oglFunctions ) {
        qWarning( "Could not obtain OpenGL versions object" );
        exit( 1 );
    }
    oglFunctions->initializeOpenGLFunctions();
}
