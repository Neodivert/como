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

#ifndef SYSTEM_PRIMITIVES_CREATION_MENU_HPP
#define SYSTEM_PRIMITIVES_CREATION_MENU_HPP

#include <QPushButton>
#include <QFrame>
#include <client/managers/managers/primitives/system_primitives_factory.hpp>

namespace como {

class SystemPrimitivesCreationMenu : public QFrame
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        SystemPrimitivesCreationMenu( SystemPrimitivesFactoryPtr geometricPrimitivesFactory );
        SystemPrimitivesCreationMenu() = delete;
        SystemPrimitivesCreationMenu( const SystemPrimitivesCreationMenu& ) = delete;
        SystemPrimitivesCreationMenu( SystemPrimitivesCreationMenu&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~SystemPrimitivesCreationMenu() = default;


        /***
         * 3. Operators
         ***/
        SystemPrimitivesCreationMenu& operator = ( const SystemPrimitivesCreationMenu& ) = delete;
        SystemPrimitivesCreationMenu& operator = ( SystemPrimitivesCreationMenu&& ) = delete;


    private:
        /***
         * 4. Initialization
         ***/
        QPushButton* createCubeCreationButton() const;
        QPushButton* createConeCreationButton() const;
        QPushButton* createCylinderCreationButton() const;
        QPushButton* createSphereCreationButton() const;


        SystemPrimitivesFactoryPtr systemPrimitivesFactory_;
};

} // namespace como

#endif // SYSTEM_PRIMITIVES_CREATION_MENU_HPP
