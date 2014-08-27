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

#include "materials_editor.hpp"

namespace como {


MaterialsEditor::MaterialsEditor( MaterialsManagerPtr materialsManager ) :
    QFrame(),
    materialsManager_( materialsManager )
{
    QVBoxLayout* layout = new QVBoxLayout();
    materialsList_ = new MaterialsList( materialsManager );
    materialEditor_ = new MaterialEditor();

    layout->addWidget( new QLabel( "Materials Editor" ) );
    layout->addWidget( materialsList_ );
    layout->addWidget( materialEditor_ );

    setLayout( layout );

    QObject::connect( materialsManager_.get(), &MaterialsManager::materialSelectionConfirmed,
                      materialEditor_, &MaterialEditor::openMaterial );

    materialsManager_->ObservableContainer<ResourceID>::addObserver( this );
}


/***
 * 3. Updating (Observer pattern).
 ***/

void MaterialsEditor::update( ContainerAction lastContainerAction, ResourceID resourceID )
{
    switch( lastContainerAction ){
        case ContainerAction::ELEMENT_INSERTION:
            materialsList_->addMaterial( resourceID,
                                         materialsManager_->getMaterial( resourceID )->getName() );
        break;
        case ContainerAction::ELEMENT_DELETION:
            // TODO: Complete.
        break;
        case ContainerAction::ELEMENT_UPDATE:
            if( materialEditor_->isEnabled() && materialEditor_->getMaterialID() == resourceID ){
                materialEditor_->refresh();
            }
        break;
        case ContainerAction::CONTAINER_UPDATE:
        break;
    }
}

} // namespace como
