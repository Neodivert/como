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

#ifndef MENU_BAR_HPP
#define MENU_BAR_HPP

#include <QMenuBar>
#include <client/managers/managers/entities/entities_manager.hpp>
#include <client/managers/managers/meshes/meshes_manager.hpp>

namespace como {

class MenuBar : public QMenuBar, public Observer
{
    Q_OBJECT


    public:
        /***
         * 1. Construction
         ***/
        MenuBar() = delete;
        MenuBar( EntitiesManagerPtr entitiesManager );
        MenuBar( const MenuBar& ) = delete;
        MenuBar( MenuBar&& ) = delete;


        /***
         * 2. Destruction
         ***/
        ~MenuBar() = default;


        /***
         * 3. Updating (pattern Observer)
         ***/
        virtual void update();


        /***
         * 4. Operators
         ***/
        MenuBar& operator = ( const MenuBar& ) = delete;
        MenuBar& operator = ( MenuBar&& ) = delete;


    private:
        /***
         * 5. Initialization
         ***/
        QMenu* createViewMenu( EntitiesManager* entitiesManager );
        QMenu* createDisplayEdgesMenu( MeshesManager* meshesManager );
        QMenu* createDisplayVertexNormalsMenu( MeshesManager* meshesManager );


    private:
        EntitiesManagerPtr entitiesManager_;

        QAction* displayVertexNormalsAlways_;
        QAction* displayVertexNormalsNever_;
};

} // namespace como

#endif // MENU_BAR_HPP
