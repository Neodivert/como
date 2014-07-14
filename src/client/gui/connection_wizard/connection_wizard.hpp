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

#ifndef CONNECTION_WIZARD_HPP
#define CONNECTION_WIZARD_HPP

#include <QWizard>
#include <QWizardPage>

#include <client/managers/scene/scene.hpp>
#include <common/utilities/log.hpp>


namespace como {

class ConnectionWizard : public QWizard
{
    Q_OBJECT

    private:
        ScenePtr scene_;

    public:
        /***
         * 1. Construction
         ***/
        ConnectionWizard() = delete ;
        ConnectionWizard( const ConnectionWizard& ) = delete;
        ConnectionWizard( ConnectionWizard&& ) = delete;
        ConnectionWizard( LogPtr log, QWidget* parent = nullptr );


        /***
         * 2. Destruction
         ***/
        ~ConnectionWizard() = default;


        /***
         * 3. Main method
         ***/
        ScenePtr run();


        /***
         * 4. Operators
         ***/
        ConnectionWizard& operator = ( const ConnectionWizard& ) = delete;
        ConnectionWizard& operator = ( ConnectionWizard&& ) = delete;
};

}

#endif // CONNECTION_WIZARD_HPP
