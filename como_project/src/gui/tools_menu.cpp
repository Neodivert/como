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

#include "tools_menu.hpp"
#include <QVBoxLayout>

ToolsMenu::ToolsMenu( shared_ptr< ComoApp > comoApp )
{
    QGridLayout* layout;
    QLabel* appModeLabel;
    QComboBox* appModeSelector;
    QGroupBox* editionScopeGroupBox;
    QVBoxLayout* editionScopeGroupBoxLayout;


    // Share the given pointer to app's state.
    this->comoApp = comoApp;

    // Set app mode's label.
    appModeLabel = new QLabel( "App mode" );

    // Set app mode's selector.
    appModeSelector = new QComboBox;
    for( int i = 0; i < N_APP_MODES; i++ ){
        appModeSelector->addItem( QString( appModeStrings[i] ) );
    }

    editionScopeGroupBox = new QGroupBox( tr( "Edition scope" ) );
    editionScopeGroupBoxLayout = new QVBoxLayout;
    for( int i = 0; i < N_EDITION_SCOPES; i++ ){
        editionScopeGroupBoxLayout->addWidget( new QRadioButton( tr( editionScopeStrings[i] ) ) );
    }
    editionScopeGroupBox->setLayout( editionScopeGroupBoxLayout );


    // Set tools panel layout.
    layout = new QGridLayout;
    layout->setSpacing( 1 );
    layout->addWidget( new QLabel( "Tools menu" ) );
    layout->addWidget( new QLabel( "App mode" ) );
    layout->addWidget( appModeSelector );
    layout->addWidget( editionScopeGroupBox );
    setLayout( layout );


    /*
    QGroupBox *groupBox = new QGroupBox(tr("Exclusive Radio Buttons"));

         QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
         QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
         QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));

    QGroupBox *groupBox = new QGroupBox(tr("Exclusive Radio Buttons"));

      QRadioButton *radio1 = new QRadioButton(tr("&Radio button 1"));
      QRadioButton *radio2 = new QRadioButton(tr("R&adio button 2"));
      QRadioButton *radio3 = new QRadioButton(tr("Ra&dio button 3"));

      radio1->setChecked(true);

      QVBoxLayout *vbox = new QVBoxLayout;
      vbox->addWidget(radio1);
      vbox->addWidget(radio2);
      vbox->addWidget(radio3);
      vbox->addStretch(1);
      groupBox->setLayout(vbox);
      */
}
