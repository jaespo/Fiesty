/// file piece.cpp
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with pieces.
///
///
#include "piece.h"

//
//  statics
//
const char* CColor::kStrs[EColor::kNum] = { "White", "Black" };
const char* CColor::kAbbrs[EColor::kNum] = { "W", "B" };
const char* CPieceType::kStrs[EPieceType::kNum] = 
    { "Pawn", "Knight", "Bishop", "Rook", "Queen", "King" };
const char* CPieceType::kAbbrs[EPieceType::kNum] = 
    { "P", "N", "B", "R", "Q", "K" };
const char* CPiece::kAbbrs[EPiece::kNum] = 
    { "P", "N", "B", "R", "Q", "K", "p", "n", "b", "r", "q", "k" };

///
/// Abbreviation string representation for a color
///
std::string CColor::asAbbr() const
{
    return std::string( kAbbrs[U8( mColor )] );
}

///
/// String representation for a color
///
std::string CColor::asStr() const
{
    return std::string( kStrs[ U8( mColor )] );
}

///
/// Abbreviation representation for a piece
///
std::string CPiece::asAbbr() const
{
    return std::string( kAbbrs[U8( mPiece )] );
};

///
/// String representation for a piece
///
std::string CPiece::asStr() const
{
    return getColor().asStr() + " " + getPieceType().asStr();
};

///
/// Abbreviation representation for a piece type
///
std::string CPieceType::asAbbr() const
{
    return std::string( kAbbrs[U8( mPieceType )] );
}
    
///
///  Display a piece type as a string
///
std::string CPieceType::asStr() const
{
    return std::string( kStrs[U8( mPieceType )] );
}
