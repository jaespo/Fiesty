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
/// Generates source code for various bitboards, such as move maps and 
/// magics.
///
void CBitBoard::generate()
{
    genKnightMoveMaps();
}

///
/// Generates source code for the knight move map
///
void CBitBoard::genKnightMoveMaps()
{
    std::cout << "    bbKnightMoveMap[CSqix::kNumSquares] = {";
    for ( U8 sq =  0; sq < CSqix::kNumSquares; sq++ )
    {
        CSqix sqix( sq );
        I8 r = I8( sqix.getRank().get() );
        I8 f = I8( sqix.getFile().get() );
        std::cout << "\n        /* " << sqix.asAbbr() << " */ ";
        CBitBoard bb( 0ULL );
        bb.setIfValid( r + 2, f + 1 );
        bb.setIfValid( r + 2, f - 1 );
        bb.setIfValid( r - 2, f + 1 );
        bb.setIfValid( r - 2, f - 1 );
        bb.setIfValid( r + 1, f + 2 );
        bb.setIfValid( r + 1, f - 2 );
        bb.setIfValid( r - 1, f + 2 );
        bb.setIfValid( r - 1, f - 2 );
        std::cout << bb.asAbbr();
        if( sq != CSqix::kNumSquares - 1 ) 
            std::cout << ",";
    }
    std::cout << " };\n" << std::flush;
}

///
/// helper function that sets a bit if its a valid location
///
void CBitBoard::setIfValid( I8 rank, I8 file )
{
    if ( rank < I8( ERank::kRank1 ) || rank > I8( ERank::kRank8 ) 
        || file < I8( EFile::kFileA ) || file > I8( EFile::kFileH ) )
    {
        return;
    }
    setSquare( CSqix( ERank( rank ), EFile( file ) ).get() );
}

        