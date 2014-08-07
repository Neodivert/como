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

#include <boost/test/unit_test.hpp>
#include <common/packables/packable_integer.hpp>

typedef std::uint8_t PlainInt;
typedef como::PackableUint8< PlainInt > PackableInt;

// TODO: Add tests for PackableUint16 and PackableUint32?
// TODO: How do I test packing / unpacking involving byte flipping.

BOOST_AUTO_TEST_CASE( PackableIntegerConstruction )
{
    const unsigned int N_COPY_TESTS = 50;
    unsigned int i = 0;

    PackableInt zero;
    BOOST_CHECK_EQUAL( zero.getValue(), 0 );

    PackableInt one( 1 );
    BOOST_CHECK_NE( zero.getValue(), one.getValue() );

    PlainInt randomValue;
    for( i=0; i<N_COPY_TESTS; i++ ){
        randomValue = rand()%1000;

        PackableInt randomInteger( randomValue );
        PackableInt randomCopiedInteger( randomValue );
        PackableInt randomMovedInteger( std::move( randomCopiedInteger ) );

        BOOST_CHECK_EQUAL( randomInteger.getValue(), randomCopiedInteger.getValue() );
        BOOST_CHECK_EQUAL( randomInteger.getValue(), randomMovedInteger.getValue() );
        BOOST_CHECK_EQUAL( randomCopiedInteger.getValue(), randomMovedInteger.getValue() );
    }
}


BOOST_AUTO_TEST_CASE( PackableIntegerPacking )
{
    const unsigned int N_PACKING_TESTS = 50;
    const unsigned int N_INTEGERS = 25;
    unsigned int currentTest = 0;
    PlainInt buffer[N_INTEGERS];
    unsigned int i = 0;
    PlainInt* srcBufferPtr = nullptr;
    const PlainInt* dstBufferPtr = nullptr;
    std::array< PackableInt, N_INTEGERS > srcIntegers;
    std::array< PackableInt, N_INTEGERS > dstIntegers;

    for( currentTest = 0; currentTest < N_PACKING_TESTS; currentTest++ ){

        // Start packing an array of integers into a buffer.
        srcBufferPtr = buffer;
        for( auto& currentSrcInteger : srcIntegers ){
            currentSrcInteger = rand()%100 + 1;

            BOOST_CHECK_EQUAL( currentSrcInteger.getPacketSize(), 1 );
            srcBufferPtr = static_cast< PlainInt* >( currentSrcInteger.pack( srcBufferPtr ) );

            // Check that the packet size remains unchanged.
            BOOST_CHECK_EQUAL( currentSrcInteger.getPacketSize(), 1 );
        }

        // Start unpacking integers from previous buffer.
        dstBufferPtr = buffer;
        for( i = 0; i < N_INTEGERS; i++ ){
            dstIntegers[i] = 0;

            BOOST_CHECK_EQUAL( dstIntegers[i].getPacketSize(), 1 );

            // Check that dst and src integers are different before the
            // unpacking. Then unpack dstInteger and check that they
            // are now equal.
            BOOST_CHECK_NE( dstIntegers[i].getValue(), srcIntegers[i].getValue() );
            dstBufferPtr = static_cast< const PlainInt* >( dstIntegers[i].unpack( dstBufferPtr ) );
            BOOST_CHECK_EQUAL( dstIntegers[i].getValue(), srcIntegers[i].getValue() );

            // Check that the packet size remains unchanged.
            BOOST_CHECK_EQUAL( dstIntegers[i].getPacketSize(), 1 );
        }
    }


    PlainInt randomInt;
    for( currentTest = 0; currentTest < N_PACKING_TESTS; currentTest++ ){
        randomInt = rand() % 1000;
        const PackableInt constPackableInt( randomInt );

        BOOST_CHECK_NO_THROW( constPackableInt.unpack( &randomInt ) );

        PlainInt monoBuffer = 1000 + rand() % 1000;
        BOOST_CHECK_THROW( constPackableInt.unpack( &monoBuffer ), std::runtime_error );
    }
}

