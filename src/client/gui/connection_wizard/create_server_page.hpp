/***
 * Copyright 2013, 2014 Moises J. Bonilla Caraballo (Neodivert)
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

#include "../../managers/scene/scene.hpp"
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
        ScenePtr scene_;
        QLineEdit* portInput_;
        QSpinBox* maxUsersInput_;
        QLineEdit* userNameInput_;

        // Log
        LogPtr log_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        CreateServerPage() = delete;
        CreateServerPage( const CreateServerPage& ) = delete;
        CreateServerPage( CreateServerPage&& ) = delete;
        CreateServerPage( ScenePtr scene, LogPtr log );

        ~CreateServerPage() = default;


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


        /***
         * 4. Operators
         ***/
        CreateServerPage& operator = ( const CreateServerPage& ) = delete;
        CreateServerPage& operator = ( CreateServerPage&& ) = delete;
};

} // namespace como

#endif // CREATE_SERVER_PAGE_HPP
