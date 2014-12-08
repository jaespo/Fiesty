/// file searchC.cpp
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with searches
///
///
#include "search.h"

///
/// generates the perft node count for the current position.
///
U64 CSearch::perft( U16 depthLeft )
{
    if ( depthLeft == 0 )
        return 1;

    CMoves      moves;
    U64         nodeCount = 0;

    mpPos->genLegalMoves( moves );
    for ( int moveIx = 0; moveIx = moves.getNumMoves(); moveIx++ )
    {
        CMove move = moves[moveIx];
        mpPos->makeMove( move );
        nodeCount += perft( depthLeft - 1 );
        mpPos->unmakeMove( move );
    }
}
