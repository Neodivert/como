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

#ifndef GENERAL_INFO_MENU_HPP
#define GENERAL_INFO_MENU_HPP

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <client/managers/selections/entities/local_entities_selection.hpp>

namespace como {

class GeneralInfoMenu : public QWidget, public Observer
{
    Q_OBJECT

    private:
        /*! Local user's (entities) selection */
        LocalEntitiesSelection* userSelection_;

        QLabel* centroidPosition_;

        /*! Name of the currently selected 3D object. */
        QLineEdit* objectName_;

        /*! Type of the currently selected 3D object. */
        QLabel* objectType_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        GeneralInfoMenu( LocalEntitiesSelection* userSelection );

        /*! \brief Copy constructor */
        GeneralInfoMenu( const GeneralInfoMenu& ) = delete;

        /*! \brief Move constructor */
        GeneralInfoMenu( GeneralInfoMenu&& ) = delete;


        /***
         * 2. Initialization
         ***/

    public:


        /***
         * 3. Destruction
         ***/

        /*! \brief Destructor */
        ~GeneralInfoMenu() = default;


        /***
         * 4. Updating (Observer pattern)
         ***/

        void update();


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        GeneralInfoMenu& operator = ( const GeneralInfoMenu& ) = delete;

        /*! \brief Move assignment operator */
        GeneralInfoMenu& operator = ( GeneralInfoMenu&& ) = delete;
};

} // namespace como

#endif // GENERAL_INFO_MENU_HPP
