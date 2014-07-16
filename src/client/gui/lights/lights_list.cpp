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

LightsList::LightsList( LightsManagerPtr lightsManager ) :
    lightsManager_( lightsManager )
{
    QObject::connect( lightsManager.get(), &LightsManager::lightCreated,
                      this, &LightsList::addLight );

    QObject::connect( this, &LightsList::itemActivated, [=]( QListWidgetItem * item ){
        emit lightSelected( ( dynamic_cast< LightsListItem* >( item ) )->getLightID() );
    });

    QObject::connect( this, &LightsList::itemClicked, [=]( QListWidgetItem * item ){
        emit lightSelected( ( dynamic_cast< LightsListItem* >( item ) )->getLightID() );
    });

    QObject::connect( this, &LightsList::lightSelected, [=]( LightID lightID ){
        lightsManager_->selectLight( lightID );
        lightsManager_->highlightLight( lightID );
    });

    QObject::connect( lightsManager_.get(), &LightsManager::lightRemoved, this, &LightsList::removeLight );
}


/***
 * 4. Slots
 ***/

void LightsList::addLight( LightID id, std::string name )
{
    LightsListItem* newListItem = new LightsListItem( id, name );

    insertItem( count(), newListItem );
}


void LightsList::removeLight( PackableDrawableID id )
{
    QListWidgetItem* uncastedListItem;
    LightsListItem* castedListItem;
    int i;

    for( i=0; i<count(); i++ ){
        uncastedListItem = item( i );
        if( uncastedListItem ){
            castedListItem = dynamic_cast< LightsListItem * >( uncastedListItem );
            if( castedListItem->getLightID() == id ){
                takeItem( i );
            }
        }
    }
}

} // namespace como
