//
// fiestygen.cpp : Defines the entry point for the fiesty bitmap generator 
// progam
//
#include <iostream>
#include "bitboard.h"
#include "fiestygen.h"

// #define BBTRACE 

int main( int argc, const char* argv[] )
{
    std::cout << "//FiestyGen (C) 2014 by Jeffery A Esposito" << std::endl;
    CFiestyGen::generate();
    return 0;
}

///
/// Generates source code for various bitboards, such as move sets and 
/// magics.
///
void CFiestyGen::generate()
{
    genKnightAttacks();
    genRookRays();
}

///
/// Generates source code for the knight moves
///
void CFiestyGen::genKnightAttacks()
{
    std::cout << "const YBitBoard CGen::mbbKnightAttacks[CSqix::kNumSquares] = {";
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
void CFiestyGen::genRookRays()
{
    std::cout << "const SRookRays CGen::mbbRookRays[CSqix::kNumSquares] = {";
    for ( U8 sq =  0; sq < CSqix::kNumSquares; sq++ )
    {
        CSqix sqix( sq );
        U8 rookRank = U8( sqix.getRank().get() );
        U8 rookFile = U8( sqix.getFile().get() );
        CBitBoard bbNorth( 0ULL );
		CBitBoard bbEast( 0ULL );
		CBitBoard bbSouth( 0ULL );
		CBitBoard bbWest( 0ULL );
		//
		//	Loop setting the north bits
		//
		for ( U8 r = rookRank + 1; r <= U8( ERank::kRank8 ); r++ )
			bbNorth.setSquare( CSqix( ERank( r ), EFile( rookFile ) ).get() );
		//
		//	Loop setting the east bits
		//
		for ( U8 f = rookFile + 1; f <= U8( EFile::kFileH ); f++ )
			bbEast.setSquare( CSqix( ERank( rookRank ), EFile( f ) ).get() );
		//
		//	Loop setting the south bits
		//
		for ( U8 r = U8( ERank::kRank1 ); r < rookRank; r++ )
			bbSouth.setSquare( CSqix( ERank( r ), EFile( rookFile ) ).get() );
		//
		//	Loop setting the west bits
		//
		for ( U8 f = U8( EFile::kFileA ); f < rookFile; f++ )
			bbWest.setSquare( CSqix( ERank( rookRank ), EFile( f ) ).get() );
#ifdef BBTRACE
            printBitBoardDiagram( sqix.asAbbr() + " North",  bbNorth );
            printBitBoardDiagram( sqix.asAbbr() + " East",  bbEast );
            printBitBoardDiagram( sqix.asAbbr() + " South",  bbSouth );
            printBitBoardDiagram( sqix.asAbbr() + " West",  bbWest );
#endif
        std::cout << "\n    /* " << sqix.asAbbr() << " */ { "
            << bbNorth.asAbbr() << "ULL" << ", "
            << bbEast.asAbbr() << "ULL" << ", "
            << bbSouth.asAbbr() << "ULL" << ", "
            << bbWest.asAbbr() << "ULL" << " }";
        if ( sq != CSqix::kNumSquares - 1 )
            std::cout << ", ";
    }
    std::cout << "};\n" << std::flush;
}

///
/// prints a labeled bitboard diagram as a comment
///
void CFiestyGen::printBitBoardDiagram( std::string& rLabel, CBitBoard bb )
{
    std::cout << "\n/* " << rLabel << "\n" << bb.asDiagram() << "*/";
}


