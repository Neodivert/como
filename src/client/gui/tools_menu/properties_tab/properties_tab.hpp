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

#ifndef PROPERTIES_TAB_HPP
#define PROPERTIES_TAB_HPP

#include <QFrame>
#include "general_info_menu.hpp"
#include "mesh_info_menu.hpp"
#include "../../../models/3d/drawables_selection.hpp"

namespace como {

/*!
 * \class PropertiesTab
 *
 * \brief Tools menu's tab that allows the user to view and to modify the
 * properties of the current selected 3D object.
 */
class PropertiesTab : public QFrame
{
    Q_OBJECT

    private:
        /*! Local user's (drawables) selection. */
        DrawablesSelectionPtr userSelection_;

        /*!
         * Menu for manipulating the general info about the currently selected
         * 3D object.
         */
        GeneralInfoMenu* generalInfoMenu_;

        /*!
         * Menu for manipulating the properties of the mesh(es) selected by
         * user.
         */
        MeshInfoMenu* meshInfoMenu_;


    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        PropertiesTab( DrawablesSelectionPtr userSelection );

        /*! \brief Copy constructor */
        PropertiesTab( const PropertiesTab& ) = delete;

        /*! \brief Move constructor */
        PropertiesTab( PropertiesTab&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~PropertiesTab() = default;


        /***
         * 3. Refreshing
         ***/

        /*! \brief Refresh the info about the user's selection properties */
    public slots:
        void refresh();


        /***
         * 4. Operators
         ***/
    public:
        /*! \brief Copy assignment operator */
        PropertiesTab& operator = ( const PropertiesTab& ) = delete;

        /*! \brief Move assignment operator */
        PropertiesTab& operator = ( PropertiesTab&& ) = delete;
};

} // namespace como

#endif // PROPERTIES_TAB_HPP
