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

#include <QVBoxLayout>
#include <QLabel>
#include "system_primitives_creation_menu.hpp"

namespace como {

/***
 * 1. Construction
 ***/

SystemPrimitivesCreationMenu::SystemPrimitivesCreationMenu( SystemPrimitivesFactoryPtr systemPrimitivesFactory ) :
    systemPrimitivesFactory_( systemPrimitivesFactory )
{
    QVBoxLayout* layout = new QVBoxLayout;

    layout->addWidget( new QLabel( "Geometry creation" ) );
    layout->addWidget( createCubeCreationButton() );
    layout->addWidget( createConeCreationButton() );
    layout->addWidget( createCylinderCreationButton() );
    layout->addWidget( createSphereCreationButton() );

    setLayout( layout );
}


/***
 * 4. Initialization
 ***/

QPushButton* SystemPrimitivesCreationMenu::createCubeCreationButton() const
{
    QPushButton* cubeCreationButton = new QPushButton( "Create cube" );

    QObject::connect( cubeCreationButton, &QPushButton::pressed, [this](){
        systemPrimitivesFactory_->createCube();
    });

    return cubeCreationButton;
}


QPushButton *SystemPrimitivesCreationMenu::createConeCreationButton() const
{
    QPushButton* coneCreationButton = new QPushButton( "Create cone" );

    QObject::connect( coneCreationButton, &QPushButton::pressed, [this](){
        systemPrimitivesFactory_->createCone();
    });

    return coneCreationButton;
}


QPushButton *SystemPrimitivesCreationMenu::createCylinderCreationButton() const
{
    QPushButton* cylinderCreationButton = new QPushButton( "Create cylinder" );

    QObject::connect( cylinderCreationButton, &QPushButton::pressed, [this](){
        systemPrimitivesFactory_->createCylinder();
    });

    return cylinderCreationButton;
}


QPushButton *SystemPrimitivesCreationMenu::createSphereCreationButton() const
{
    QPushButton* sphereCreationButton = new QPushButton( "Create sphere" );

    QObject::connect( sphereCreationButton, &QPushButton::pressed, [this](){
        systemPrimitivesFactory_->createSphere();
    });

    return sphereCreationButton;
}



} // namespace como
