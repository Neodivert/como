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

#include "resource_selector.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourceSelector::ResourceSelector()
{
    // TODO: Duplicated code.

    // Select the first element as the current one.
    if( count() > 0 ){
        setCurrentIndex( 0 );
    }

    // Whenever the current index changes, emit a signal with the associated
    // ResourceID.
    void (QComboBox::*signal)( int ) = &ResourceSelector::activated;
    QObject::connect( this, signal, [this]( int index ){
        emit resourceSelected( resourcesIDs_.at( index ) );
    });
}


ResourceSelector::ResourceSelector( const ResourcesList resourcesList )
{
    resourcesIDs_.reserve( resourcesList.size() );

    for( auto category : resourcesList ){
        insertResource( category.first, category.second );
    }

    // Select the first element as the current one.
    if( count() > 0 ){
        setCurrentIndex( 0 );
    }

    // Whenever the current index changes, emit a signal with the associated
    // ResourceID.
    void (QComboBox::*signal)( int ) = &ResourceSelector::activated;
    QObject::connect( this, signal, [this]( int index ){
        emit resourceSelected( resourcesIDs_.at( index ) );
    });
}


/***
 * 3. Getters
 ***/

ResourceID ResourceSelector::getCurrentResourceID() const
{
    return resourcesIDs_.at( currentIndex() );
}


/***
 * 4. Items management
 ***/

void ResourceSelector::insertResource( ResourceID id, std::string name )
{
    // Save the mapping index -> resource ID.
    resourcesIDs_.push_back( id );

    addItem( name.c_str() );
}

} // namespace como
