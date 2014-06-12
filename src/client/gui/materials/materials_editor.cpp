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
    materialPanel_ = new MaterialPanel();

    layout->addWidget( new QLabel( "Materials Editor" ) );
    layout->addWidget( materialsList_ );
    layout->addWidget( materialPanel_ );

    setLayout( layout );

    QObject::connect( materialsManager_.get(), &MaterialsManager::materialSelectionConfirmed,
                      materialPanel_, &MaterialPanel::openMaterial );

    materialsManager_->addObserver( this, ContainerActionType::ALL );
}


/***
 * 3. ContainerObserver interface
 ***/

void MaterialsEditor::onElementInsertion( MaterialID materialID )
{
    materialsList_->addMaterial( materialID,
                                 materialsManager_->getMaterial( materialID )->getName() );
}

void MaterialsEditor::onElementDeletion( MaterialID materialID )
{
    // TODO: Complete.
    Q_UNUSED( materialID );
}

void MaterialsEditor::onElementModification( MaterialID materialID )
{
    if( materialPanel_->isEnabled() && materialPanel_->getMaterialID() == materialID ){
        materialPanel_->refresh();
    }
}

} // namespace como
