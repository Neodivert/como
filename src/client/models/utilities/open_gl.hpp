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

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

extern "C" {
    #include <GL/gl.h>
}

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


// TODO: Use this class as much as possible for OpenGL work.
// TODO: Use locks (monitor).
class OpenGL
{
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
        ~OpenGL();


        /***
         * 3. Getters
         ***/
        ShadingMode getShadingMode() const;
        GLint getShaderInteger( ShaderProgramType shaderProgramType, std::string varName );
        GLuint getShaderProgramID( ShaderProgramType shaderProgramType ) const; // TODO: Remove this method.


        /***
         * 4. Setters
         ***/
        void setShadingMode( ShadingMode shadingMode );
        void setMVPMatrix( const glm::mat4& modelMatrix, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix );


        /***
         * 5. Utilities
         ***/
        GLint getShaderVariableLocation( std::string varName, GLint program = -1 ) const;


        /***
         * 6. Checking
         ***/
        static void checkStatus( std::string location );
        static std::string getOpenGLErrorString( GLenum errorCode );


        /***
         * 7. Operators
         ***/
        OpenGL& operator = ( const OpenGL& ) = delete;
        OpenGL& operator = ( OpenGL&& ) = delete;


    private:
        /***
         * 8. Setters (private)
         ***/
        void setProgram( ShaderProgramType program );


        /***
         * 9. Lighting
         ***/
        void enableLighting() const;
        void disableLighting() const;


        /***
         * 10. Texturing
         ***/
        void enableTexturing() const;
        void disableTexturing() const;


        /***
         * Attributes
         ***/
        // Index of the current shader program.
        GLuint currentProgramID_;

        std::map< ShaderProgramType, GLuint > shaderProgramsIDs_;

        ShadingMode currentShadingMode_;
};

typedef std::shared_ptr< OpenGL > OpenGLPtr;

} // namespace como

#endif // OPEN_GL_HPP
