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

#ifndef CONNECT_SERVER_PAGE_HPP
#define CONNECT_SERVER_PAGE_HPP

#include <QWizardPage>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QRegExpValidator>
#include "../../managers/scene.hpp"

namespace como {

class ConnectServerPage : public QWizardPage
{
    private:
        ScenePtr scene_;
        QLineEdit* ipInput_;
        QLineEdit* portInput_;
        QLineEdit* userNameInput_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        ConnectServerPage( ScenePtr scene );


        /***
         * 2. Validators
         ***/
    protected:
        virtual bool validatePage();


        /***
         * 3. Auxiliar methods
         ***/
    public:
        virtual int nextId() const ;
};

} // namespace como

#endif // CONNECT_SERVER_PAGE_HPP
