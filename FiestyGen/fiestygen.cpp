//
// fiestygen.cpp : Defines the entry point for the fiesty bitmap generator 
// progam
//
#include <iostream>
#include "bitboard.h"
#include "generator.h"

int main( int argc, const char* argv[] )
{
    std::cout << "//FiestyGen (C) 2014 by Jeffery A Esposito" << std::endl;
    CGenerator::generate();
    return 0;
}

///
/// Generates source code for various bitboards, such as move sets and 
/// magics.
///
void CGenerator::generate()
{
    genKnightAttacks();
}

///
/// Generates source code for the knight moves
///
void CGenerator::genKnightAttacks()
{
    std::cout << "const YBitBoard CGen::bbKnightAttacks[CSqix::kNumSquares] = {";
    for ( U8 sq =  0; sq < CSqix::kNumSquares; sq++ )
    {
        CSqix sqix( sq );
        S8 r = S8( sqix.getRank().get() );
        S8 f = S8( sqix.getFile().get() );
        std::cout << "\n    /* " << sqix.asAbbr() << " */ ";
        CBitBoard bb( 0ULL );
        bb.setIfValid( r + 2, f + 1 );
        bb.setIfValid( r + 2, f - 1 );
        bb.setIfValid( r - 2, f + 1 );
        bb.setIfValid( r - 2, f - 1 );
        bb.setIfValid( r + 1, f + 2 );
        bb.setIfValid( r + 1, f - 2 );
        bb.setIfValid( r - 1, f + 2 );
        bb.setIfValid( r - 1, f - 2 );
        std::cout << bb.asAbbr() << "ULL";
        if( sq != CSqix::kNumSquares - 1 ) 
            std::cout << ",";
    }
    std::cout << " };\n" << std::flush;
}

///
/// Generates source code for the rook rays.
///
void CGenerator::genKnightAttacks()
{
    std::cout << "const CRookRays CGen::mRookRays[CSqix::kNumSquares] = {";
    for ( U8 sq =  0; sq < CSqix::kNumSquares; sq++ )
    {
        CSqix sqix( sq );
        U8 rank = U8( sqix.getRank().get() );
        U8 file = U8( sqix.getFile().get() );
        std::cout << "\n    /* " << sqix.asAbbr() << " */ ";
        CBitBoard bbNorth( 0ULL );
		CBitBoard bbEast( 0ULL );
		CBitBoard bbSouth( 0ULL );
		CBitBoard bbWest( 0ULL );

		//
		//	Set the north bits
		//
		for ( U8 r = rank + 1; r <= U8( ERank::kRank8 ); r++ )
			bbNorth.setSquare( CSqix( ERank( r ), EFile( file ) ).get() );
		//
		//	Set the north bits
		//
		for ( U8 r = rank + 1; r <= U8( ERank::kRank8 ); r++ )
			bbNorth.setSquare( CSqix( ERank( r ), EFile( file ) ).get() );
        std::cout << bb.asAbbr() << "ULL";
        if( sq != CSqix::kNumSquares - 1 ) 
            std::cout << ",";
    }
    std::cout << " };\n" << std::flush;
}
