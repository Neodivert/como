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

#include <QVBoxLayout>
#include <QLabel>
#include "materials_editor.hpp"

namespace como {


MaterialsEditor::MaterialsEditor( MaterialsManagerPtr materialsManager, LocalMeshesSelection* localMeshesSelection ) :
    QFrame(),
    materialsManager_( materialsManager ),
    localMeshesSelection_( localMeshesSelection )
{
    QVBoxLayout* layout = new QVBoxLayout;
    materialsList_ = new MaterialsListWidget( materialsManager );
    materialEditor_ = new MaterialEditor;

    layout->addWidget( new QLabel( "Select material: " ) );
    layout->addWidget( materialsList_ );
    layout->addWidget( materialEditor_ );
    setLayout( layout );

    QObject::connect( materialsList_, &MaterialsListWidget::materialSelected,
                      materialEditor_, &MaterialEditor::openMaterial );

    materialsManager_->ObservableContainer<ResourceID>::addObserver( this );
    setEnabled( false );
}


/***
 * 3. Updating (Observer pattern).
 ***/

void MaterialsEditor::update( ContainerAction lastContainerAction, ResourceID resourceID )
{
    if( localMeshesSelection_->size() ){
        setEnabled( true );

        (void)( lastContainerAction );
        (void)( resourceID );

        materialsList_->populate();

    }else{
        setEnabled( false );
    }
}

} // namespace como
