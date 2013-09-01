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

#ifndef SHADER_LOADER_HPP
#define SHADER_LOADER_HPP

/***
 * Includes
 ***/
#include <fstream>
#include "../models/dependencies.hpp"

namespace como {

/***
 * File main class
 ***/
class ShaderLoader
{
    private:
        // Singlenton pattern: only one static instance and a private constructor.
        static ShaderLoader* instance;

        GLuint shaderProgram;

        /***
         * 1. Initialization and destruction
         ***/
        ShaderLoader();
        ~ShaderLoader();

    public:
        static ShaderLoader* getInstance();
        static void destroy();

    protected:
        /***
         * 2. Utilities
         ***/
        void readFile( const char* file, GLchar* buffer, const unsigned int n );

        /***
         * 3. Shader loading
         ***/
        void loadShaderObject( GLenum shaderType, const char* shaderFile );
    public:
        void loadMinimumShaderProgram( const char* vertexShaderFile, const char* fragmentShaderFile );
};

} // namespace como

#endif // SHADER_LOADER_HPP
