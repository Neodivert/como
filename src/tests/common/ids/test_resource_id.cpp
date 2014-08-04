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

#include <tests/common/tests_header.hpp>
#include <common/ids/resource_id.hpp>

BOOST_AUTO_TEST_CASE( ResourceID_Getters )
{
    como::ResourceID nullID;
    BOOST_CHECK_EQUAL( nullID.getCreatorID(), 0 );
    BOOST_CHECK_EQUAL( nullID.getResourceIndex(), 0 );

    unsigned int nTests = 0;
    for( nTests = 0; nTests < 10; nTests++ ){
        como::UserID userID = rand()%50;
        como::ResourceIndex resourceIndex = rand()%50;
        como::ResourceID randomID( userID, resourceIndex );

        BOOST_CHECK_EQUAL( randomID.getCreatorID(), userID );
        BOOST_CHECK_EQUAL( randomID.getResourceIndex(), resourceIndex );
    }

    como::ResourceID originalID;
    como::ResourceID copiedID( originalID );
    como::ResourceID movedID( std::move( originalID ) );
    BOOST_CHECK_EQUAL( originalID, copiedID );
    BOOST_CHECK_EQUAL( originalID, copiedID );
    BOOST_CHECK_EQUAL( copiedID, movedID );
}


BOOST_AUTO_TEST_CASE( ResourceID_Comparisons )
{
    como::ResourceID resourceID_A1( 0, 1 );
    como::ResourceID resourceID_A2( 0, 1 );
    como::ResourceID resourceID_B( 1, 0 );

    BOOST_CHECK( resourceID_A1 == resourceID_A1 );
    BOOST_CHECK( resourceID_A1 == resourceID_A2 );

    BOOST_CHECK( !( resourceID_A1 == resourceID_B ) );
    BOOST_CHECK( !( resourceID_A2 == resourceID_B ) );

    BOOST_CHECK( resourceID_A1 < resourceID_B );
    BOOST_CHECK( !( resourceID_B < resourceID_A1 ) );
    BOOST_CHECK( !( resourceID_A1 < resourceID_A1 ) );
}


BOOST_AUTO_TEST_CASE( ResourceID_Increments ){
    como::ResourceID resourceID_A( 0, 0 );
    como::ResourceID resourceID_B( 0, 1 );

    BOOST_CHECK_NE( resourceID_A++, resourceID_B );
    BOOST_CHECK_EQUAL( resourceID_A, resourceID_B );
    BOOST_CHECK_NE( ++resourceID_A, resourceID_B );

    BOOST_CHECK_EQUAL( resourceID_A, como::ResourceID( 0, 2 ) );

    // Make a ResourceID overflow and check that only its ResourceIndex part
    // gets overflowed.
    como::ResourceID overflowedID( UINT16_MAX, UINT32_MAX );
    como::ResourceID maximumID = overflowedID++;
    BOOST_CHECK_EQUAL( maximumID.getCreatorID(), overflowedID.getCreatorID() );
    BOOST_CHECK_NE( maximumID.getResourceIndex(), overflowedID.getResourceIndex() );
    BOOST_CHECK_EQUAL( overflowedID.getResourceIndex(), 0 );
    BOOST_CHECK_EQUAL( (++overflowedID).getResourceIndex(), 1 );
}
