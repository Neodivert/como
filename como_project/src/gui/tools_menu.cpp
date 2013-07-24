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

namespace como {

ToolsMenu::ToolsMenu( shared_ptr< ComoApp > comoApp )
{

    QVBoxLayout* layout;
    QLabel* toolsMenuLabel;
    QLabel* appModeLabel;
    QComboBox* appModeSelector;
    QGroupBox* editionScopeGroupBox;
    QVBoxLayout* editionScopeGroupBoxLayout;

    // Share the given pointer to app's state.
    this->comoApp = comoApp;

    // Set tools menu's label.
    toolsMenuLabel = new QLabel( "Tools menu" );

    // Set app mode's label.
    appModeLabel = new QLabel( "App mode" );

    // Set app mode's selector.
    appModeSelector = new QComboBox;
    for( auto appMode : appModeStrings ){
        appModeSelector->addItem( appMode );

    }

    // When user change app mode in selector, call ComoApp::setAppMode().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( appModeSelector, signal, [=]( int index ) {
        comoApp->setAppMode( appModes[index] );
    }  );
    cout << "Signal 1 connected" << endl;

    // When comoApp::setAppMode() be invoked, change appMode selector's index.
    connect( comoApp.get(), &ComoApp::appModeIndexChanged, appModeSelector, &QComboBox::setCurrentIndex );
    cout << "Signal 2 connected" << endl;

    editionScopeGroupBox = new QGroupBox( tr( "Edition scope" ) );
    editionScopeGroupBoxLayout = new QVBoxLayout;
    for( auto editionScope : editionScopeStrings ){
        editionScopeGroupBoxLayout->addWidget( new QRadioButton( tr( (editionScope.second).c_str() ) ) );

        //currentIndexChanged(const QString & text)
    }
    editionScopeGroupBox->setLayout( editionScopeGroupBoxLayout );

    // Set tools panel layout.
    layout = new QVBoxLayout;
    layout->setSpacing( 1 );
    layout->addWidget( toolsMenuLabel );
    layout->addWidget( appModeLabel );
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

} // namespace como
