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
    std::cout 
        << mgCurSuiteName << ": " << mgOkCount << " tests ok" << std::endl;
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
/// tests checks
///
void CTester::testCheck()
{
    beginSuite( "testCheck" );

    CPos            pos;
    std::string     errorText;
    bool            bOk;

    //
    //  Test for no checkers in the starting position
    //
    TESTEQ( "checkWhiteStartFen", 
        bOk = pos.parseFen( CPos::kStartFen, errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteStart", pos.getCheckers().asStrSquares(), "" );

    //////////////////////////////////////////////////////////////////////////
    //
    //  White Pawn Checks
    //
    //////////////////////////////////////////////////////////////////////////

    //
    //  Test white pawn checking to the left
    //
    TESTEQ( "checkWhitePawnLeftFen", 
        bOk = pos.parseFen( "8/8/8/8/4k3/3P4/4P3/4K3 b - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhitePawnLeft", pos.getCheckers().asStrSquares(), "d3" );

    //
    //  Test white pawn checking to the right
    //
    TESTEQ( "checkWhitePawnRightFen", 
        bOk = pos.parseFen( "8/8/8/8/2k5/3P4/4P3/4K3 b - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhitePawnRight", pos.getCheckers().asStrSquares(), "d3" );

    //
    //  Make sure nothing terrible happens when the black king is on the 
    //  1st rank.
    //
    TESTEQ( "checkWhitePawn1stRankFen", 
        bOk = pos.parseFen( "8/8/8/8/8/3P4/4P3/2k1K3 b - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhitePawn1stRank", pos.getCheckers().asStrSquares(), "" );

    //
    //  Test that checks don't wrap over the left side of the board.
    //
    TESTEQ( "checkWhitePawnWrapLeftFen", 
        bOk = pos.parseFen( "8/8/8/8/7k/P7/4P3/4K3 b - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhitePawnWrapLeft", pos.getCheckers().asStrSquares(), "" );

    //
    //  Test that checks don't wrap over the right side of the board.
    //
    TESTEQ( "checkWhitePawnWrapRightFen", 
        bOk = pos.parseFen( "k7/7P/K7/8/8/8/6P1/8 b - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhitePawnWrapRight", pos.getCheckers().asStrSquares(), "" );

    //////////////////////////////////////////////////////////////////////////
    //
    //  Black Pawn Checks
    //
    //////////////////////////////////////////////////////////////////////////

    //
    //  Test black pawn checking to the left
    //
    TESTEQ( "checkBlackPawnLeftFen", 
        bOk = pos.parseFen( "2k5/8/4p3/3p4/2K5/8/8/8 w - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findBlackCheckers();
    TESTEQ( "checkBlackPawnLeft", pos.getCheckers().asStrSquares(), "d5" );

    //
    //  Test Black pawn checking to the right
    //
    TESTEQ( "checkBlackPawnRightFen", 
        bOk = pos.parseFen( "2k5/8/4p3/3p1K2/8/8/8/8 w - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findBlackCheckers();
    TESTEQ( "checkBlackPawnRight", pos.getCheckers().asStrSquares(), "e6" );

    //
    //  Make sure nothing terrible happens when the white king is on the 
    //  8th rank.
    //
    TESTEQ( "checkBlackPawn8thRankFen", 
        bOk = pos.parseFen( "2k1K3/8/4p3/3p4/8/8/8/8 w - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findBlackCheckers();
    TESTEQ( "checkBlackPawn8thRank", pos.getCheckers().asStrSquares(), "" );

    //
    //  Test that checks don't wrap over the left side of the board.
    //
    TESTEQ( "checkBlackPawnWrapLeftFen", 
        bOk = pos.parseFen( "2k5/8/4p3/8/p7/7K/8/8 w - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findBlackCheckers();
    TESTEQ( "checkBlackPawnWrapLeft", pos.getCheckers().asStrSquares(), "" );

    //
    //  Test that checks don't wrap over the right side of the board.
    //
    TESTEQ( "checkBlackPawnWrapRightFen", 
        bOk = pos.parseFen( "2k5/8/4p3/8/8/8/7p/K7 w - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findBlackCheckers();
    TESTEQ( "checkBlackPawnWrapRightFen", pos.getCheckers().asStrSquares(), "" );

    //////////////////////////////////////////////////////////////////////////
    //
    //  Knight Checks
    //
    //////////////////////////////////////////////////////////////////////////

    //
    //  Test white knight check
    //
    TESTEQ( "checkWhiteKnightFen", 
        bOk = pos.parseFen( "2k5/8/1N2p3/8/8/8/7p/K7 b - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteKnight", pos.getCheckers().asStrSquares(), "b6" );

    //
    //  Test black knight check
    //
    TESTEQ( "checkBlackKnightFen", 
        bOk = pos.parseFen( "2k5/8/1n2p3/8/3K4/8/2n4p/8 w - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findBlackCheckers();
    TESTEQ( "checkBlackKnight", pos.getCheckers().asStrSquares(), "c2" );

    //
    //  Make sure a black knight does register as given check to the black
    //  king.
    //
    TESTEQ( "checkWhiteKnightNonCheckFen", 
        bOk = pos.parseFen( "2k5/8/1n2p3/8/3K4/8/2n4p/8 b - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteKnightNonCheck", pos.getCheckers().asStrSquares(), "" );

    //////////////////////////////////////////////////////////////////////////
    //
    //  King Checks
    //
    //////////////////////////////////////////////////////////////////////////

    //
    //  Test white king check
    //
    TESTEQ( "checkWhiteKingFen", 
        bOk = pos.parseFen( "2kK4/8/1n2p3/8/8/8/2n4p/8 w - - 0 1", errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteKing", pos.getCheckers().asStrSquares(), "d8" );

    //
    //  Test black king check
    //
    pos.findBlackCheckers();
    TESTEQ( "checkBlackKing", pos.getCheckers().asStrSquares(), "c8" );

    //////////////////////////////////////////////////////////////////////////
    //
    //  White Rook Checks
    //
    //////////////////////////////////////////////////////////////////////////

    //
    //  Test rook check -- left rank
    //
    TESTEQ( "checkWhiteRookLeftRankFen", 
        bOk = pos.parseFen( "R1k5/8/1n2p3/8/8/3K4/2n4p/8 b - - 0 1", 
            errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteRookRank", pos.getCheckers().asStrSquares(), "a8" );

    //
    //  Test white rook check -- right rank
    //
    TESTEQ( "checkWhiteRookRightRankFen", 
        bOk = pos.parseFen( "2k3R1/8/1n2p3/8/8/3K4/2n4p/8 b - - 0 1", 
            errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteRookRightRank", pos.getCheckers().asStrSquares(), "g8" );

    //
    //  Test white rook check -- forward file
    //
    TESTEQ( "checkWhiteRookForwardFileFen", 
        bOk = pos.parseFen( "2k5/8/1n2p3/8/8/2RK4/2n4p/8 b - - 0 1", 
            errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteRookForwardFile", pos.getCheckers().asStrSquares(), "c3" );

    //
    //  Test white rook check -- reverse file
    //
    TESTEQ( "checkWhiteRookReverseFileFen", 
        bOk = pos.parseFen( "8/8/1n2pk2/8/8/3K4/2n4p/5R2 w - - 0 1", 
            errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteRookReverseFile", pos.getCheckers().asStrSquares(), "f1" );


    //
    //  Test white rook check -- white blocker
    //
    TESTEQ( "checkWhiteRookWhiteBlockerFen", 
        bOk = pos.parseFen( "8/8/1n2pk2/8/8/3K4/2n2P1p/5R2 b - - 0 1", 
            errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteRookWhiteBlocker", pos.getCheckers().asStrSquares(), "" );

    //
    //  Test white rook check -- black blocker
    //
    TESTEQ( "checkWhiteRookBlackBlockerFen", 
        bOk = pos.parseFen( "8/8/1nR1pk2/5p2/8/3K4/2n4p/8 b - - 0 1", 
            errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findWhiteCheckers();
    TESTEQ( "checkWhiteRookBlackBlocker", pos.getCheckers().asStrSquares(), "" );

    //////////////////////////////////////////////////////////////////////////
    //
    //  Black Rook Checks
    //
    //////////////////////////////////////////////////////////////////////////

    //
    //  Test black rook check -- left rank
    //
    TESTEQ( "checkBlackRookLeftRankFen", 
        bOk = pos.parseFen( "8/8/1nR1pk2/5p2/8/2rK4/2n4p/8 w - - 0 1", 
            errorText ), true );
    if ( !bOk ) 
        std::cout << errorText << std::endl;
    pos.findBlackCheckers();
    TESTEQ( "checkBlackRookLeftRank", pos.getCheckers().asStrSquares(), "c3" );

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
/// tests move generation
///
void CTester::testMoveGen()
{
    beginSuite( "testMoveGen" );

    CMoves          moves;
    CPos            pos;
    std::string     errorText;

    //
    //  Test white pawn pushes from the starting position
    //
    TESTEQ( "wmgFen", pos.parseFen( CPos::kStartFen, errorText ), true );
    pos.genWhitePawnQuiets( moves );
    TESTEQ( "whitePawnQuiets", moves.asStr(), 
        "16:h2h3 g2g3 f2f3 e2e3 d2d3 c2c3 b2b3 a2a3 h2h4 g2g4 f2f4 e2e4 "
        "d2d4 c2c4 b2b4 a2a4" );

    //
    //  Test white promotions, single pushes from the non-second rank, 
    //  and blocked pawns.
    //
    TESTEQ( "wmgFen", pos.parseFen( 
        "8/4k2P/8/8/3p2P1/1K3p2/1P1P1P2/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhitePawnQuiets( moves );
    TESTEQ( "whitePawnQuiets2", moves.asStr(), 
        "6:h7h8=Q h7h8=R h7h8=B h7h8=N g4g5 d2d3" );

    //
    //  Test black pawn pushes from the starting position
    //
    TESTEQ( "bmgFen", pos.parseFen( CPos::kStartFen, errorText ), true );
    moves.reset();
    pos.genBlackPawnQuiets( moves );
    TESTEQ( "blackPawnQuiets", moves.asStr(), 
        "16:a7a6 b7b6 c7c6 d7d6 e7e6 f7f6 g7g6 h7h6 a7a5 b7b5 c7c5 d7d5 "
        "e7e5 f7f5 g7g5 h7h5" );

    //
    //  Test black promotions, single pushes from the non-second rank, 
    //  and blocked pawns.
    //
    TESTEQ( "bmgFen", pos.parseFen( 
        "4k3/p7/8/K2p1p2/3P4/8/7p/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackPawnQuiets( moves );
    TESTEQ( "blackPawnQuiets2", moves.asStr(), 
        "6:h2h1=Q h2h1=R h2h1=B h2h1=N f5f4 a7a6" );

    //
    //  Test white pawn captures.
    //
    TESTEQ( "wpCapFen", pos.parseFen( 
        "r1b1k3/1P6/p5pp/2Pp1pPP/4p3/1p1P1P1p/P7/4K3 w - f6 0 1", 
        errorText ), true );
    moves.reset();
    pos.genWhitePawnCaptures( moves );
    TESTEQ( "whitePawnCaptures", moves.asStr(), 
        "14:b7c8=Q b7c8=R b7c8=B b7c8=N b7a8=Q b7a8=R b7a8=B b7a8=N g5h6 "
        "h5g6 g5f6 d3e4 f3e4 a2b3" );

    //
    //  Test black pawn captures.
    //
    TESTEQ( "bpCapFen", pos.parseFen( 
        "8/8/2p1p3/1P1P4/Pp6/8/5p2/K3R1Q1 b - a3 0 1", 
        errorText ), true );
    moves.reset();
    pos.genBlackPawnCaptures( moves );
    TESTEQ( "blackPawnCaptures", moves.asStr(), 
        "12:f2e1=Q f2e1=R f2e1=B f2e1=N f2g1=Q f2g1=R f2g1=B f2g1=N b4a3 "
        "c6b5 c6d5 e6d5" );

    //
    //  Test white knight quiets.
    //
    TESTEQ( "wnqFen", pos.parseFen( 
        "N6N/1N4N1/8/3N4/8/8/1N4N1/N6N w - - 0 1", 
        errorText ), true );
    moves.reset();
    pos.genWhiteKnightQuiets( moves );
    TESTEQ( "whiteKnightQuiets", moves.asStr(), 
        "32:h8f7 h8g6 a8c7 a8b6 g7e8 g7e6 g7h5 g7f5 b7d8 b7d6 b7c5 b7a5 "
        "d5e7 d5c7 d5f6 d5b6 d5f4 d5b4 d5e3 d5c3 g2h4 g2f4 g2e3 g2e1 b2c4 "
        "b2a4 b2d3 b2d1 h1g3 h1f2 a1b3 a1c2" );

    //
    //  Test white knight captures
    //
    TESTEQ( "gwkFen", pos.parseFen( 
        "8/8/3P1p2/8/4N3/3p4/5p2/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteKnightCaptures( moves );
    TESTEQ( "whiteKnightCaptures", moves.asStr(), "2:e4f6 e4f2" );

    //
    //  Test black knight quiets.
    //
    TESTEQ( "bnqFen", pos.parseFen( 
        "n6n/1n4n1/8/3n4/8/8/1n4n1/n6n b - - 0 1", 
        errorText ), true );
    moves.reset();
    pos.genBlackKnightQuiets( moves );
    TESTEQ( "bnqMoves", moves.asStr(), 
        "32:a1c2 a1b3 h1f2 h1g3 b2d1 b2d3 b2a4 b2c4 g2e1 g2e3 g2f4 g2h4 "
        "d5c3 d5e3 d5b4 d5f4 d5b6 d5f6 d5c7 d5e7 b7a5 b7c5 b7d6 b7d8 g7f5 "
        "g7h5 g7e6 g7e8 a8b6 a8c7 h8g6 h8f7" );

    //
    //  Test black knight captures
    //
    TESTEQ( "gbkFen", pos.parseFen( 
        "8/8/3p1P2/8/4n3/3P4/5P2/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackKnightCaptures( moves );
    TESTEQ( "blackKnightCaptures", moves.asStr(), "2:e4f2 e4f6" );

    //
    //  Test white rook quiets
    //
    TESTEQ( "whiteRookQuietsFen", pos.parseFen( 
        "R6R/8/3p4/3R4/4R3/8/3R4/R6R w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteRookQuiets( moves );
    TESTEQ( "whiteRookQuiets", moves.asStr(), 
        "81:h8g8 h8f8 h8e8 h8d8 h8c8 h8b8 h8h7 h8h6 h8h5 h8h4 h8h3 h8h2 "
        "a8g8 a8f8 a8e8 a8d8 a8c8 a8b8 a8a7 a8a6 a8a5 a8a4 a8a3 a8a2 d5h5 "
        "d5g5 d5f5 d5e5 d5c5 d5b5 d5a5 d5d4 d5d3 e4e8 e4e7 e4e6 e4e5 e4h4 "
        "e4g4 e4f4 e4d4 e4c4 e4b4 e4a4 e4e3 e4e2 e4e1 d2d4 d2d3 d2h2 d2g2 "
        "d2f2 d2e2 d2c2 d2b2 d2a2 d2d1 h1h7 h1h6 h1h5 h1h4 h1h3 h1h2 h1g1 "
        "h1f1 h1e1 h1d1 h1c1 h1b1 a1a7 a1a6 a1a5 a1a4 a1a3 a1a2 a1g1 a1f1 "
        "a1e1 a1d1 a1c1 a1b1" );

    //
    //  Test white rook captures
    //
    TESTEQ( "whiteRookCapturesFen", pos.parseFen( 
        "8/8/2pp4/p1RP4/1R6/nP1RP1n1/2pP4/3q4 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteRookCaptures( moves );
    TESTEQ( "whiteRookCaptures", moves.asStr(), "3:c5c6 c5a5 c5c2" );

    //
    //  Test black rook quiets
    //
    TESTEQ( "blackRookQuietsFen", pos.parseFen( 
        "8/2P5/8/8/2r5/8/2r2p2/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackRookQuiets( moves );
    TESTEQ( "blackRookQuiets", moves.asStr(), 
        "16:c2c1 c2a2 c2b2 c2d2 c2e2 c2c3 c4c3 c4a4 c4b4 c4d4 c4e4 c4f4 "
        "c4g4 c4h4 c4c5 c4c6" );

    //
    //  Test black rook captures
    //
    TESTEQ( "blackRookCapturesFen", pos.parseFen( 
        "8/8/3P4/3p4/Pr1r2P1/8/8/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackRookCaptures( moves );
    TESTEQ( "blackRookCaptures", moves.asStr(), "2:b4a4 d4g4" );

    //
    //  Test white bishop quiets
    //
    TESTEQ( "whiteBishopQuietsFen", pos.parseFen( 
        "8/8/3pp3/p6P/1B1B2B1/P6P/3PP3/7B w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteBishopQuiets( moves );
    TESTEQ( "whiteBishopQuiets", moves.asStr(), 
        "24:g4f5 g4f3 d4h8 d4g7 d4a7 d4f6 d4b6 d4e5 d4c5 d4e3 d4c3 d4f2 "
        "d4b2 d4g1 d4a1 b4c5 b4c3 h1a8 h1b7 h1c6 h1d5 h1e4 h1f3 h1g2" );

    //
    //  Test white bishop captures
    //
    TESTEQ( "whiteBishopCapturesFen", pos.parseFen( 
        "r7/1P4pp/1p4P1/8/3BB3/2pP1P2/2p2pp1/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteBishopCaptures( moves );
    TESTEQ( "whiteBishopCaptures", moves.asStr(), 
        "4:d4g7 d4b6 d4f2 d4c3" );

    //
    //  Test black bishop quiets
    //
    TESTEQ( "blackBishopQuietsFen", pos.parseFen( 
        "8/p7/5P2/3b4/3b4/4p3/1p6/8 b - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackBishopQuiets( moves );
    TESTEQ( "blackBishopQuiets", moves.asStr(), 
        "17:d4c3 d4c5 d4e5 d4b6 d5h1 d5a2 d5g2 d5b3 d5f3 d5c4 d5e4 d5c6 "
        "d5e6 d5b7 d5f7 d5a8 d5g8" );

    //
    //  Test black bishop captures
    //
    TESTEQ( "blackBishopCapturesFen", pos.parseFen( 
        "N3N3/1pPp2P1/2b5/1p2b3/N3p3/5N2/1P5P/8 b - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackBishopCaptures( moves );
    TESTEQ( "blackBishopCaptures", moves.asStr(), 
        "4:e5g7 e5c7 e5h2 e5b2" );

    //
    //  Test white queen quiets
    //
    TESTEQ( "whiteQueenQuietsFen", pos.parseFen( 
        "8/4p3/8/4Q3/8/4P1P1/1P6/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteQueenQuiets( moves );
    TESTEQ( "whiteQueenQuiets", moves.asStr(), 
        "18:e5e6 e5h5 e5g5 e5f5 e5d5 e5c5 e5b5 e5a5 e5e4 e5h8 e5b8 e5g7 "
        "e5c7 e5f6 e5d6 e5f4 e5d4 e5c3" );

    //
    //  Test white queen captures
    //
    TESTEQ( "whiteQueenCapturesFen", pos.parseFen( 
        "8/4p3/8/4Q3/8/4P1P1/1P6/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteQueenCaptures( moves );
    TESTEQ( "whiteQueenCaptures", moves.asStr(), "1:e5e7" );

    //
    //  Test black queen quiets
    //
    TESTEQ( "blackQueenQuietsFen", pos.parseFen( 
        "8/3p4/5p2/8/3q4/4P3/1P6/8 b - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackQueenQuiets( moves );
    TESTEQ( "blackQueenQuiets", moves.asStr(), 
        "17:d4d1 d4d2 d4d3 d4a4 d4b4 d4c4 d4e4 d4f4 d4g4 d4h4 d4d5 d4d6 "
        "d4c3 d4c5 d4e5 d4b6 d4a7" );

    //
    //  Test black queen captures
    //
    TESTEQ( "blackQueenCapturesFen", pos.parseFen( 
        "8/3p4/5p2/8/3q4/4P3/1P6/8 b - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackQueenCaptures( moves );
    TESTEQ( "blackQueenCaptures", moves.asStr(), "2:d4e3 d4b2" );

    //
    //  Test white king quiets
    //
    TESTEQ( "whiteKingQuietsFen", pos.parseFen( 
        "8/8/8/4P3/3K4/8/8/8 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteKingQuiets( moves );
    TESTEQ( "whiteKingQuiets", moves.asStr(), 
        "7:d4d5 d4c5 d4e4 d4c4 d4e3 d4d3 d4c3" );

    //
    //  Test white king captures
    //
    TESTEQ( "whiteKingCapturesFen", pos.parseFen( 
        "8/8/8/8/8/8/4p3/4Kq2 w - - 0 1", errorText ), true );
    moves.reset();
    pos.genWhiteKingCaptures( moves );
    TESTEQ( "whiteKingCaptures", moves.asStr(), "2:e1e2 e1f1" );

    //
    //  Test black king quiets
    //
    TESTEQ( "blackKingQuietsFen", pos.parseFen( 
        "8/8/8/8/8/8/pp6/1k6 b - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackKingQuiets( moves );
    TESTEQ( "blackKingQuiets", moves.asStr(), 
        "3:b1a1 b1c1 b1c2" );

    //
    //  Test black king captures
    //
    TESTEQ( "blackKingCapturesFen", pos.parseFen( 
        "6R1/5ppk/6Pp/8/8/8/8/8 b - - 0 1", errorText ), true );
    moves.reset();
    pos.genBlackKingCaptures( moves );
    TESTEQ( "blackKingCaptures", moves.asStr(), "2:h7g6 h7g8" );

    endSuite();
}

///
/// tests the piece.h module
///
void CTester::testPiece()
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
    TESTEQ( "asStrWhitePawn", CPiece( EPiece::kWhitePawn ).asStr(), 
        "White Pawn" );
    TESTEQ( "asStrWhiteKnight", CPiece( EPiece::kWhiteKnight ).asStr(), 
        "White Knight" );
    TESTEQ( "asStrWhiteBishop", CPiece( EPiece::kWhiteBishop ).asStr(), 
        "White Bishop" );
    TESTEQ( "asStrWhiteRook", CPiece( EPiece::kWhiteRook ).asStr(), 
        "White Rook" );
    TESTEQ( "asStrWhiteQueen", CPiece( EPiece::kWhiteQueen ).asStr(), 
        "White Queen" );
    TESTEQ( "asStrWhiteKing", CPiece( EPiece::kWhiteKing ).asStr(), 
        "White King" );

    //
    //  Black pieces type strings
    //
    TESTEQ( "asStrBlackPawn", CPiece( EPiece::kBlackPawn ).asStr(), 
        "Black Pawn" );
    TESTEQ( "asStrBlackKnight", CPiece( EPiece::kBlackKnight ).asStr(), 
        "Black Knight" );
    TESTEQ( "asStrBlackBishop", CPiece( EPiece::kBlackBishop ).asStr(), 
        "Black Bishop" );
    TESTEQ( "asStrBlackRook", CPiece( EPiece::kBlackRook ).asStr(), 
        "Black Rook" );
    TESTEQ( "asStrBlackQueen", CPiece( EPiece::kBlackQueen ).asStr(), 
        "Black Queen" );
    TESTEQ( "asStrBlackKing", CPiece( EPiece::kBlackKing ).asStr(), 
        "Black King" );

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
    CPosRights      rights;

    //
    //  test castling flags
    //
    TESTEQ( "castleAll", rights.castlingAsStr(), "KQkq" );
    rights.clearWhiteOOO();
    TESTEQ( "castleNoWQ", rights.castlingAsStr(), "Kkq" );
    rights.clearWhiteOO();
    TESTEQ( "castleNoW", rights.castlingAsStr(), "kq" );
    rights.clearBlackOO();
    TESTEQ( "castleNoWBk", rights.castlingAsStr(), "q" );
    rights.clearBlackOOO();
    TESTEQ( "castleNone", rights.castlingAsStr(), "-" );
    rights.setWhiteOO();
    TESTEQ( "castleK", rights.castlingAsStr(), "K" );
    rights.setWhiteOOO();
    TESTEQ( "castleW", rights.castlingAsStr(), "KQ" );
    rights.setBlackOO();
    TESTEQ( "castleKQk", rights.castlingAsStr(), "KQk" );
    rights.setBlackOOO();
    TESTEQ( "castleAll2", rights.castlingAsStr(), "KQkq" );
    
    //
    //  test moving some rooks
    //
    rights.onWqrMove();
    TESTEQ( "castleMoveNoWQ", rights.castlingAsStr(), "Kkq" );
    rights.onWkrMove();
    TESTEQ( "castleMoveNoW", rights.castlingAsStr(), "kq" );
    rights.onBkrMove();
    TESTEQ( "castleMoveNoWBk", rights.castlingAsStr(), "q" );
    rights.onBqrMove();
    TESTEQ( "castleMoveNone", rights.castlingAsStr(), "-" );

    //
    //  test moving the kings
    //
    rights.init();
    rights.onKMove( EColor::kWhite );
    TESTEQ( "castleMoveWk", rights.castlingAsStr(), "kq" );
    rights.onKMove( EColor::kBlack );
    TESTEQ( "castleMoveWk", rights.castlingAsStr(), "-" );

    rights.init();
    TESTEQ( "illegalEp", rights.asStr(), "KQkq -" );
    rights.setEnPassantFile( EFile::kFileE );
    TESTEQ( "whiteEp", rights.asStr(), "KQkq e" );

    //
    //  Test fen parsing
    CPos                pos;
    std::string         errorText;

    bool ok = pos.parseFen( CPos::kStartFen, errorText );
    if ( !ok )
        std::cout << "*** Error: " << errorText << std::endl << std::flush;
    TESTEQ( "pfen", ok, true );
    TESTEQ( "asDiagram", pos.asDiagram(), 
        "r n b q k b n r\n"
        "p p p p p p p p\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        ". . . . . . . .\n"
        "P P P P P P P P\n"
        "R N B Q K B N R\n"
        "whoseMove = W\n"
        "rights    = KQkq -\n"
        "move      = 1\n"
        "50-move   = 0\n"
        "dups      = 0\n" );
    std::string fen = pos.asFen(); 
    TESTEQ( "asFen", fen, CPos::kStartFen );
    std::string s = pos.asStr();
    TESTEQ( "posAsStr", s, 
        "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1\n"
        "P: 00000000/11111111/00000000/00000000/00000000/00000000/11111111/00000000\n"
        "N: 01000010/00000000/00000000/00000000/00000000/00000000/00000000/01000010\n"
        "B: 00100100/00000000/00000000/00000000/00000000/00000000/00000000/00100100\n"
        "R: 10000001/00000000/00000000/00000000/00000000/00000000/00000000/10000001\n"
        "Q: 00010000/00000000/00000000/00000000/00000000/00000000/00000000/00010000\n"
        "K: 00001000/00000000/00000000/00000000/00000000/00000000/00000000/00001000\n"
        "W: 00000000/00000000/00000000/00000000/00000000/00000000/11111111/11111111\n"
        "B: 11111111/11111111/00000000/00000000/00000000/00000000/00000000/00000000\n" );
    endSuite();
}

///
/// tests the square.h module
///
void CTester::testSquare( void )
{
    beginSuite( "testSquare" );

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
/// Run all the tests
///
void CTester::testAll()
{
    testPiece();
    testSquare();
    testMove();
    testBitBoard();
    testPosition();
    testMoveGen();
    testCheck();
}
