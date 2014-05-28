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
 * along with COMO.  If not, see <http://www.gnu.org/licenses/>.
***/

#include "materials_list.hpp"

namespace como {


/***
 * 1. Construction
 ***/

MaterialsList::MaterialsList( MaterialsManagerPtr materialsManager )
{
    QObject::connect( materialsManager.get(), &MaterialsManager::materialCreated,
                      this, &MaterialsList::addMaterial );

    QObject::connect( this, &MaterialsList::itemActivated, [=]( QListWidgetItem * item ){
        emit materialSelected( ( dynamic_cast< MaterialsListItem* >( item ) )->getMaterialID() );
    });

    QObject::connect( this, &MaterialsList::itemClicked, [=]( QListWidgetItem * item ){
        emit materialSelected( ( dynamic_cast< MaterialsListItem* >( item ) )->getMaterialID() );
    });

    QObject::connect( this, &MaterialsList::materialSelected, [=]( MaterialID materialID ){
        materialsManager->selectMaterial( materialID );
    });
}


/***
 * 4. Slots
 ***/

void MaterialsList::addMaterial( const MaterialID& id, const std::string& name )
{
    MaterialsListItem* newListItem = new MaterialsListItem( id, name );

    insertItem( count(), newListItem );
}

} // namespace como
