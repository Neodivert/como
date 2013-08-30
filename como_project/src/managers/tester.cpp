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

#include "tester.hpp"

namespace como {

Tester* Tester::instance = nullptr;


/***
 * 1. Initialization and destruction
 ***/

Tester* Tester::getInstance()
{
    if( instance == nullptr ){
        instance = new Tester;
    }
    return instance;
}

void Tester::destroy()
{
    delete instance;
}


/***
 * 3. Tests related to meshes
 ***/

void Tester::testMeshes() const
{
    testMeshTranslations();
}

void Tester::testMeshTranslations() const
{
    /*
    const unsigned int N_TRIANGLES = 6;
    const unsigned int N_VERTICES = N_TRIANGLES * 3;
    GLubyte elements[N_VERTICES];
    unsigned int i = 0;

    // Mesh are always rendered indirectly (by using indices). For simplifying, here
    // we simulate a direct rendering by incrementing indices one by one.
    for( i=0; i<N_VERTICES; i++ ){
        elements[i] = i;
    }
    */
}


} // namespace como
