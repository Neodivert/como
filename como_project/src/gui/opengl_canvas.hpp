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


/***
 * Includes
 ***/

#include <QGLWidget>
#include "../3d_entities/cube.hpp"
#include "../utilities/shader_loader.hpp"


/***
 * File main class
 ***/

class OpenGLCanvas : public QGLWidget
{
    Q_OBJECT

    /***
     * 1. Initialization and destruction
     ***/
public:
    explicit OpenGLCanvas(QWidget *parent = 0);
    ~OpenGLCanvas();

protected:
    // TODO: move to another place.
    Cube* cube;

    // Initialize OpenGL context.
    void initializeGL();

    /***
     * 2. Events
     ***/
    void keyPressEvent( QKeyEvent *e );

    /***
     * 3. Updating and drawing
     ***/
    // Resize OpenGL canvas.
    void resizeGL( int w, int h );

    // Render OpenGL canvas.
    void paintGL();

    
signals:
    
public slots:
    
};

#endif // OPENGL_CANVAS_HPP
