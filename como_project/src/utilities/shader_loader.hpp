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

#include <fstream>
#include "../3d_entities/drawable_entity.hpp"

class ShaderLoader
{
public:
    ShaderLoader();

    void readFile( const char* file, GLchar* buffer, const unsigned int n );
    void loadShaders( const char* vertexShaderFile, const char* fragmentShaderFile );
};

#endif // SHADER_LOADER_HPP
