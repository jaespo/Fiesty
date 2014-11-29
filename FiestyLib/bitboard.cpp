///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with BitBoards
///
///
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "fiesty.h"
#include "square.h"
#include "bitboard.h"

///
/// returns a BitBoard as a fenlike string
///
std::string CBitBoard::asStr() const
{
    std::string s;
    for ( std::int8_t r = U8( ERank::kRank8 ); r >= 0; r-- )
    {
        for ( U8 f = U8( EFile::kFileA ); f <= U8( EFile::kFileH ); f++ )
        {
            CSqix sqix = CSqix( ERank( r ), EFile( f ) );
            s.append( ( sqix.asBitBoard() & mBitBoard ) ? "1" : "0" );
        }
        if ( r ) 
            s.append( "/" );
    }
    return s;
}

///
/// returns a BitBoard as an 8x8 diagram
///
std::string CBitBoard::asDiagram() const
{
    std::string s;
    for ( std::int8_t r = U8( ERank::kRank8 ); r >= 0; r-- )
    {
        for ( U8 f = U8( EFile::kFileA ); f <= U8( EFile::kFileH ); f++ )
        {
            CSqix sqix = CSqix( ERank( r ), EFile( f ) );
            s.append( ( sqix.asBitBoard() & mBitBoard ) ? "1" : "0" );
        }
        s.append( "\n" );
    }
    return s;
}

///
/// returns a BitBoard as a hex string
///
std::string CBitBoard::asAbbr() const
{
    std::stringstream stream;
    stream << "0x" << std::hex << mBitBoard;
    std::string result( stream.str() );
    return result;
}

///
/// helper function that sets a bit if its a valid location
///
void CBitBoard::setIfValid( S8 rank, S8 file )
{
    if ( rank < S8( ERank::kRank1 ) || rank > S8( ERank::kRank8 ) 
        || file < S8( EFile::kFileA ) || file > S8( EFile::kFileH ) )
    {
        return;
    }
    setSquare( CSqix( ERank( rank ), EFile( file ) ).get() );
}

        