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

#ifndef TESTER_HPP
#define TESTER_HPP

#include "../models/3d/mesh.hpp"
#include "shader_loader.hpp"

namespace como {

class Tester : public QOffscreenSurface
{
    private:
        static Tester* instance;

        /***
         * 1. Initialization and destruction
         ***/
    public:
        Tester();

        static Tester* getInstance();
        void destroy();


        /***
         * 2. General tests
         ***/

        /***
         * 3. Tests related to meshes
         ***/
        void testMeshes() const ;
        void testMeshTranslations() const ;
};

/***
 * 3. Auxiliar functions
 ***/

template <class T>
void generateRandomValues( unsigned int n, T* vector )
{
    for( unsigned int i=0; i<n; i++ ){
        vector[i] = (T)rand()/(T)RAND_MAX;
    }
}

} // namespace como

#endif // TESTER_HPP


