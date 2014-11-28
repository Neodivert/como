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

#ifndef CONNECT_SERVER_PAGE_HPP
#define CONNECT_SERVER_PAGE_HPP

#include <QWizardPage>
#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QRegExpValidator>
#include <QMessageBox>

#include <client/managers/scene/scene.hpp>

namespace como {

class ConnectServerPage : public QWizardPage
{
    public:
        /***
         * 1. Construction
         ***/
        ConnectServerPage() = delete ;
        ConnectServerPage( const ConnectServerPage& ) = delete;
        ConnectServerPage( ConnectServerPage&& ) = delete;
        ConnectServerPage( ScenePtr& scene, LogPtr log );


        /***
         * 2. Destruction
         ***/
        ~ConnectServerPage() = default;


        /***
         * 4. Getters
         ***/
        virtual int nextId() const ;


        /***
         * 5. Operators
         ***/
        ConnectServerPage& operator = ( const ConnectServerPage& ) = delete;
        ConnectServerPage& operator = ( ConnectServerPage&& ) = delete;


    protected:
        /***
         * 6. Validators
         ***/
        virtual bool validatePage();


    private:
        ScenePtr& scene_;
        QLineEdit* ipInput_;
        QLineEdit* portInput_;
        QLineEdit* userNameInput_;

        LogPtr log_;
};

} // namespace como

#endif // CONNECT_SERVER_PAGE_HPP
