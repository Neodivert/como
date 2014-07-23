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

#ifndef OPEN_GL_HPP
#define OPEN_GL_HPP

#include <map>
#include <memory>
#include <client/models/utilities/msl/src/shader_loader.hpp>

namespace como {

enum class ShaderProgramType {
    DEFAULT,
    NORMALS
};

enum class ShadingMode {
    SOLID_LIGHTING_AND_TEXTURING,
    SOLID_LIGHTING,
    SOLID_PLAIN,
    NORMALS
};


class OpenGL
{
    private:
        // Index of the current shader program.
        GLint currentProgramID_;

        std::map< ShaderProgramType, GLint > shaderProgramsIDs_;

    public:
        /***
         * 1. Construction
         ***/
        OpenGL();
        OpenGL( const OpenGL& ) = delete;
        OpenGL( OpenGL&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~OpenGL() = default;


        /***
         * 3. Setters
         ***/
        void setShadingMode( ShadingMode shadingMode );


    private:
        void setProgram( ShaderProgramType program );

        /***
         * 4. Lighting
         ***/
        void enableLighting() const;
        void disableLighting() const;


        /***
         * 5. Texturing
         ***/
        void enableTexturing() const;
        void disableTexturing() const;


        /***
         * 6. Utilities
         ***/
    private:
        GLint getShaderVariableLocation( std::string varName ) const;
    public:


        /***
         * 7. Checking
         ***/
        static void checkStatus( std::string location );
        static std::string getOpenGLErrorString( GLenum errorCode );


        /***
         * 8. Operators
         ***/
        OpenGL& operator = ( const OpenGL& ) = delete;
        OpenGL& operator = ( OpenGL&& ) = delete;
};

typedef std::shared_ptr< OpenGL > OpenGLPtr;

} // namespace como

#endif // OPEN_GL_HPP
