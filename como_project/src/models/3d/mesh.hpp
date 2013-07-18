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
#include "camera.hpp"
#include <vector>
#include <array>


namespace como {

/***
 * File main class
 ***/

class Mesh : public Drawable
{
    protected:
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

        // Number of inner and contour elements to be renderer (for indexed drawing).
        GLsizei nInnerElements;
        GLsizei nContourElements;


        std::vector< std::array< GLubyte, 3 > > triangles;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Mesh();
        ~Mesh();

        /***
         * 2. Getters and setters
         ***/
        void setInnerColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a );
        void setContourColor( const GLfloat& r, const GLfloat& g, const GLfloat& b, const GLfloat& a );

        /***
         * 3. Intersections
         ***/
        virtual void intersects( glm::vec3 rayOrigin, glm::vec3 rayDirection, float& minT, unsigned int* triangle = nullptr ) const ;

        /***
         * 4. Update and drawing.
         ***/
    protected:
        // Recompute transformed vertices based on original ones and transformation matrix.
        virtual void update();

    public:
        // Send mesh to OpenGL server for rendering it.
        virtual void draw( Camera* camera, bool selected = false ) const;
};

}

#endif // MESH_HPP
