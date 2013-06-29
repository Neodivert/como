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

#include <iostream>
using namespace std;

#ifndef DRAWABLE_ENTITY_HPP
#define DRAWABLE_ENTITY_HPP



//#include <GL/glew.h>

// http://www.opengl.org/discussion_boards/showthread.php/172481-glGenBuffer-was-not-declared
#define GL_GLEXT_PROTOTYPES
#include <QtOpenGL>

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glx.h>


#include <glm/glm.hpp>


class DrawableEntity
{
public:
    virtual void draw() const = 0;

    //virtual void translate( const GLfloat& tx, const GLfloat& ty, const GLfloat& tz ) = 0;

protected:
    void update();
};

#endif // DRAWABLE_ENTITY_HPP
