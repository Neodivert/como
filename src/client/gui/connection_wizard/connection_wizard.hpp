/***
 * Copyright 2013 Moises J. Bonilla Caraballo (Neodivert)
 *
 * This file is part of COMO.
 *
 * COMO is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * COMO is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
***/

#ifndef CONNECTION_WIZARD_HPP
#define CONNECTION_WIZARD_HPP

#include <QWizard>
#include <QWizardPage>
#include "../../managers/scene.hpp"
#include "../../../common/utilities/log.hpp"


namespace como {

class ConnectionWizard : public QWizard
{
    Q_OBJECT

    public:
        /***
         * 1. Initialization and destruction
         ***/
        ConnectionWizard( ScenePtr scene, LogPtr log, QWidget* parent = nullptr );
};

}

#endif // CONNECTION_WIZARD_HPP
