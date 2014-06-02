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

#ifndef INTRO_PAGE_HPP
#define INTRO_PAGE_HPP

#include <QLabel>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QLineEdit>
#include <QFormLayout>
#include <QRegExpValidator>
#include "connect_server_page.hpp"

namespace como {

class IntroPage : public QWizardPage
{
    private:
        QRadioButton* createServerRadioButton_;
        QRadioButton* connectToServerRadioButton_;

    public:
        /***
         * 1. Initialization and destruction
         ***/
        IntroPage();
        IntroPage( const IntroPage& ) = delete;
        IntroPage( IntroPage&& ) = delete;

        ~IntroPage() = default;


        /***
         * 2. Auxiliar methods
         ***/
        virtual int nextId() const;


        /***
         * 4. Operators
         ***/
        IntroPage& operator = ( const IntroPage& ) = delete;
        IntroPage& operator = ( IntroPage&& ) = delete;
};

} // namespace como

#endif // INTRO_PAGE_HPP
