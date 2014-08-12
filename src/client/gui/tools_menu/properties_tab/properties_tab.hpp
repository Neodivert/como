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

#ifndef PROPERTIES_TAB_HPP
#define PROPERTIES_TAB_HPP

#include <QFrame>
#include <client/managers/scene/scene.hpp>

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

    public:
        /***
         * 1. Construction
         ***/

        PropertiesTab( ScenePtr scene );

        /*! \brief Default constructor */
        PropertiesTab() = delete;

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
         * 3. Operators
         ***/
        /*! \brief Copy assignment operator */
        PropertiesTab& operator = ( const PropertiesTab& ) = delete;

        /*! \brief Move assignment operator */
        PropertiesTab& operator = ( PropertiesTab&& ) = delete;
};

} // namespace como

#endif // PROPERTIES_TAB_HPP
