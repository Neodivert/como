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

#ifndef GEOMETRY_PRIMITIVES_CREATION_MENU_HPP
#define GEOMETRY_PRIMITIVES_CREATION_MENU_HPP

#include <QFrame>
#include <client/managers/managers/primitives/geometry_primitives_factory.hpp>

namespace como {

class GeometricPrimitivesCreationMenu : public QFrame
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        GeometricPrimitivesCreationMenu( GeometricPrimitivesFactoryPtr geometricPrimitivesFactory );
        GeometricPrimitivesCreationMenu() = delete;
        GeometricPrimitivesCreationMenu( const GeometricPrimitivesCreationMenu& ) = delete;
        GeometricPrimitivesCreationMenu( GeometricPrimitivesCreationMenu&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~GeometricPrimitivesCreationMenu() = default;


        /***
         * 3. Operators
         ***/
        GeometricPrimitivesCreationMenu& operator = ( const GeometricPrimitivesCreationMenu& ) = delete;
        GeometricPrimitivesCreationMenu& operator = ( GeometricPrimitivesCreationMenu&& ) = delete;


    private:
        /***
         * 4. Initialization
         ***/
        QPushButton* createCubeCreationButton() const;
        QPushButton* createConeCreationButton() const;
        QPushButton* createCylinderCreationButton() const;


        GeometricPrimitivesFactoryPtr geometricPrimitivesFactory_;
};

} // namespace como

#endif // GEOMETRY_PRIMITIVES_CREATION_MENU_HPP
