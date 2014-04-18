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

#ifndef OBJECT_PROPERTIES_TAB_HPP
#define OBJECT_PROPERTIES_TAB_HPP

#include <QFrame>
#include "general_info_menu.hpp"

namespace como {

/*!
 * \class ObjectPropertiesTab
 *
 * \brief Tools menu's tab that allows the user to view and to modify the
 * properties of the current selected 3D object.
 */
class ObjectPropertiesTab : public QFrame
{
    Q_OBJECT

    private:
        /*!
         * Menu for manipulating the general info about the currently selected
         * 3D object.
         */
        GeneralInfoMenu* generalInfoMenu_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        ObjectPropertiesTab();

        /*! \brief Copy constructor */
        ObjectPropertiesTab( const ObjectPropertiesTab& ) = delete;

        /*! \brief Move constructor */
        ObjectPropertiesTab( ObjectPropertiesTab&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~ObjectPropertiesTab() = default;


        /***
         * 3. Operators
         ***/

        /*! \brief Copy assignment operator */
        ObjectPropertiesTab& operator = ( const ObjectPropertiesTab& ) = delete;

        /*! \brief Move assignment operator */
        ObjectPropertiesTab& operator = ( ObjectPropertiesTab&& ) = delete;
};

} // namespace como

#endif // OBJECT_PROPERTIES_TAB_HPP
