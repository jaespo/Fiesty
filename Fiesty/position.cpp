///
/// file position.cpp
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with positions
///
///
#include "position.h"

///
/// format the castling rights (fen friendly format)
///
std::string CPosRights::castlingAsStr() const
{
    std::string     s;

    if ( canWhiteOO() )
        s.append( "K" );
    if ( canWhiteOOO() )
        s.append( "Q" );
    if ( canBlackOO() )
        s.append( "k" );
    if ( canBlackOOO() )
        s.append( "q" );
    if ( s.length() == 0 )
        s.append( "-" );
    return s;
}

///
/// convert an UndoContext into a string abbr
///
std::string CUndoContext::asAbbr() const
{
    std::string s = mPieceMoved.asAbbr(); 
    if ( mPieceCaptured.get() != EPiece::kNone )
    {
        s.append( "x" );
        s.append( mPieceCaptured.asAbbr() );
    }
    s.append( " " );
    s.append( mPosRights.asAbbr() );
    return s;
}