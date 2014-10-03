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

#ifndef COORDINATE_AXES_HPP
#define COORDINATE_AXES_HPP

#include <client/models/utilities/open_gl.hpp>
#include <vector>
#include <common/packables/packable_color.hpp>

namespace como {

enum class Axis {
    X = 0,
    Y,
    Z
};

class AuxiliarLinesRenderer
{
    public:
        /***
         * 1. Construction
         ***/
        AuxiliarLinesRenderer( OpenGL& openGL, const Color& userColor );
        AuxiliarLinesRenderer() = delete;
        AuxiliarLinesRenderer( const AuxiliarLinesRenderer& ) = delete;
        AuxiliarLinesRenderer( AuxiliarLinesRenderer&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~AuxiliarLinesRenderer();


        /***
         * 3. Setters
         ***/
        void setTransformGuideLine( const glm::vec3& origin, const glm::vec3& destiny );


        /***
         * 4. Operators
         ***/
        AuxiliarLinesRenderer& operator = ( const AuxiliarLinesRenderer& ) = delete;
        AuxiliarLinesRenderer& operator = ( AuxiliarLinesRenderer&& ) = delete;


        /***
         * 5. Drawing
        ***/
        void drawGuideAxis( Axis axis,
                            const glm::mat4& viewMatrix,
                            const glm::mat4& projectionMatrix );

        void drawWorldAxes( const glm::mat4& viewMatrix ) const;

        void drawTransformGuideLine( const glm::mat4 &viewMatrix,
                                     const glm::mat4 &projectionMatrix) const;


        /***
         * 6. Initialization
         ***/
        void initGuideRectsColor( const Color& userColor );
        void initWorldAxesData();
        void initGuideAxesData();
        void initTransformGuideLineData();

        GLuint generateVBO( const std::vector<GLfloat>& vboData, GLenum usage );
        GLuint generateVAO();


    private:
        OpenGL* openGL_;

        GLuint worldAxesVAO_;
        GLuint worldAxesVBO_;

        GLuint guideAxesVAO_;
        GLuint guideAxesVBO_;

        GLuint transformGuideLineVAO_;
        GLuint transformGuideLineVBO_;

        GLint colorShaderLocation_;
        glm::vec4 guideRectsColor_;
};


} // namespace como

#endif // COORDINATE_AXES_HPP
