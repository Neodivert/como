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

#ifndef RENDER_PANEL_HPP
#define RENDER_PANEL_HPP

#include <QGLWidget>
#include <QSplitter>
#include <QOpenGLContext>
#include "view_frame.hpp"
#include "../utilities/shader_loader.hpp"
#include <QOffscreenSurface>

QT_FORWARD_DECLARE_CLASS(QLabel)
QT_FORWARD_DECLARE_CLASS(QSlider)
QT_FORWARD_DECLARE_CLASS(QSplitter)


class RenderPanel : public QFrame
{
    Q_OBJECT

    private:
        // Pointer to shared OpenGL context.
        shared_ptr<QOpenGLContext> oglContext;

        // Pointer to current 3D scene.
        shared_ptr<Scene> scene;

    public:
        /***
         * 1. Initialization
         ***/
        explicit RenderPanel( shared_ptr<QOpenGLContext> oglContext, shared_ptr<Scene> scene, QWidget *parent = 0 );

    signals:

    public slots:
    
};

#endif // RENDER_PANEL_HPP
