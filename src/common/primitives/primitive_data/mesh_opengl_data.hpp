/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License v3 as published by
 * the Free Software Foundation.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef MESH_OPENGL_DATA_HPP
#define MESH_OPENGL_DATA_HPP

#include <vector>

extern "C" {
    #include <GL/gl.h>
}

namespace como {

typedef std::vector< GLfloat > GLFloatBuffer;
typedef std::vector< GLuint > GLUintBuffer;


struct MeshOpenGLData {
    bool includesTextures;

    GLFloatBuffer vboData;
    GLUintBuffer eboData;


    /***
     * 1. Construction
     ***/
    MeshOpenGLData();


    /***
     * 2. Getters
     ***/
    GLuint componentsPerVertex() const;
};

} // namespace como

#endif // MESH_OPENGL_DATA_HPP
