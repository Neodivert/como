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

#ifndef MESH_INFO_MENU_HPP
#define MESH_INFO_MENU_HPP

#include <QCheckBox>
#include <client/managers/selections/meshes/local_meshes_selection.hpp>

namespace como {

class MeshInfoMenu : public QWidget, public Observer
{
    Q_OBJECT

    public:
        /***
         * 1. Construction
         ***/
        MeshInfoMenu( LocalMeshesSelection* userSelection );
        MeshInfoMenu( const MeshInfoMenu& ) = delete;
        MeshInfoMenu( MeshInfoMenu&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~MeshInfoMenu() = default;


        /***
         * 3. Updating (observer pattern)
         ***/
        virtual void update();


        /***
         * 4. Operators
         ***/
        MeshInfoMenu& operator = ( const MeshInfoMenu& ) = delete;
        MeshInfoMenu& operator = ( MeshInfoMenu&& ) = delete;


    private:
        /***
         * 5. Initialization
         ***/
        static QCheckBox* createVertexNormalsDisplayCheckBox( LocalMeshesSelection* meshesSelection );


        /*** Attributes ***/
        LocalMeshesSelection* userSelection_;

        // Child widgets
        QCheckBox* displayVertexNormalsCheckBox_;
};

} // namespace como

#endif // MESH_INFO_MENU_HPP
