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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef LOCAL_DRAWABLES_SELECTION_HPP
#define LOCAL_DRAWABLES_SELECTION_HPP

#include "drawables_selection.hpp"
#include "../server_interface/server_interface.hpp"
#include "../../../common/commands/commands.hpp"

namespace como {

class LocalDrawablesSelection : public DrawablesSelection, QObject
{
    Q_OBJECT

    private:
        /*!
         * \brief Interface with the server. Used for sending all the
         * transformations made to this selection.
         */
        ServerInterfacePtr server_;

        // TODO: Remove
        UserID localUserID_;

    public:
        /***
         * 1. Construction
         ***/

        LocalDrawablesSelection( UserID localUserID, ServerInterfacePtr server );
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
         * 4. Transformations
         ***/
        virtual void translate( glm::vec3 direction );
        virtual void rotate( GLfloat angle, glm::vec3 axis );
        virtual void scale( glm::vec3 scaleFactors );


        /***
         * 5. Updating
         ***/
        virtual void onChange();


        /***
         * 6. Operators
         ***/
        LocalDrawablesSelection& operator=( const LocalDrawablesSelection& ) = delete ;
        LocalDrawablesSelection& operator=( LocalDrawablesSelection&& ) = delete;


        /***
         * 7. Signals
         ***/
    signals:
        void hasChanged();
};

} // namespace como

#endif // LOCAL_DRAWABLES_SELECTION_HPP