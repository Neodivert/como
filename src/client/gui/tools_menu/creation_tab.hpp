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

#ifndef CREATION_TAB_HPP
#define CREATION_TAB_HPP

#include <QFrame>
#include <client/gui/primitives/primitive_import_dialog.hpp>
#include <client/gui/tools_menu/geometric_primitives_creation_menu.hpp>
#include <client/managers/scene/scene.hpp>

namespace como {

/*!
 * \class CreationTab
 *
 * \brief Tab widget for tools panel including options for adding new elements
 * (meshes, lights, etc) to the shared scene.
 */
class CreationTab : public QFrame
{
    Q_OBJECT

    private:
        /*! Pointer to the scene we will add elements to */
        ScenePtr scene_;

    public:
        /***
         * 1. Construction
         ***/

        /*! \brief Default constructor */
        CreationTab( ScenePtr scene );

        /*! \brief Copy constructor */
        CreationTab( const CreationTab& ) = delete;

        /*! \brief Move constructor */
        CreationTab( CreationTab&& ) = delete;


        /***
         * 2. Initialization
         ***/
    private:
        QFrame* createMeshFromPrimitiveCreationMenu();
        QPushButton* createPrimitiveImportButton() const;
        QFrame* createLightsCreationPanel() const;
    public:


        /***
         * 3. Destruction
         ***/

        /*! \brief Destructor */
        ~CreationTab() = default;


        /***
         * 4. Operators
         ***/

        /*! \brief Copy assignment operator */
        CreationTab& operator = ( const CreationTab& ) = delete;

        /*! \brief Move assignment operator */
        CreationTab& operator = ( CreationTab&& ) = delete;


        /***
         * 5. Signals
         ***/
    signals:
        void meshColorChanged( QColor newColor );
};

} // namespace como

#endif // CREATION_TAB_HPP
