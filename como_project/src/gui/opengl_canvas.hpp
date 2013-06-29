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

#ifndef OPENGL_CANVAS_HPP
#define OPENGL_CANVAS_HPP

#include <QGLWidget>
#include "../3d_entities/cube.hpp"
#include "../utilities/shader_loader.hpp"

class OpenGLCanvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLCanvas(QWidget *parent = 0);

protected:
    Cube cube;

    void initializeGL();
    void paintGL();
    void resizeGL( int w, int h );
    
signals:
    
public slots:
    
};

#endif // OPENGL_CANVAS_HPP
