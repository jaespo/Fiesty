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
