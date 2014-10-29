/// file test.cpp
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// Unit tests
///
#include "test.h"
#include "piece.h"
#include "position.h"

int          CTester::mgOkCount          = 0;
char*        CTester::mgCurSuiteName     = nullptr;
bool         CTester::mgbInSuite         = false;

///
/// Starts a test suite
///
void CTester::beginSuite( char* pzSuiteName )
{
    mgCurSuiteName = pzSuiteName;
    mgOkCount = 0;
    mgbInSuite = true;
}

///
/// Ends a test suite
///
void CTester::endSuite()
{
    std::cout << mgCurSuiteName << ": " << mgOkCount << " tests ok"<< std::endl;
    mgbInSuite = false;
}


///
/// Test bitboard.h
///
void CTester::testBitBoard()
{
    beginSuite( "testBitBoard" );
    CSqix g7( ERank::kRank7, EFile::kFileG );
    CSqix c3( ERank::kRank3, EFile::kFileC );

    CBitBoard bm( 0ULL );
    bm.setSquare( c3.get() );

/*
    c3 is 

    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 1 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    
    0x00000000040000ULL
    
    g7 is

    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 1 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0
    0 0 0 0 0 0 0 0

    0x004000000000000ULL
*/

    TESTEQ( "c3bmAsStr", bm.asStr(),
            "00000000/"
            "00000000/"
            "00000000/"
            "00000000/"
            "00000000/"
            "00100000/"
            "00000000/"
            "00000000" );
    TESTEQ( "c3bmAsAbbr", bm.asAbbr(), "0x40000" );

    bm.setSquare( g7.get() );
    TESTEQ( "c2g7bmAsStr", bm.asStr(),
            "00000000/"
            "00000010/"
            "00000000/"
            "00000000/"
            "00000000/"
            "00100000/"
            "00000000/"
            "00000000" );
    TESTEQ( "c3bmAsAbbr", bm.asAbbr(), "0x40000000040000" );

    CSqix sqix = bm.popLsb();
    TESTEQ( "popC3", sqix.asAbbr(), "c3" );

    sqix = bm.popLsb();
    TESTEQ( "popG7", sqix.asAbbr(), "g7" );
    TESTEQ( "emptyBitBoard", bm.get(), 0 );
    endSuite();
}

///
/// tests the piece.h module
///
void CTester::testPiece( void )
{
    beginSuite( "testPiece" );

    //
    //  Colors
    //
    TESTEQ( "getOpponentWhite", 
        CColor( EColor::kBlack ), CColor( EColor::kWhite ).getOpponent() ); 
    TESTEQ( "getOpponentBlack", 
        CColor( EColor::kWhite ), CColor( EColor::kBlack ).getOpponent() ); 
    TESTEQ( "asStrWhite", CColor( EColor::kWhite ).asStr(), "White" );
    TESTEQ( "asStrBlack", CColor( EColor::kBlack ).asStr(), "Black" );
    TESTEQ( "asAbbrWhite", CColor( EColor::kWhite ).asAbbr(), "W" );
    TESTEQ( "asAbbrBlack", CColor( EColor::kBlack ).asAbbr(), "B" );

    //
    //  Pieces type strings
    //
    TESTEQ( "asStrPawn", CPieceType( EPieceType::kPawn ).asStr(), "Pawn" );
    TESTEQ( "asStrKnight", CPieceType( EPieceType::kKnight ).asStr(), "Knight" );
    TESTEQ( "asStrBishop", CPieceType( EPieceType::kBishop ).asStr(), "Bishop" );
    TESTEQ( "asStrRook", CPieceType( EPieceType::kRook ).asStr(), "Rook" );
    TESTEQ( "asStrQueen", CPieceType( EPieceType::kQueen ).asStr(), "Queen" );
    TESTEQ( "asStrKing", CPieceType( EPieceType::kKing ).asStr(), "King" );

    //
    //  Pieces type abbreviations
    //
    TESTEQ( "asAbbrPawn", CPieceType( EPieceType::kPawn ).asAbbr(), "P" );
    TESTEQ( "asAbbrKnight", CPieceType( EPieceType::kKnight ).asAbbr(), "N" );
    TESTEQ( "asAbbrBishop", CPieceType( EPieceType::kBishop ).asAbbr(), "B" );
    TESTEQ( "asAbbrRook",  CPieceType( EPieceType::kRook ).asAbbr(), "R" );
    TESTEQ( "asAbbrQueen", CPieceType( EPieceType::kQueen ).asAbbr(), "Q" );
    TESTEQ( "asAbbrKing", CPieceType( EPieceType::kKing ).asAbbr(), "K" );

    //
    //  White pieces type strings
    //
    TESTEQ( "asStrWhitePawn", CPiece( EPiece::kWhitePawn ).asStr(), "White Pawn" );
    TESTEQ( "asStrWhiteKnight", CPiece( EPiece::kWhiteKnight ).asStr(), "White Knight" );
    TESTEQ( "asStrWhiteBishop", CPiece( EPiece::kWhiteBishop ).asStr(), "White Bishop" );
    TESTEQ( "asStrWhiteRook", CPiece( EPiece::kWhiteRook ).asStr(), "White Rook" );
    TESTEQ( "asStrWhiteQueen", CPiece( EPiece::kWhiteQueen ).asStr(), "White Queen" );
    TESTEQ( "asStrWhiteKing", CPiece( EPiece::kWhiteKing ).asStr(), "White King" );

    //
    //  Black pieces type strings
    //
    TESTEQ( "asStrBlackPawn", CPiece( EPiece::kBlackPawn ).asStr(), "Black Pawn" );
    TESTEQ( "asStrBlackKnight", CPiece( EPiece::kBlackKnight ).asStr(), "Black Knight" );
    TESTEQ( "asStrBlackBishop", CPiece( EPiece::kBlackBishop ).asStr(), "Black Bishop" );
    TESTEQ( "asStrBlackRook", CPiece( EPiece::kBlackRook ).asStr(), "Black Rook" );
    TESTEQ( "asStrBlackQueen", CPiece( EPiece::kBlackQueen ).asStr(), "Black Queen" );
    TESTEQ( "asStrBlackKing", CPiece( EPiece::kBlackKing ).asStr(), "Black King" );

    //
    //  White pieces type abbreviations
    //
    TESTEQ( "asAbbrWhitePawn", CPiece( EPiece::kWhitePawn ).asAbbr(), "P" );
    TESTEQ( "asAbbrWhiteKnight", CPiece( EPiece::kWhiteKnight ).asAbbr(), "N" );
    TESTEQ( "asAbbrWhiteBishop", CPiece( EPiece::kWhiteBishop ).asAbbr(), "B" );
    TESTEQ( "asAbbrWhiteRook", CPiece( EPiece::kWhiteRook ).asAbbr(), "R" );
    TESTEQ( "asAbbrWhiteQueen", CPiece( EPiece::kWhiteQueen ).asAbbr(), "Q" );
    TESTEQ( "asAbbrWhiteKing", CPiece( EPiece::kWhiteKing ).asAbbr(), "K" );

    //
    //  Black pieces type abbreviations
    //
    TESTEQ( "asAbbrBlackPawn", CPiece( EPiece::kBlackPawn ).asAbbr(), "p" );
    TESTEQ( "asAbbrBlackKnight", CPiece( EPiece::kBlackKnight ).asAbbr(), "n" );
    TESTEQ( "asAbbrBlackBishop", CPiece( EPiece::kBlackBishop ).asAbbr(), "b" );
    TESTEQ( "asAbbrBlackRook", CPiece( EPiece::kBlackRook ).asAbbr(), "r" );
    TESTEQ( "asAbbrBlackQueen", CPiece( EPiece::kBlackQueen ).asAbbr(), "q" );
    TESTEQ( "asAbbrBlackKing", CPiece( EPiece::kBlackKing ).asAbbr(), "k" );
    endSuite();
}

///
/// tests the position.h module
///
void CTester::testPosition( void )
{
    beginSuite( "testPosition" );

    //
    //  Rank as String
    //
    TESTEQ( "asStrRank1", CRank( ERank::kRank1 ).asStr(), "1st rank" );
    TESTEQ( "asStrRank2", CRank( ERank::kRank2 ).asStr(), "2nd rank" );
    TESTEQ( "asStrRank3", CRank( ERank::kRank3 ).asStr(), "3rd rank" );
    TESTEQ( "asStrRank4", CRank( ERank::kRank4 ).asStr(), "4th rank" );
    TESTEQ( "asStrRank5", CRank( ERank::kRank5 ).asStr(), "5th rank" );
    TESTEQ( "asStrRank6", CRank( ERank::kRank6 ).asStr(), "6th rank" );
    TESTEQ( "asStrRank7", CRank( ERank::kRank7 ).asStr(), "7th rank" );
    TESTEQ( "asStrRank8", CRank( ERank::kRank8 ).asStr(), "8th rank" );

    //
    //  Rank as Abbreviation
    //
    TESTEQ( "asAbbrRank1", CRank( ERank::kRank1 ).asAbbr(), "1" );
    TESTEQ( "asAbbrRank2", CRank( ERank::kRank2 ).asAbbr(), "2" );
    TESTEQ( "asAbbrRank3", CRank( ERank::kRank3 ).asAbbr(), "3" );
    TESTEQ( "asAbbrRank4", CRank( ERank::kRank4 ).asAbbr(), "4" );
    TESTEQ( "asAbbrRank5", CRank( ERank::kRank5 ).asAbbr(), "5" );
    TESTEQ( "asAbbrRank6", CRank( ERank::kRank6 ).asAbbr(), "6" );
    TESTEQ( "asAbbrRank7", CRank( ERank::kRank7 ).asAbbr(), "7" );
    TESTEQ( "asAbbrRank8", CRank( ERank::kRank8 ).asAbbr(), "8" );

    //
    //  File as String
    //
    TESTEQ( "asStrFileA", CFile( EFile::kFileA ).asStr(), "a file" );
    TESTEQ( "asStrFileB", CFile( EFile::kFileB ).asStr(), "b file" );
    TESTEQ( "asStrFileC", CFile( EFile::kFileC ).asStr(), "c file" );
    TESTEQ( "asStrFileD", CFile( EFile::kFileD ).asStr(), "d file" );
    TESTEQ( "asStrFileE", CFile( EFile::kFileE ).asStr(), "e file" );
    TESTEQ( "asStrFileF", CFile( EFile::kFileF ).asStr(), "f file" );
    TESTEQ( "asStrFileG", CFile( EFile::kFileG ).asStr(), "g file" );
    TESTEQ( "asStrFileH", CFile( EFile::kFileH ).asStr(), "h file" );

    //
    //  File as Abbreviation
    //
    TESTEQ( "asAbbrFileA", CFile( EFile::kFileA ).asAbbr(), "a" );
    TESTEQ( "asAbbrFileB", CFile( EFile::kFileB ).asAbbr(), "b" );
    TESTEQ( "asAbbrFileC", CFile( EFile::kFileC ).asAbbr(), "c" );
    TESTEQ( "asAbbrFileD", CFile( EFile::kFileD ).asAbbr(), "d" );
    TESTEQ( "asAbbrFileE", CFile( EFile::kFileE ).asAbbr(), "e" );
    TESTEQ( "asAbbrFileF", CFile( EFile::kFileF ).asAbbr(), "f" );
    TESTEQ( "asAbbrFileG", CFile( EFile::kFileG ).asAbbr(), "g" );
    TESTEQ( "asAbbrFileH", CFile( EFile::kFileH ).asAbbr(), "h" );

    //
    //  Square index test
    //
    CSqix c4( ERank::kRank4, EFile::kFileC );
    TESTEQ( "asAbbrC4", c4.asAbbr(), "c4" );
    TESTEQ( "asStrC4", c4.asStr(), "c4(26)" );
    endSuite();
}

///
/// Test the moves.h
///
void CTester::testMove()
{
    beginSuite( "testMove" );
    CMove c3c4( 
        CSqix( ERank::kRank3, EFile::kFileC ),
        CSqix( ERank::kRank4, EFile::kFileC ) );
    CMove d3d4( 
        CSqix( ERank::kRank3, EFile::kFileD ),
        CSqix( ERank::kRank4, EFile::kFileD ) );
    CMove e7e8Q( 
        CSqix( ERank::kRank7, EFile::kFileE ),
        CSqix( ERank::kRank8, EFile::kFileE ), 
        EPieceType::kQueen );

    //
    //  Make sure the compiler is packing the size correctly
    //
    TESTEQ( "moveSize", sizeof( c3c4 ), 2 );

    TESTEQ( "c3c4AsStr",    c3c4.asStr(), "c3c4" );
    TESTEQ( "c3c4From",     c3c4.getFrom().asAbbr(), "c3" );
    TESTEQ( "c3c4To",       c3c4.getTo().asAbbr(), "c4" );

    TESTEQ( "d3d4AsStr",    d3d4.asStr(), "d3d4" );
    TESTEQ( "d3d4From",     d3d4.getFrom().asAbbr(), "d3" );
    TESTEQ( "d3d4To",       d3d4.getTo().asAbbr(), "d4" );

    TESTEQ( "e7e8QAsStr",   e7e8Q.asStr(), "e7e8=Q" );
    TESTEQ( "e7e8QFrom",    e7e8Q.getFrom().asAbbr(), "e7" );
    TESTEQ( "e7e8QTo",      e7e8Q.getTo().asAbbr(), "e8" );
    TESTEQ( "e7e8QPromo",   e7e8Q.getPromo().asAbbr(), "Q" );

    CMoves      moves;
    moves.addMove( c3c4 );
    moves.addMove( d3d4 );

    TESTEQ( "movesAsStr", moves.asStr(), "2:c3c4 d3d4" );
    
    CLine       line;
    line.addMove( c3c4, 0 );
    line.addMove( d3d4, 0 );

    TESTEQ( "lineAsAbbr", line.asAbbr(), "2:c3c4 d3d4" );
    TESTEQ( "lineAsStr", line.asStr(), "2 of 1024:c3c4 d3d4" );
    endSuite();
}

///
/// Run all the tests
///
void CTester::testAll()
{
    testPiece();
    testPosition();
    testMove();
    testBitBoard();
}
