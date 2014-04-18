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

#ifndef GENERAL_INFO_MENU_HPP
#define GENERAL_INFO_MENU_HPP

#include <QWidget>
#include <QLineEdit>
#include <QLabel>

namespace como {

class GeneralInfoMenu : public QWidget
{
    Q_OBJECT

    private:
        /*! Name of the currently selected 3D object. */
        QLineEdit* objectName_;

        /*! Type of the currently selected 3D object. */
        QLabel* objectType_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        GeneralInfoMenu();

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
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        GeneralInfoMenu& operator = ( const GeneralInfoMenu& ) = delete;

        /*! \brief Move assignment operator */
        GeneralInfoMenu& operator = ( GeneralInfoMenu&& ) = delete;
};

} // namespace como

#endif // GENERAL_INFO_MENU_HPP
