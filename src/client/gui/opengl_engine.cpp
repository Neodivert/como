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

#include "opengl_engine.hpp"
#include "../models/utilities/msl/src/shader_loader.hpp"

namespace como {


/***
 * 1. Initialization
 ***/

OpenGLEngine::OpenGLEngine( LogPtr log ) :
    log_( log )
{
    // Create a surface format for OpenGL 4.2 Core.
    // http://stackoverflow.com/questions/11000014/cant-set-desired-opengl-version-in-qglwidget
    QSurfaceFormat format;
    format.setMajorVersion( 4 );
    format.setMinorVersion( 2 );
    format.setProfile( QSurfaceFormat::CoreProfile );

    // Initialize this offscreen surface with previous format.
    setFormat( format );
    create();
}


/***
 * 2. Getters
 ***/

shared_ptr< QOpenGLContext > OpenGLEngine::createOpenGLContext()
{
    shared_ptr< QOpenGLContext > oglContext;

    // Create an empty OpenGL context and make it use this surface's format.
    oglContext = shared_ptr< QOpenGLContext >( new QOpenGLContext );
    oglContext->setFormat( format() );

    // Initialize the OpenGL context and make it the current one.
    oglContext->create();
    oglContext->makeCurrent( this );

    // Obtain a functions object and resolve all entry points
    QAbstractOpenGLFunctions* oglFunctions = oglContext->versionFunctions();
    if ( !oglFunctions ) {
        qWarning( "Could not obtain OpenGL versions object" );
        exit( 1 );
    }
    oglFunctions->initializeOpenGLFunctions();

    // Load shaders
    msl::ShaderLoader* shaderLoader = msl::ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    shaderLoader->destroy();

    // Set clear color.
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );

    // Set OpenGL depth test.
    glEnable( GL_DEPTH_TEST );
    glDepthFunc( GL_LEQUAL );

    return oglContext;
}

shared_ptr< Scene > OpenGLEngine::createPublicScene( QOpenGLContext* oglContext )
{
    // Make the given OpenGL context current for this surface.
    oglContext->makeCurrent( this );

    // Create and return an empty scene.
    shared_ptr< Scene > scene = shared_ptr<Scene>( new PublicScene( log_ ) );
    return scene;
}

} // namespace como
