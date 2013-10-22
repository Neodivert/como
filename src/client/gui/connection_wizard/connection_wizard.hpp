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
#include "../../models/server/server_interface.hpp"

namespace como {

class ConnectionWizard : public QWizard
{
    Q_OBJECT

    public:
        enum { PAGE_INTRO, PAGE_CONNECT_SERVER /*Page_Intro, Page_Evaluate, Page_Register, Page_Details,
                Page_Conclusion*/ };

        /***
         * 1. Initialization and destruction
         ***/
        ConnectionWizard( std::shared_ptr< ServerInterface > serverInterface );
    private:
        QWizardPage* createIntroPage() const ;
};

}

#endif // CONNECTION_WIZARD_HPP
