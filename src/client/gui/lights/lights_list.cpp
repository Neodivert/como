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

#include "lights_list.hpp"


namespace como {

/***
 * 1. Construction
 ***/

LightsList::LightsList( DrawablesManagerPtr drawablesManager ) :
    drawablesManager_( drawablesManager )
{
    /*
    QObject::connect( materialsManager.get(), &MaterialsManager::materialCreated,
                      this, &LightsList::addMaterial );

    QObject::connect( this, &LightsList::itemActivated, [=]( QListWidgetItem * item ){
        emit materialSelected( ( dynamic_cast< LightsListItem* >( item ) )->getMaterialID() );
    });

    QObject::connect( this, &LightsList::itemClicked, [=]( QListWidgetItem * item ){
        emit materialSelected( ( dynamic_cast< LightsListItem* >( item ) )->getMaterialID() );
    });

    QObject::connect( this, &LightsList::materialSelected, [=]( MaterialID materialID ){
        materialsManager->selectMaterial( materialID );
    });
    */
}


/***
 * 4. Slots
 ***/

void LightsList::addLight( const LightID& id, const std::string& name )
{
    LightsListItem* newListItem = new LightsListItem( id, name );

    insertItem( count(), newListItem );
}

} // namespace como
