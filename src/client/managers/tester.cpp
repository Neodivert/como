/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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

Tester::Tester( LogPtr log ) :
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

    oglContext = new QOpenGLContext;

    // Create an empty OpenGL context and make it use this surface's format.
    oglContext->setFormat( this->format() );

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
}

Tester::~Tester()
{
    delete oglContext;
}

Tester* Tester::getInstance( LogPtr log )
{
    if( instance == nullptr ){
        instance = new Tester( log );
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
    glm::vec3 acumTranslation( 0.0f );
    glm::vec3 translation( 0.0f );
    unsigned int currentTest = 0;
    const unsigned int N_TESTS = 50;
    unsigned int i = 0;
    unsigned int n;
    const unsigned int N_TRIANGLES = 6;

    GLfloat originalVertexData[N_TRIANGLES*3*3];
    GLfloat transformedVertexData[N_TRIANGLES*3*3];

    log_->debug( "Running test [Mesh Translations - Number of tests: ", N_TESTS, ")] ...\n" );

    Mesh* mesh = generateRandomMesh( N_TRIANGLES );
    mesh->getTransformedVertices( n, originalVertexData );

    for( currentTest=0; currentTest<N_TESTS; currentTest++ ){
        translation.x = generateRandomFloat( MIN_FLOAT, MAX_FLOAT );
        translation.y = generateRandomFloat( MIN_FLOAT, MAX_FLOAT );
        translation.z = generateRandomFloat( MIN_FLOAT, MAX_FLOAT );


        log_->debug( "translation: (", translation.x, ", ", translation.y, ", ", translation.z, ")\n" );
        acumTranslation += translation;

        mesh->translate( translation );

        mesh->getTransformedVertices( n, transformedVertexData );
        assert( n == N_TRIANGLES*3 );
        for( i=0; i < (n * COMPONENTS_PER_VERTEX); i+=3 ){
            assert( transformedVertexData[i] == ( originalVertexData[i] + acumTranslation.x ) );
            assert( transformedVertexData[i+1] == ( originalVertexData[i+1] + acumTranslation.y ) );
            assert( transformedVertexData[i+2] == ( originalVertexData[i+2] + acumTranslation.z ) );
        }
    }

    delete mesh;
    log_->debug( "Running test [Mesh Translations] ...OK\n" );
}


/***
 * 4. Auxiliar methods
 ***/

Mesh* Tester::generateRandomMesh( const unsigned int& nTriangles ) const
{
    const unsigned int N_VERTICES = nTriangles * 3;
    GLubyte* elements = nullptr;
    GLfloat* originalVertexData = nullptr;
    GLfloat* transformedVertexData = nullptr;
    Mesh* mesh = nullptr;
    unsigned int i = 0;
    unsigned int n;


    // Mesh are always rendered indirectly (by using indices). For simplifying, here
    // we simulate a direct rendering by incrementing indices one by one.
    elements = new GLubyte[nTriangles*3];
    for( i=0; i<N_VERTICES; i++ ){
        elements[i] = i;
    }

    // Generate a vector with random vertex data for the mesh.
    originalVertexData = new GLfloat[nTriangles*3*3];
    generateRandomValues( N_VERTICES * 3, originalVertexData );

    // Generate a mesh and set its vertices and indices.
    mesh = new Mesh;
    mesh->setVertices( N_VERTICES, originalVertexData );
    mesh->setElements( N_VERTICES, elements );

    // When initialized, mesh's transformed vertices are equal to the original
    // ones (the transformation matrix is an identity one).
    // Get transformed vertices from mesh and check this equality.
    transformedVertexData = new GLfloat[nTriangles*3*3];
    mesh->getTransformedVertices( n, transformedVertexData );
    assert( n == N_VERTICES );
    for( i=0; i < (n * COMPONENTS_PER_VERTEX); i++ ){
        assert( transformedVertexData[i] == originalVertexData[i] );
    }

    // Free resources.
    delete [] elements;
    delete [] originalVertexData;
    delete [] transformedVertexData;

    // Return generated mesh.
    return mesh;
}


/***
 * 5. Auxiliar functions
 ***/

float generateRandomFloat( float min, float max )
{
    float randomFloat = (float)rand() / (float)RAND_MAX;

    randomFloat = randomFloat * (max - min) + min;

    return randomFloat;
}


} // namespace como
