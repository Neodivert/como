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

#include <client/models/utilities/msl/src/shader_loader.hpp>
#include <map>
#include <memory>

namespace como {

enum class ShaderVariable {
    LIGHTING_ENABLED
};

class OpenGL
{
    private:
        // ID of the current shader program.
        GLint currentShaderProgram_;

        //
        std::map< ShaderVariable, GLint > sharedVariablesLocations_;

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
         * 3. Lighting
         ***/
        void enableLighting() const;
        void disableLighting() const;


        /***
         * 4. Utilities
         ***/
    private:
        GLint getShaderVariableLocation( std::string varName ) const;
    public:


        /***
         * 5. Checking
         ***/
        static void checkStatus( std::string location );
        static std::string getOpenGLErrorString( GLenum errorCode );


        /***
         * 6. Operators
         ***/
        OpenGL& operator = ( const OpenGL& ) = delete;
        OpenGL& operator = ( OpenGL&& ) = delete;
};

typedef std::shared_ptr< OpenGL > OpenGLPtr;

} // namespace como

#endif // OPEN_GL_HPP
