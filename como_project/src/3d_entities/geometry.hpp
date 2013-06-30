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

#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

/***
 * Includes
 ***/
#include "drawable_entity.hpp"
#include <vector>


/***
 * File main class
 ***/

class Geometry : public DrawableEntity
{
    protected:
        GLuint vao; // VAO : Vertex-Attributes Array
        GLuint vbo; // VBO : Vertex-Buffer Array
        GLuint ebo; // EBO : Elements-Buffer Array

        //glm::vec3 color;
        std::vector< glm::vec3 > originalVertices;
        std::vector< glm::vec3 > originalNormals;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        Geometry();
        ~Geometry();

        /***
         * 2. Update and drawing.
         ***/
    protected:
        // Recompute transformed vertices based on original ones and transformation matrix.
        virtual void update();

    public:
        // Send geometry to OpenGL server for rendering it.
        virtual void draw() const;
};

#endif // GEOMETRY_HPP
