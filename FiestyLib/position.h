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
/// Class that represents the casting and en passant rights, and the number of
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

	void onWqrMove() { clearWhiteOOO(); }
	void onWkrMove() { clearWhiteOO(); }
	void onBqrMove() { clearBlackOOO(); }
	void onBkrMove() { clearBlackOO(); }
	void onKMove(CColor c)
	{
		if (c.isWhite())
			mRights &= ~(kWhiteOOMask | kWhiteOOOMask);
		else
			mRights &= ~(kBlackOOMask | kBlackOOOMask);
	};

private:
    static const U8 kEnPassantFileMask  = 0x07;
    static const U8 kEnPassantLegalMask = 0x08;
    static const U8 kWhiteOOMask        = 0x10;
    static const U8 kWhiteOOOMask       = 0x20;
    static const U8 kBlackOOMask        = 0x40;
    static const U8 kBlackOOOMask       = 0x80;
    static const U8 kAllCastle          = 0xF0;

    U8          mRights;

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
    //
    //  Typedefs and constants
    //
    static const char* kStartFen;
    enum ELegalMoveType { kLegal, kQuasiLegal };

    CPos() { clearBoard(); }

    void clearBoard();
    void addPiece( CPiece p, CSqix sq );
    bool parseFen( 
        const std::string&          sFen,
        std::string&                rsErrorText );
    CBitBoard getCheckers() const { return mbbCheckers; }
    CColor getWhoseMove() const { return mWhoseMove; }
    CPiece getPiece( YSqix sqix ) const { return mBoard[sqix]; }
    CBitBoard getPieces( CColor c, CPieceType pt ) const
    { 
        return ( mbbColor[U8( c.get() )].get() 
            & mbbPieceType[U8( pt.get() )].get() );
    }

    std::string asAbbr() const { return asFen(); }
    std::string asStr() const;
    std::string asFen() const;
    std::string asDiagram() const;

    void genWhitePawnQuiets( CMoves& rMoves );
    void genWhitePawnCaptures( CMoves& rMoves );
    void genWhiteKnightQuiets( CMoves& rMoves );
    void genWhiteKnightCaptures( CMoves& rMoves );
    void genWhiteBishopQuiets( CMoves& rMoves );
    void genWhiteBishopCaptures( CMoves& rMoves );
    void genWhiteRookQuiets( CMoves& rMoves );
    void genWhiteRookCaptures( CMoves& rMoves );
    void genWhiteQueenQuiets( CMoves& rMoves );
    void genWhiteQueenCaptures( CMoves& rMoves );
    void genWhiteKingQuiets( CMoves& rMoves );
    void genWhiteKingCaptures( CMoves& rMoves );

    void genBlackPawnQuiets( CMoves& rMoves );
    void genBlackPawnCaptures( CMoves& rMoves );
    void genBlackKnightQuiets( CMoves& rMoves );
    void genBlackKnightCaptures( CMoves& rMoves );
    void genBlackBishopQuiets( CMoves& rMoves );
    void genBlackBishopCaptures( CMoves& rMoves );
    void genBlackRookQuiets( CMoves& rMoves );
    void genBlackRookCaptures( CMoves& rMoves );
    void genBlackQueenCaptures( CMoves& rMoves );
    void genBlackQueenQuiets( CMoves& rMoves );
    void genBlackKingQuiets( CMoves& rMoves );
    void genBlackKingCaptures( CMoves& rMoves );

    void findWhiteCheckers();
    void findBlackCheckers();

    void genWhiteMoves( CMoves& rMoves );
    void genWhiteLegalMoves( CMoves& rMoves );
    void genBlackMoves( CMoves& rMoves );
    void genBlackLegalMoves( CMoves& rMoves );
    
    void makeMoveForPerft( CMove m, CUndoContext undoContext );
    void unmakeMoveForPerft( CMove m, CUndoContext undoContext);                //TODO: code me

    ///
    /// @returns the bitmask of unoccupied squares in the specified bitboard
    ///
    CBitBoard unoccupied( const CBitBoard& bb ) const 
    {
        return ( bb.get() & ~( mbbColor[U8( EColor::kWhite )].get() 
            | mbbColor[U8( EColor::kBlack )].get() ) );
    }

    ///
    /// @returns the bitmask of occupied squares in the specified bitboard
    ///
    CBitBoard occupied( const CBitBoard& bb ) const 
    {
        return ( bb.get() & ( mbbColor[U8( EColor::kWhite )].get() 
            | mbbColor[U8( EColor::kBlack )].get() ) );
    }

    ///
    /// @returns a non-zero bitboard if the square is occupied by a white
    /// piece.
    ///
    CBitBoard isWhite( CSqix sqix ) const
    {
        return mbbColor[U8( EColor::kWhite )].getSquareBits( sqix.get() );
    }

    ///
    /// @returns a non-zero bitboard if the square is occupied by a black
    /// piece.
    ///
    CBitBoard isBlack( CSqix sqix ) const
    {
        return mbbColor[U8( EColor::kBlack )].getSquareBits( sqix.get() );
    }

private:
    void genBlackBishopCapturesFrom( CMoves& rMoves, CBitBoard bbFrom );
    void genBlackBishopQuietsFrom( CMoves& rMoves, CBitBoard bbFrom );
    void genBlackRookCapturesFrom( CMoves& rMoves, CBitBoard bbFrom );
    void genBlackRookQuietsFrom( CMoves& rMoves, CBitBoard bbFrom );
    void genWhiteBishopCapturesFrom( CMoves& rMoves, CBitBoard bbFrom );
    void genWhiteBishopQuietsFrom( CMoves& rMoves, CBitBoard bbFrom );
    void genWhiteRookCapturesFrom( CMoves& rMoves, CBitBoard bbFrom );
    void genWhiteRookQuietsFrom( CMoves& rMoves, CBitBoard bbFrom );

    void findDiagonalCheckers( CSqix kingSqix, CBitBoard bbBishopsAndQueens );
    void findRankAndFileCheckers( CSqix kingSqix, CBitBoard bbRooksAndQueens );

    void findWhiteKnightCheckers( CSqix kingSqix );
    void findWhiteRankAndFileCheckers( CSqix kingSqix );
    void findWhiteDiagonalCheckers( CSqix kingSqix );
    void findWhitePawnCheckers( CBitBoard bbKing );
    void findWhiteKingCheckers( CSqix kingSqix );

    void findBlackKnightCheckers( CSqix kingSqix );
    void findBlackRankAndFileCheckers( CSqix kingSqix );
    void findBlackDiagonalCheckers( CSqix kingSqix );
    void findBlackPawnCheckers( CBitBoard bbKing ); 
    void findBlackKingCheckers( CSqix kingSqix );
	void updatePosRights( CMove m );

    CColor          mWhoseMove;
    U8              mHalfMoveClock;                 // for 50 move rule
    U8              mDups;                          // for 3 time repetitions
    CPosRights      mPosRights;
    std::uint16_t   mMoveNum;
    CPiece          mBoard[U8( ERank::kNum ) * U8( EFile::kNum )];
    CBitBoard       mbbPieceType[U8( EPieceType::kNum )];
    CBitBoard       mbbColor[U8( EColor::kNum )];
    CBitBoard       mbbCheckers;

    static std::string nextFenTok( 
        const std::string &sFen, size_t &rPos );

};

#endif      // position.h