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

#include "connection_wizard.hpp"
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QRegExpValidator>

#include "page_ids.hpp"
#include "intro_page.hpp"
#include "connect_server_page.hpp"
#include "create_server_page.hpp"

namespace como {

/***
 * 1. Initialization and destruction
 ***/

ConnectionWizard::ConnectionWizard( LogPtr log, QWidget* parent ) :
    QWizard( parent ),
    scene_( nullptr )
{   
    setPage( PAGE_INTRO, new IntroPage );
    setPage( PAGE_CONNECT_SERVER, new ConnectServerPage( scene_, log ) );
    setPage( PAGE_CREATE_SERVER, new CreateServerPage( scene_, log ) );

    setStartId( PAGE_INTRO );
}


/***
 * 3. Main method
 ***/

ScenePtr ConnectionWizard::run()
{
    exec();

    return scene_;
}

}
