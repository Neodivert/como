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

#include <QObject>
#include <client/managers/managers/meshes/meshes_manager.hpp>
#include <QGroupBox>

namespace como {

class MeshInfoMenu : public QWidget, public Observer
{
    Q_OBJECT

    private:
        /*! Local user's (drawables) selection */
        LocalMeshesSelection* userSelection_;

        QGroupBox* displayVertexNormalsGroupBox_;
        QRadioButton* displayVertexNormalsAlways_;
        QRadioButton* displayVertexNormalsNever_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        MeshInfoMenu( LocalMeshesSelection* userSelection );

        /*! \brief Copy constructor */
        MeshInfoMenu( const MeshInfoMenu& ) = delete;

        /*! \brief Move constructor */
        MeshInfoMenu( MeshInfoMenu&& ) = delete;


        /***
         * 2. Destruction
         ***/

        /*! \brief Destructor */
        ~MeshInfoMenu() = default;


        /***
         * 3. Initialization
         ***/
        QGroupBox* createVertexNormalsDisplayGroupBox( LocalMeshesSelection* meshesSelection );


        /***
         * 4. Updating
         ***/

        virtual void update();


        /***
         * 5. Operators
         ***/

        /*! \brief Copy assignment operator */
        MeshInfoMenu& operator = ( const MeshInfoMenu& ) = delete;

        /*! \brief Move assignment operator */
        MeshInfoMenu& operator = ( MeshInfoMenu&& ) = delete;
};

} // namespace como

#endif // MESH_INFO_MENU_HPP
