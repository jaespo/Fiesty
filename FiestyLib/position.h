/// file position.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// headers having to do with positions
///
///
#ifndef Fiesty_position_h
#define Fiesty_position_h

#include "fiesty.h"
#include "bitboard.h"
#include "piece.h"
#include "move.h"

///
/// Class that represents the casting and en passant rights, an the number of
/// duplicates in a position.
///
class CPosRights
{
public:
    CPosRights() { mRights = kAllCastle; }

    //
    //  Getters
    //
    CFile getEnPassantFile() const { return EFile( mRights & kEnPassantFileMask ); }
    U8 isEnPassantLegal() const 
    { 
        return mRights & kEnPassantLegalMask; 
    }
    U8 canWhiteOO() const       { return mRights & kWhiteOOMask; }
    U8 canWhiteOOO() const      { return mRights & kWhiteOOOMask; }
    U8 canBlackOO() const       { return mRights & kBlackOOMask; }
    U8 canBlackOOO() const      { return mRights & kBlackOOOMask; }

    //
    //  Setters
    //
    void init() { mRights = kAllCastle; }
    void setEnPassantFile( CFile f ) 
    { 
        mRights &= ~ kEnPassantFileMask;
        mRights |= U8( f.get() ) | kEnPassantLegalMask; 
    }
    void clearEnPassantFile( CFile f ) 
    { 
        mRights &= ~ ( kEnPassantFileMask | kEnPassantLegalMask );
        mRights |= U8( f.get() ) | kEnPassantLegalMask; 
    }
    void setWhiteOO()    { mRights |= kWhiteOOMask; }
    void clearWhiteOO()  { mRights &= ~kWhiteOOMask; }
    void setWhiteOOO()   { mRights |= kWhiteOOOMask; }
    void clearWhiteOOO() { mRights &= ~kWhiteOOOMask; }
    void setBlackOO()    { mRights |= kBlackOOMask; }
    void clearBlackOO()  { mRights &= ~kBlackOOMask; }
    void setBlackOOO()   { mRights |= kBlackOOOMask; }
    void clearBlackOOO() { mRights &= ~kBlackOOOMask; }
    void clearCastling() { mRights &= ~kAllCastle; }

    std::string asStr() const;
    std::string asAbbr() const { return asStr(); }
    std::string castlingAsStr() const;

private:
    friend class CTester;

    static const U8 kEnPassantFileMask  = 0x07;
    static const U8 kEnPassantLegalMask = 0x08;
    static const U8 kWhiteOOMask        = 0x10;
    static const U8 kWhiteOOOMask       = 0x20;
    static const U8 kBlackOOMask        = 0x40;
    static const U8 kBlackOOOMask       = 0x80;
    static const U8 kAllCastle          = 0xF0;

    U8          mRights;

    void onWqrMove() { clearWhiteOOO(); }
    void onWkrMove() { clearWhiteOO(); }
    void onBqrMove() { clearBlackOOO(); }
    void onBkrMove() { clearBlackOO(); }
    void onKMove( CColor c ) 
    { 
        if ( c.isWhite() )
            mRights &= ~( kWhiteOOMask | kWhiteOOOMask );
        else
            mRights &= ~( kBlackOOMask | kBlackOOOMask );
    };
};

///
/// Class that represents the information needed to undo a chess after it's 
/// been made
///
class CUndoContext
{
public:
    CPiece getPieceMoved() const { return mPieceMoved; }
    CPiece getPieceCaptured() const { return mPieceCaptured; }
    CPosRights getPosRights() const { return mPosRights; }

    void setPieceMoved( CPiece p ) { mPieceMoved = p; }
    void setPieceCaptured( CPiece p ) { mPieceCaptured = p; }
    void setPosRights( CPosRights pr ) { mPosRights = pr; }
    
    std::string asStr() const { return asAbbr(); };
    std::string asAbbr() const;

private:
    CPiece      mPieceMoved;
    CPiece      mPieceCaptured;
    CPosRights  mPosRights;
};

//
//  Class the represents a chess position
//
class CPos
{
public:
    static const char* kStartFen;

    CPos() { clearBoard(); }

    void clearBoard();
    void addPiece( CPiece p, CSqix sq );
    bool parseFen( 
        const std::string&          sFen,
        std::string&                rsErrorText );
    CColor getWhoseMove() { return mWhoseMove; }
    CPiece getPiece( YSqix sqix ) { return mBoard[sqix]; }

    std::string asAbbr() const { return asFen(); }
    std::string asStr() const;
    std::string asFen() const;
    std::string asDiagram() const;

    void genWhitePawnQuiets( CMoves& rMoves );
    void genWhitePawnCaptures( CMoves& rMoves );
    void genWhiteKnightQuiets( CMoves& rMoves );        //TODO: code me
    void genWhiteKnightCaptures( CMoves& rMoves );      //TODO: code me
    void genWhiteBishopQuiets( CMoves& rMoves );        //TODO: code me
    void genWhiteBishopCaptures( CMoves& rMoves );      //TODO: code me
    void genWhiteRookQuiets( CMoves& rMoves );          //TODO: code me
    void genWhiteRookCaptures( CMoves& rMoves );        //TODO: code me
    void genWhiteQueenQuiets( CMoves& rMoves );         //TODO: code me
    void genWhiteQueenCaptures( CMoves& rMoves );       //TODO: code me
    void genWhiteKingQuiets( CMoves& rMoves );          //TODO: code me    
    void genWhiteKingCaptures( CMoves& rMoves );        //TODO: code me

    void genBlackPawnQuiets( CMoves& rMoves );
    void genBlackPawnCaptures( CMoves& rMoves );
    void genBlackKnightQuiets( CMoves& rMoves );        //TODO: code me
    void genBlackKnightCaptures( CMoves& rMoves );      //TODO: code me
    void genBlackBishopQuiets( CMoves& rMoves );        //TODO: code me
    void genBlackBishopCaptures( CMoves& rMoves );      //TODO: code me
    void genBlackRookQuiets( CMoves& rMoves );          //TODO: code me
    void genBlackRookCaptures( CMoves& rMoves );        //TODO: code me
    void genBlackQueenQuiets( CMoves& rMoves );         //TODO: code me
    void genBlackQueenCaptures( CMoves& rMoves );       //TODO: code me
    void genBlackKingQuiets( CMoves& rMoves );          //TODO: code me    
    void genBlackKingCaptures( CMoves& rMoves );        //TODO: code me

    void genMoves( CMoves& rMoves );                //TODO: code me

    void makeMove( CMove m );                       //TODO: code me
    void unmakeMove( CMove m );                     //TODO: code me

private:
    CColor          mWhoseMove;
    U8              mHalfMoveClock;                 // for 50 move rule
    U8              mDups;                          // for 3 time repetions
    CPosRights      mPosRights;
    std::uint16_t   mMoveNum;
    CPiece          mBoard[U8( ERank::kNum ) * U8( EFile::kNum )];
    CBitBoard       mbbPieceType[EPieceType::kNum];
    CBitBoard       mbbColor[EColor::kNum];

    static std::string nextFenTok( 
        const std::string &sFen, size_t &rPos );

    ///
    /// @returns the bitmask of unoccupied squares in the specified bitboard
    ///
    CBitBoard unoccupied( const CBitBoard& bb ) const 
    {
        return ( bb.get() & ~( mbbColor[U8( EColor::kWhite )].get() 
            | mbbColor[U8( EColor::kBlack )].get() ) );
    }
};

#endif      // position.h