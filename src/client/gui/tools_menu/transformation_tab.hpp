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

#ifndef TRANSFORMATION_TAB_HPP
#define TRANSFORMATION_TAB_HPP

#include <QFrame>
#include <client/managers/como_app.hpp>

namespace como {

/*!
 * \class TransformationTab
 *
 * \brief Tools menu's tab with options for transformating selections of
 * drawables (translations, rotations and scales).
 */
class TransformationTab : public QFrame
{
    private:
        ComoAppPtr comoApp_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        TransformationTab( ComoAppPtr comoApp );

        /*! \brief Copy constructor */
        TransformationTab( const TransformationTab& ) = delete;

        /*! \brief Move constructor */
        TransformationTab( TransformationTab&& ) = delete;


        /***
         * 2. Initialization
         ***/
    private:
        QGroupBox* createTransformationLockMenu();
        QGroupBox* createPivotPointModeSelector();
    public:


        /***
         * 3. Destruction
         ***/

        /*! \brief Destructor */
        ~TransformationTab() = default;


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        TransformationTab& operator = ( const TransformationTab& ) = delete;

        /*! \brief Move assignment operator */
        TransformationTab& operator = ( TransformationTab&& ) = delete;
};

} // namespace como

#endif // TRANSFORMATION_TAB_HPP
