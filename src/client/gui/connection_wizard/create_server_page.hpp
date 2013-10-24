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

#ifndef CREATE_SERVER_PAGE_HPP
#define CREATE_SERVER_PAGE_HPP

#include "../../models/server/server_interface.hpp"
#include <QWizardPage>
#include <QLineEdit>
#include <QLabel>
#include <memory>

const char SERVER_PATH[] = "../server/server";

namespace como {

class CreateServerPage : public QWizardPage
{
    Q_OBJECT

    private:
        std::shared_ptr< ServerInterface > serverInterface_;
        QLineEdit* portInput_;
        QLineEdit* maxUsersInput_;
        QLineEdit* userNameInput_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        CreateServerPage( std::shared_ptr< ServerInterface > serverInterface );


        /***
         * 2. Validators
         ***/
    protected:
        virtual bool validatePage();


        /***
         * 3. Auxiliar methods
         ***/
    public:
        virtual int nextId() const;
};

} // namespace como

#endif // CREATE_SERVER_PAGE_HPP
