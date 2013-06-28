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

#include "drawable_entity.hpp"
#include <vector>

enum VBOS
{
    VERTEX_DATA = 0,
    VERTEX_INDICES,

    N_VBOS
};

class Geometry : public DrawableEntity
{
protected:
    GLuint vao;
    GLuint* vbos;

    glm::vec3 color;
    std::vector< glm::vec3 > originalVertices;
    std::vector< glm::vec3 > originalNormals;

public:
    Geometry();
    ~Geometry();

   virtual void draw() const;
};

#endif // GEOMETRY_HPP
