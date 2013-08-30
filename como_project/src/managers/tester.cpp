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

#include "tester.hpp"
#include <cassert>

namespace como {

Tester* Tester::instance = nullptr;


/***
 * 1. Initialization and destruction
 ***/

Tester::Tester()
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

    QOpenGLContext oglContext;

    // Create an empty OpenGL context and make it use this surface's format.
    oglContext.setFormat( this->format() );

    // Initialize the OpenGL context and make it the current one.
    oglContext.create();
    oglContext.makeCurrent( this );

    // Obtain a functions object and resolve all entry points
    QAbstractOpenGLFunctions* oglFunctions = oglContext.versionFunctions();
    if ( !oglFunctions ) {
        qWarning( "Could not obtain OpenGL versions object" );
        exit( 1 );
    }
    oglFunctions->initializeOpenGLFunctions();

    // Load shaders
    ShaderLoader* shaderLoader = ShaderLoader::getInstance();
    shaderLoader->loadMinimumShaderProgram( "data/shaders/basicVertexShader.shader", "data/shaders/basicFragmentShader.shader" );
    shaderLoader->destroy();
}

Tester* Tester::getInstance()
{
    if( instance == nullptr ){
        instance = new Tester;
    }
    return instance;
}

void Tester::destroy()
{
    delete instance;
}


/***
 * 3. Tests related to meshes
 ***/

void Tester::testMeshes() const
{
    testMeshTranslations();
}

void Tester::testMeshTranslations() const
{
    std::cout << "Running test [Mesh Translations] ..." << std::endl;
    const unsigned int N_TRIANGLES = 6;
    const unsigned int N_VERTICES = N_TRIANGLES * 3;
    GLubyte elements[N_VERTICES];
    GLfloat originalVertices[N_VERTICES];
    GLfloat transformedVertices[N_VERTICES];
    Mesh mesh;

    unsigned int i = 0;
    unsigned int n;

    // Mesh are always rendered indirectly (by using indices). For simplifying, here
    // we simulate a direct rendering by incrementing indices one by one.
    for( i=0; i<N_VERTICES; i++ ){
        elements[i] = i;
    }

    // Generate a set of random values for the Mesh's original vertices.
    generateRandomValues( N_VERTICES, originalVertices );

    // Set mesh's vertices and indices.
    mesh.setVertices( N_VERTICES, originalVertices );
    mesh.setElements( N_VERTICES, elements );

    mesh.getTransformedVertices( n, transformedVertices );
    assert( n == N_VERTICES );
    for( i=0; i<n; i++ ){
        assert( transformedVertices[i] == originalVertices[i] );
    }
    std::cout << "Running test [Mesh Translations] ...OK" << std::endl;
}


} // namespace como
