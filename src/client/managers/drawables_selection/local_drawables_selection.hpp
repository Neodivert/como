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

#ifndef LOCAL_DRAWABLES_SELECTION_HPP
#define LOCAL_DRAWABLES_SELECTION_HPP

#include "drawables_selection.hpp"
#include <client/managers/server_interface/server_interface.hpp>
#include <common/commands/commands.hpp>
#include <client/managers/selections/resources/local_resources_selection.hpp>

namespace como {

class LocalDrawablesSelection : public DrawablesSelection, public LocalResourcesSelection<Drawable>
{
    public:
        /***
         * 1. Construction
         ***/
        LocalDrawablesSelection( ServerInterfacePtr server );
        LocalDrawablesSelection( const LocalDrawablesSelection& ) = delete;
        LocalDrawablesSelection( LocalDrawablesSelection&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~LocalDrawablesSelection() = default;


        /***
         * 3. Setters
         ***/
        virtual void setPivotPointMode( PivotPointMode pivotPointMode );


        /***
         * 4. Drawables management
         ***/
        ResourceID addDrawable( DrawablePtr drawable );


        /***
         * 4. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotate( GLfloat angle, glm::vec3 axis );
        virtual void scale( glm::vec3 scaleFactors );


        /***
         * 6. Operators
         ***/
        LocalDrawablesSelection& operator = ( const LocalDrawablesSelection& ) = delete;
        LocalDrawablesSelection& operator = ( LocalDrawablesSelection&& ) = delete;


    private:
        /***
         * 7. Auxiliar methods
         ***/
        void roundTransformationMagnitude( glm::vec3& v );
        void roundTransformationMagnitude( float& angle, glm::vec3& v );
};

typedef std::shared_ptr< LocalDrawablesSelection > LocalDrawablesSelectionPtr;

} // namespace como

#endif // LOCAL_DRAWABLES_SELECTION_HPP
