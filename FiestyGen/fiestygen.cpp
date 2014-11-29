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
