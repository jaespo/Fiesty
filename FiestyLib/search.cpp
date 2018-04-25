/// file searchC.cpp
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with searches
///
///
#include "search.h"

///
/// Generates the perft node count for the current position with black to
/// to move.
///
U64 CSearcher::perftBlack( U16 depthLeft )
{
    if ( depthLeft == 0 )
        return 1;

    CMoves			moves;
    U64				nodeCount = 0;
	CUndoContext	undoContext;

    mpPos->genBlackMoves( moves );
    for ( U16 moveIx = 0; moveIx = moves.getNumMoves(); moveIx++ )
    {
        CMove move = moves.get( moveIx );
        mpPos->makeMoveForPerft( move );

        //
        //  If the move leaves us in check, it's not legal.  The
        //  can be removed when genBlackMoves implements check evasion.
        //
        mpPos->findWhiteCheckers();
        if ( mpPos->getCheckers().get() )
        {
            if ( depthLeft == 1 ) 
                nodeCount++;
            else
                nodeCount += perftBlack( depthLeft - 1 );
        }
        mpPos->unmakeMove( move );
    }
    return nodeCount;
}

///
/// Generates the perft node count for the current position with white to
/// to move.
///
U64 CSearcher::perftWhite( U16 depthLeft )
{
    if ( depthLeft == 0 )
        return 1;

    CMoves      moves;
    U64         nodeCount = 0;

    mpPos->genWhiteMoves( moves );
    for ( U16 moveIx = 0; moveIx = moves.getNumMoves(); moveIx++ )
    {
        CMove move = moves.get( moveIx );
        mpPos->makeMove( move );

        //
        //  If the move leaves us in check, it's not legal.  The
        //  can be removed when genWhiteMoves implements check evasion.
        //
        mpPos->findBlackCheckers();
        if ( mpPos->getCheckers().get() )
        {
            if ( depthLeft == 1 ) 
                nodeCount++;
            else
                nodeCount += perftBlack( depthLeft - 1 );
        }
        mpPos->unmakeMove( move );
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
        return perftWhite( depthLeft );
    }
    else 
    {
        return perftBlack( depthLeft );
    }
}
