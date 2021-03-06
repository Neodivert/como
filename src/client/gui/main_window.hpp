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

#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QMainWindow>
#include <QTypeInfo>
#include "../managers/como_app.hpp"

Q_DECLARE_METATYPE( como::UserConnectionCommandConstPtr )

namespace como {

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
    private:
        // Log
        LogPtr log_;


    public:
        /***
         * 1. Initialization and destruction
         ***/
        MainWindow() = delete;
        MainWindow( const MainWindow& ) = delete;
        MainWindow( MainWindow&& ) = delete;
        explicit MainWindow( QWidget* parent, std::shared_ptr< ComoApp > comoApp );

        virtual ~MainWindow();


        /***
         * 2. Operators
         ***/
        MainWindow& operator = ( const MainWindow& ) = delete;
        MainWindow& operator = ( MainWindow&& ) = delete;
};

} // namespace como

#endif // MAIN_WINDOW_HPP
