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

#include "resources_dropdown_list.hpp"

namespace como {


/***
 * 1. Construction
 ***/

ResourcesDropdownList::ResourcesDropdownList( SelectableResourcesContainer& resourcesContainer ) :
    resourcesContainer_( &resourcesContainer )
{
    void (QComboBox::*srcSignal)( int ) = &QComboBox::currentIndexChanged;
    QObject::connect( this, srcSignal, [this]( int index ){
        if( index != -1 ){
            std::list< ResourceID >::const_iterator it = indexToID_.begin();
            std::advance( it, index );
            emit resourceSelected( *it );
        }
    });

    // Is it OK to return a "this" pointer from constructor?
    resourcesContainer.addObserver( this );
}


/***
 * 3. Updating (observer pattern)
 ***/

void ResourcesDropdownList::update(ContainerAction lastContainerAction, ResourceID lastElementModifiedID )
{
    switch( lastContainerAction ){
        case ContainerAction::ELEMENT_INSERTION:
            if( resourcesContainer_->isResourceSelectable( lastElementModifiedID ) ){
                addResource( lastElementModifiedID, resourcesContainer_->getResourceName( lastElementModifiedID ) );
            }
        break;
        case ContainerAction::ELEMENT_DELETION:
            removeResource( lastElementModifiedID );
        break;
        default:
            if( resourcesContainer_->isResourceSelectable( lastElementModifiedID ) ){
                addResource( lastElementModifiedID, resourcesContainer_->getResourceName( lastElementModifiedID ) );
            }else{
                removeResource( lastElementModifiedID );
            }
        break;
    }
}


/***
 * 6. Resources management
 ***/

void ResourcesDropdownList::addResource( const ResourceID &id, const std::string &name )
{
    // TODO: Don't insert if resource ID already exists.
    indexToID_.push_back( id );
    addItem( name.c_str() );
}


void ResourcesDropdownList::removeResource( const ResourceID &id )
{
    std::list< ResourceID >::iterator it = indexToID_.begin();
    unsigned int index = 0;

    while( it != indexToID_.end() ){
        if( *it == id  ){
            removeItem( index );
            indexToID_.erase( it );
            return;
        }
        index++;
        it++;
    }
}

} // namespace como
