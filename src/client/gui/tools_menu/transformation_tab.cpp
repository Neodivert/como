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

#include <QComboBox>
#include <QFormLayout>
#include "transformation_tab.hpp"


namespace como {

TransformationTab::TransformationTab( ComoAppPtr comoApp ) :
    comoApp_( comoApp )
{
    // Set this widget's layout.
    QFormLayout* layout = new QFormLayout;
    layout->setRowWrapPolicy( QFormLayout::WrapAllRows );
    layout->addRow( "Select pivot point mode:",
                    createPivotPointModeSelector( comoApp->getScene()->getEntitiesManager()->getLocalSelection() ) );
    setLayout( layout );
}


/***
 * 4. Initialization
 ***/

QComboBox* TransformationTab::createPivotPointModeSelector( LocalEntitiesSelection* localEntitiesSelection )
{
    // Available pivot point modes (strings for GUI output).
    const unsigned int N_PIVOT_POINT_MODES = 3;
    const char pivotPointModeStrings[N_PIVOT_POINT_MODES][32] =
    {
         "Selection's centroid",
         "Individual centroids",
         "World origin"
    };

    // Create a QComboBox ("selector") with all possible pivot point modes.
    QComboBox* pivotPointModeSelector = new QComboBox;
    for( const auto& pivotPointModeString : pivotPointModeStrings ){
        pivotPointModeSelector->addItem( tr( pivotPointModeString ) );
    }

    // When user change pivot point mode in selector, call ComoApp::setPivotPoint().
    void (QComboBox::*signal)( int ) = &QComboBox::activated;
    connect( pivotPointModeSelector, signal, [=]( int index ){
        localEntitiesSelection->setPivotPointMode( static_cast< PivotPointMode >( index ) );
    });

    // Return the created pivot point mode selector.
    return pivotPointModeSelector;
}

} // namespace como
