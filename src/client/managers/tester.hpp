/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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
#include "../models/utilities/msl/src/shader_loader.hpp"
#include "../../common/utilities/log.hpp"

namespace como {

const float MIN_FLOAT = -500.0f;
const float MAX_FLOAT = 500.0f;

class Tester : public QOffscreenSurface
{
    private:
        static Tester* instance;
        QOpenGLContext* oglContext;

        // Log
        LogPtr log_;


        /***
         * 1. Initialization and destruction
         ***/
    public:
        Tester( LogPtr log );
        ~Tester();

        static Tester* getInstance( LogPtr log );
        void destroy();


        /***
         * 2. General tests
         ***/


        /***
         * 3. Tests related to meshes
         ***/
        void testMeshes() const ;
        void testMeshTranslations() const ;


        /***
         * 4. Auxiliar methods
         ***/
        Mesh* generateRandomMesh( const unsigned int& nTriangles ) const ;
};


/***
 * 5. Auxiliar functions
 ***/

template <class T>
void generateRandomValues( unsigned int n, T* vector )
{
    for( unsigned int i=0; i<n; i++ ){
        vector[i] = (T)rand()/(T)RAND_MAX;
    }
}

float generateRandomFloat( float min, float max );

} // namespace como

#endif // TESTER_HPP


