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

#ifndef MESH_HPP
#define MESH_HPP

/***
 * Includes
 ***/
#include "drawable.hpp"
#include <vector>
#include <array>

namespace como {

const GLuint COMPONENTS_PER_VERTEX = 3;
const GLuint N_TRIANGLE_VERTICES = 3;

/***
 * File main class
 ***/

class Mesh : public Drawable
{
    private:
        // Location of the uniform shader variable used for coloring geometries.
        static GLint uniformColorLocation;

        // VAO : Vertex Attributes Array.
        GLuint vao;

        // VBO for transformed vertices.
        GLuint vbo;

        // // EBO : Elements-Buffer Array.
        GLuint ebo;

        // Colors
        GLfloat innerColor[4];
        GLfloat contourColor[4];

        // Original vertex attribues.
        std::vector< glm::vec3 > originalVertices;
        std::vector< glm::vec3 > originalNormals;

        // Mesh's triangles.
        std::vector< std::array< GLubyte, 3 > > triangles;

        // Mesh's centroid.
        glm::vec4 originalCentroid;
        glm::vec4 transformedCentroid;

        // Number of elements in mesh.
        GLuint nElements;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Mesh();
        ~Mesh();

        virtual void setVertices( const GLuint nVertices, const GLfloat* vertices );
        virtual void setElements( const GLuint nElements, const GLubyte* elements );


        /***
         * 2. Getters and setters
         ***/
        void setInnerColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a );
        void setContourColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a );
        glm::vec4 getCentroid() const ;
        void getTransformedVertices( unsigned int& n, GLfloat* vertices );

        /***
         * 3. Transformations
         ***/
        //virtual void rotateAroundCentroid( const GLfloat& angle, const glm::vec3& axis );

        /***
         * 4. Intersections
         ***/
        virtual void intersects( glm::vec3 rayOrigin, glm::vec3 rayDirection, float& minT, unsigned int* triangle = nullptr ) const ;

        /***
         * 5. Update and drawing.
         ***/
    protected:
        // Recompute transformed vertices based on original ones and transformation matrix.
        virtual void update();

    public:
        // Send mesh to OpenGL server for rendering it.
        virtual void draw( bool selected = false ) const;
};

}

#endif // MESH_HPP
