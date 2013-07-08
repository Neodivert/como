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

#ifndef OPENGL_INITIALIZER_HPP
#define OPENGL_INITIALIZER_HPP

#include <QOpenGLContext>
#include "../utilities/shader_loader.hpp"
#include "../3d_entities/scene.hpp"
#include <iostream>
#include <memory>
using namespace std;

class OpenGLInitializer : public QOffscreenSurface
{
    private:
        // Pointer to shared OpenGL context.
        shared_ptr<QOpenGLContext> oglContext;

        // Pointer to current 3D scene.
        shared_ptr<Scene> scene;

    public:
        /***
         * 1. Initialization
         ***/
        OpenGLInitializer();

        /***
         * 2. Getters
         ***/
        shared_ptr< QOpenGLContext > context();
        shared_ptr< Scene > getScene();
};

#endif // OPENGL_INITIALIZER_HPP
