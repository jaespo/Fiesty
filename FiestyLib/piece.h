/// file piece.h
///
///  Fiesty (C) 2014 by Jeffery A Esposito
///
///  header having to do with pieces.
///
///
#ifndef Fiesty_piece_h
#define Fiesty_piece_h
#include "fiesty.h"

///
/// Class for a color
///
class CColor
{
public:
    CColor() {}
    CColor( EColor c ) 
    { 
        mColor = c; 
        assert( isValid() );
    } 
    bool operator==( CColor c ) { return c.mColor == mColor; }
    EColor get() const { return mColor; }
    bool isValid() const { return mColor < EColor::kNum; }
    bool isWhite() const { return mColor == EColor::kWhite; }
    bool isBlack() const { return mColor == EColor::kBlack; }
    CColor getOpponent() const { return EColor( 1 - U8( mColor ) ); }
    static EColor parseColor( std::string s );

    std::string asStr() const;
    std::string asAbbr() const;

private:
    EColor                      mColor;
    static const char*          kStrs[EColor::kNum];
    static const char*          kAbbrs[EColor::kNum];
};  // of class CColor

///
/// ostream insertion operator for an EColor
///
inline std::ostream& operator<<( std::ostream& os, CColor c )
    { return os << c.asStr(); }

///
/// Class for a type of piece
///
class CPieceType
{
public:
    CPieceType(){}
    CPieceType( EPieceType pt ) 
    { 
        mPieceType = pt; 
        assert( isValid() );
    }
    EPieceType get() const { return mPieceType; }
    bool isValid() const { return mPieceType <= EPieceType::kNum; }

    std::string asStr() const;
    std::string asAbbr() const;

private:
    EPieceType                  mPieceType;
    static const char*          kStrs[U8( EPieceType::kNum )];
    static const char*          kAbbrs[U8( EPieceType::kNum )];
};

///
/// ostream insertion operator for an EColor
///
inline std::ostream& operator<<( std::ostream& os, CPieceType pt )
{
    return os << pt.asStr();
}

///
/// Class for a piece, including color
///
class CPiece
{
public:
    CPiece(){}
    CPiece( EPiece p ) 
    { 
        mPiece = p; 
        assert( isValid() );
    }
    CPiece( CColor c, CPieceType pt ) 
    { 
        assert( c.isValid() );
        assert( pt.isValid() );
        mPiece = EPiece( 
            U8( c.get() ) * U8( EPieceType::kNum ) + U8( pt.get() ) );
    }
    EPiece get() const { return mPiece; }
    bool isValid() const { return mPiece < EPiece::kNum; }

    CPieceType getPieceType() const
        { return EPieceType( U8( mPiece ) % U8( EPieceType::kNum ) ); }
    CColor getColor() const
        { return EColor( ( mPiece > EPiece::kWhiteKing ) & 1  ); }

    std::string asStr() const;       
    std::string asAbbr() const;

    static EPiece parsePiece( const std::string& s );

private:
    EPiece                      mPiece;
    static const char*          kAbbrs[EPiece::kNum];
};

///
/// ostream insertion operator for an EPiece
///
inline std::ostream& operator<<( std::ostream& os, const CPiece c )
{
    return os << c.asStr();
}

#endif  // piece.h