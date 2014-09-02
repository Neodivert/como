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

#ifndef CONDITIONAL_PACKABLE_WRAPPER_HPP
#define CONDITIONAL_PACKABLE_WRAPPER_HPP

#include <common/packables/packable.hpp>
#include <functional>
#include <memory>

namespace como {

typedef std::function< bool( void ) > ConditionPredicate;

template< class InnerPackable >
class ConditionalPackableWrapper : public Packable {
    private:
        std::unique_ptr< InnerPackable > innerPackable_;
        ConditionPredicate predicate_;

    public:
        /***
         * 1. Construction
         ***/
        ConditionalPackableWrapper() = delete;
        ConditionalPackableWrapper( std::unique_ptr< InnerPackable > innerPackable, ConditionPredicate predicate );
        ConditionalPackableWrapper( const ConditionalPackableWrapper<InnerPackable>& ) = delete;
        ConditionalPackableWrapper( ConditionalPackableWrapper<InnerPackable>&& ) = delete;


        /***
         * 2. Destruction
         ***/
        virtual ~ConditionalPackableWrapper() = default;


        /***
         * 3. Packing and unpacking
         ***/
        virtual void* pack( void *buffer ) const;
        virtual const void* unpack( const void *buffer );
        virtual const void* unpack( const void *buffer ) const;


        /***
         * 4. Getters
         ***/
        virtual PacketSize getPacketSize() const;
        const InnerPackable* getInnerPackable() const;


        /***
         * 5. Operators
         ***/
        ConditionalPackableWrapper<InnerPackable>& operator = ( const ConditionalPackableWrapper<InnerPackable>& ) = delete;
        ConditionalPackableWrapper<InnerPackable>& operator = ( ConditionalPackableWrapper<InnerPackable>&& ) = delete;
};


/***
 * 1. Construction
 ***/

template< class InnerPackable >
ConditionalPackableWrapper<InnerPackable>::ConditionalPackableWrapper( std::unique_ptr<InnerPackable> innerPackable, ConditionPredicate predicate ) :
    innerPackable_( std::move( innerPackable ) ),
    predicate_( predicate )
{}


/***
 * 3. Packing and unpacking
 ***/

template< class InnerPackable >
void *ConditionalPackableWrapper<InnerPackable>::pack( void *buffer ) const
{
    if( !predicate_ || predicate_() ){
        return innerPackable_->pack( buffer );
    }else{
        return buffer;
    }
}

template< class InnerPackable >
const void *ConditionalPackableWrapper<InnerPackable>::unpack( const void *buffer )
{
    if( !predicate_ || predicate_() ){
        return innerPackable_->unpack( buffer );
    }else{
        return buffer;
    }
}


template< class InnerPackable >
const void *ConditionalPackableWrapper<InnerPackable>::unpack( const void *buffer ) const
{
    if( !predicate_ || predicate_() ){
        return innerPackable_->unpack( buffer );
    }else{
        return buffer;
    }
}


/***
 * 4. Getters
 ***/

template< class InnerPackable >
PacketSize ConditionalPackableWrapper<InnerPackable>::getPacketSize() const
{
    if( !predicate_ || predicate_() ){
        return innerPackable_->getPacketSize();
    }else{
        return 0;
    }
}

template< class InnerPackable >
const InnerPackable* ConditionalPackableWrapper<InnerPackable>::getInnerPackable() const
{
    if( !predicate_ || predicate_() ){
        return innerPackable_.get();
    }else{
        return nullptr;
    }
}

} // namespace como

#endif // CONDITIONAL_PACKABLE_WRAPPER_HPP
