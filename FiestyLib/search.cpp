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
U64 CSearcher::perftBlack( U16 depthLeft )
{
    if ( depthLeft == 0 )
        return 1;

    CMoves      moves;
    U64         nodeCount = 0;

    mpPos->genLegalMoves( moves ); ...
    for ( U16 moveIx = 0; moveIx = moves.getNumMoves(); moveIx++ )
    {
        CMove move = moves.get( moveIx );
        mpPos->makeMove( move ); ...
        nodeCount += perft( depthLeft - 1 );
        mpPos->unmakeMove( move ); ...
    }
    return nodeCount;
}

///
/// generates the perft node count for the current position.
///
U64 CSearcher::perftWhite( U16 depthLeft )
{
    if ( depthLeft == 0 )
        return 1;

    CMoves      moves;
    U64         nodeCount = 0;

    mpPos->genLegalMoves( moves ); ...
    for ( U16 moveIx = 0; moveIx = moves.getNumMoves(); moveIx++ )
    {
        CMove move = moves.get( moveIx );
        mpPos->makeMove( move ); ...
        nodeCount += perft( depthLeft - 1 );
        mpPos->unmakeMove( move ); ...
    }
    return nodeCount;
}

///
/// generates the perft node count for the current position.
///
U64 CSearcher::perft( U16 depthLeft )
{
    if ( mpPos->getWhoseMove() == EColor::kWhite )
    {
    }
}
