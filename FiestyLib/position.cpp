/// file position.cpp
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with positions
///
///
#include <cctype>
#include <algorithm>
#include "position.h"
#include "gen.h"

const char* CPos::kStartFen 
    = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

///
/// Add a piece to the board.
///
void CPos::addPiece( CPiece p, CSqix sq )
{
    mBoard[sq.get()] = p;
    mbbPieceType[U8( p.getPieceType().get() )] |= sq.asBitBoard();
    mbbColor[U8( p.getColor().get() )] |= sq.asBitBoard();
}

///
/// String reprentation of a position as a pretty-printable diagram
///
std::string CPos::asDiagram() const
{
    std::string         s;

    for ( S8 r = S8( ERank::kRank8 ); r >= S8( ERank::kRank1 ); r-- )
    {
        for ( U8 f = U8( EFile::kFileA ); f <= U8( EFile::kFileH ); f++ )
        {
            CPiece piece =  mBoard[CSqix( ERank( r ), EFile( f ) ).get()];
            if ( piece.get() == EPiece::kNone )
            {
                s.append( "." );
            }
            else
            {
                s.append( 
                    mBoard[CSqix( ERank( r ), EFile( f ) ).get()].asAbbr() );
            }
            if ( EFile( f ) != EFile::kFileH )
                s.append( " " );
        }
        s.append( "\n" );
    }
    s.append( 
          "whoseMove = " + mWhoseMove.asAbbr()
        + "\nrights    = " + mPosRights.asAbbr() 
        + "\nmove      = " + std::to_string( mMoveNum ) 
        + "\n50-move   = " + std::to_string( mHalfMoveClock ) 
        + "\ndups      = " + std::to_string( mDups ) + "\n" );
    return s;
}

///
/// Convert a position to a fen string, for example
///
///     rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
///
std::string CPos::asFen() const
{
    std::string             s;

    for ( S8 r = S8( ERank::kRank8 ); r >= S8( ERank::kRank1 ); r-- )
    {
        int spaceCount = 0;
        for ( U8 f = U8( EFile::kFileA ); f <= U8( EFile::kFileH ); f++ )
        {
            CPiece piece =  mBoard[CSqix( ERank( r ), EFile( f ) ).get()];
            if ( piece.get() == EPiece::kNone )
            {
                spaceCount++;
            }
            else
            {
                if ( spaceCount > 0 )
                    s.push_back( '0' + spaceCount );
                s.append( 
                    mBoard[CSqix( ERank( r ), EFile( f ) ).get()].asAbbr() );
            }
        }
        if ( spaceCount > 0 )
            s.push_back( '0' + spaceCount );
        if ( r != S8( ERank::kRank1 ) )
            s.append( "/" );
    }
    s.append( " " );
    s.push_back( std::tolower( mWhoseMove.asAbbr()[0] ) );
    s.append( " " + mPosRights.asAbbr() 
        + " " + std::to_string( mHalfMoveClock ) 
        + " " + std::to_string( mMoveNum ) );
    return s;
}

///
/// Position as a fen, including the bitmaps
///
std::string CPos::asStr() const
{
    std::string s = asFen() + "\n";
    for ( U8 pt = U8( EPieceType::kPawn ); pt < U8( EPieceType::kNum ); pt++ )
    {
        s.append( CPieceType( EPieceType( pt ) ).asAbbr() + ": " );
        s.append( mbbPieceType[pt].asStr() + "\n" );
    }
    for ( U8 c = U8( EColor::kWhite ); c <= U8( EColor::kBlack ); c++ )
    {
        s.append( CColor( EColor( c ) ).asAbbr() + ": " );
        s.append( mbbColor[c].asStr() + "\n" );
    }
    return s;
}

///
/// Clear the board of pieces
///
void CPos::clearBoard()
{
    CPiece* pPiece = mBoard;

    mWhoseMove = EColor::kWhite;
    mHalfMoveClock = 0;
    mDups = 0;
    mPosRights.init();
    mMoveNum = 0;
    std::memset( 
        mBoard, U8( EPiece::kNone ), U8( ERank::kNum ) * U8( EFile::kNum ) );
    std::memset( mbbPieceType, 0, sizeof( mbbPieceType ) );
    std::memset( mbbColor, 0, sizeof( mbbColor ) );
}

///
/// finds the black pieces giving check to the white king 
/// and saves them in mbbCheckers.
///
void CPos::findBlackCheckers()
{
    mbbCheckers = 0ULL;

    CBitBoard bbWhiteKing = getPieces( EColor::kWhite, EPieceType::kKing );
    CSqix kingSqix = bbWhiteKing.msb();
    findBlackRankAndFileCheckers( kingSqix );
    findBlackDiagonalCheckers( kingSqix );
    if ( mbbCheckers.popcnt() < 2 )
        findBlackKnightCheckers( kingSqix );
    if ( mbbCheckers.popcnt() < 2 )
        findBlackPawnCheckers( bbWhiteKing );
    if ( mbbCheckers.popcnt() < 2 )
        findBlackKingCheckers( kingSqix );
}

///
/// finds black checkers along the diagonals giving check to
/// the white king.
///
void CPos::findBlackDiagonalCheckers( CSqix kingSqix )
{
	CBitBoard bbBishopsAndQueens 
		= getPieces( EColor::kBlack, EPieceType::kBishop ).get()
		| getPieces( EColor::kBlack, EPieceType::kQueen ).get();
    findDiagonalCheckers( kingSqix, bbBishopsAndQueens );
}

///
/// finds black kings giving check to the white king.  This is needed because
/// perft checks my legal moves by trying the move and seeing if it leaves 
/// the king in check.
///
void CPos::findBlackKingCheckers( CSqix whiteKingSqix )
{
    CBitBoard bbBlackKing = getPieces( EColor::kBlack, EPieceType::kKing );
    mbbCheckers 
        |= ( CGen::mbbKingAttacks[whiteKingSqix.get()] & bbBlackKing.get() );
}

///
/// finds black knights giving check to the white king
///
void CPos::findBlackKnightCheckers( CSqix kingSqix )
{
    CBitBoard bbKnights = getPieces( EColor::kBlack, EPieceType::kKnight );
    mbbCheckers |= ( CGen::mbbKnightAttacks[kingSqix.get()] & bbKnights.get() );
}

///
/// finds black pawn checkers that give check to the white king.
///
void CPos::findBlackPawnCheckers( CBitBoard bbWhiteKing )
{

    CBitBoard bbPawns = getPieces( EColor::kBlack, EPieceType::kPawn );
    CBitBoard bbKingUp1 = bbWhiteKing.advanceRanks( 1 );
    CBitBoard bbCheckers = bbPawns.get() 
        & ( bbKingUp1.leftFiles( 1 ).get() | bbKingUp1.rightFiles( 1 ).get() );
    mbbCheckers |= bbCheckers;
}

///
/// finds black checkers along the rank and file giving check to
/// the white king.
///
void CPos::findBlackRankAndFileCheckers( CSqix kingSqix )
{
	CBitBoard bbRooksAndQueens 
		= getPieces( EColor::kBlack, EPieceType::kRook ).get()
		| getPieces( EColor::kBlack, EPieceType::kQueen ).get();
    findRankAndFileCheckers( kingSqix, bbRooksAndQueens );
}

///
/// finds the checker contained in the bbBishopssAndQueens bitboard.  This 
/// method is called from each corresponding color specific method.
///
void CPos::findDiagonalCheckers( 
    CSqix           kingSqix, 
    CBitBoard       bbBishopsAndQueens )
{
	//
    //  Get the rays from the king's position to the edge of the board in
    //  each of the four directions.  (The kings's square is not included
    //  in these rays.)
    //

    //
    //  NorthEast
    //
    CBitBoard bbNorthEastRay 
        = CGen::mbbBishopRays[kingSqix.get()].mbbNorthEast;
	CBitBoard bbOccRay = occupied( bbNorthEastRay );
    if ( bbOccRay.get() )
    {
        CSqix toSqix = bbOccRay.lsb().get();
		CBitBoard bbChecker = toSqix.asBitBoard() & bbBishopsAndQueens.get();
		if ( bbChecker.get() )
		{
			mbbCheckers |= bbChecker;
			return;
		}
    }

    //
    //  NorthWest
    //
    CBitBoard bbNorthWestRay 
        = CGen::mbbBishopRays[kingSqix.get()].mbbNorthWest;
    if ( ( bbOccRay = occupied( bbNorthWestRay ) ).get() )
    {
        CSqix toSqix = bbOccRay.lsb().get();
		CBitBoard bbChecker = toSqix.asBitBoard() & bbBishopsAndQueens.get();
		if ( bbChecker.get() )
		{
			mbbCheckers |= bbChecker;
			return;
		}
    }

    //
    //  SouthEast
    //
    CBitBoard bbSouthEastRay 
        = CGen::mbbBishopRays[kingSqix.get()].mbbSouthEast;
    if ( ( bbOccRay = occupied( bbSouthEastRay ) ).get() )
    {
        CSqix toSqix = bbOccRay.msb().get();
		CBitBoard bbChecker = toSqix.asBitBoard() & bbBishopsAndQueens.get();
		if ( bbChecker.get() )
		{
			mbbCheckers |= bbChecker;
			return;
		}
    }

    //
    //  SouthWest
    //
    CBitBoard bbSouthWestRay 
        = CGen::mbbBishopRays[kingSqix.get()].mbbSouthWest;
    if ( ( bbOccRay = occupied( bbSouthWestRay ) ).get() )
    {
        CSqix toSqix = bbOccRay.msb().get();
		CBitBoard bbChecker = toSqix.asBitBoard() & bbBishopsAndQueens.get();
		if ( bbChecker.get() )
		{
			mbbCheckers |= bbChecker;
			return;
		}
    }
}

///
/// finds the checker contained in the bbRooksAndQueens bitboard.  This method
/// is called from each corresponding color specific method.
///
void CPos::findRankAndFileCheckers( 
    CSqix           kingSqix, 
    CBitBoard       bbRooksAndQueens )
{
	//
    //  Get the rays from the king's position to the edge of the board in
    //  each of the four directions.  (The kings's square is not included
    //  in these rays.)
    //
    CBitBoard bbSouthRay = CGen::mbbRookRays[kingSqix.get()].mbbSouth;
	CBitBoard bbOccRay = occupied( bbSouthRay );
    if ( bbOccRay.get() )
    {
		CSqix toSqix = bbOccRay.msb().get();
		CBitBoard bbChecker = toSqix.asBitBoard() & bbRooksAndQueens.get();
		if ( bbChecker.get() )
		{
			mbbCheckers |= bbChecker;
			return;
		}
    }
    CBitBoard bbEastRay = CGen::mbbRookRays[kingSqix.get()].mbbEast;
    if ( ( bbOccRay = occupied( bbEastRay ) ).get() )
    {
        CSqix toSqix = bbOccRay.lsb().get();
		CBitBoard bbChecker = toSqix.asBitBoard() & bbRooksAndQueens.get();
		if ( bbChecker.get() )
		{
			mbbCheckers |= bbChecker;
			return;
		}
    }
    CBitBoard bbWestRay = CGen::mbbRookRays[kingSqix.get()].mbbWest;
    if ( ( bbOccRay = occupied( bbWestRay ) ).get() )
    {
        CSqix toSqix = bbOccRay.msb().get();
		CBitBoard bbChecker = toSqix.asBitBoard() & bbRooksAndQueens.get();
		if ( bbChecker.get() )
		{
			mbbCheckers |= bbChecker;
			return;
		}
    }
    CBitBoard bbNorthRay = CGen::mbbRookRays[kingSqix.get()].mbbNorth;
    if ( ( bbOccRay = occupied( bbNorthRay ) ).get() )
    {
        CSqix toSqix = bbOccRay.lsb().get();
		CBitBoard bbChecker = toSqix.asBitBoard() & bbRooksAndQueens.get();
		if ( bbChecker.get() )
		{
			mbbCheckers |= bbChecker;
			return;
		}
    }
}

///
/// finds the white pieces giving check to the black king 
/// and saves them in mbbCheckers.
///
void CPos::findWhiteCheckers()
{
    CBitBoard bbKing = getPieces( EColor::kBlack, EPieceType::kKing );
    CSqix kingSqix = bbKing.msb();
    mbbCheckers = 0ULL;
    findWhiteRankAndFileCheckers( kingSqix );
    findWhiteDiagonalCheckers( kingSqix );
    if ( mbbCheckers.popcnt() < 2 )
        findWhiteKnightCheckers( kingSqix );
    if ( mbbCheckers.popcnt() < 2 )
        findWhitePawnCheckers( bbKing );
    if ( mbbCheckers.popcnt() < 2 )
        findWhiteKingCheckers( kingSqix );

}

///
/// finds white checkers along the diagonals giving check to
/// the black king.
///
void CPos::findWhiteDiagonalCheckers( CSqix kingSqix )
{
	CBitBoard bbBishopsAndQueens 
		= getPieces( EColor::kWhite, EPieceType::kBishop ).get()
		| getPieces( EColor::kWhite, EPieceType::kQueen ).get();
    findDiagonalCheckers( kingSqix, bbBishopsAndQueens );
}

///
/// finds white kings giving check to the black king.  This is needed because
/// perft checks my legal moves by trying the move and seeing if it leaves 
/// the king in check.
///
void CPos::findWhiteKingCheckers( CSqix blackKingSqix )
{
    CBitBoard bbWhiteKing = getPieces( EColor::kWhite, EPieceType::kKing );
    mbbCheckers 
        |= ( CGen::mbbKingAttacks[blackKingSqix.get()] & bbWhiteKing.get() );
}

///
/// finds white knights giving check to the black king
///
void CPos::findWhiteKnightCheckers( CSqix kingSqix )
{
    CBitBoard bbKnights = getPieces( EColor::kWhite, EPieceType::kKnight );
    mbbCheckers |= ( CGen::mbbKnightAttacks[kingSqix.get()] & bbKnights.get() );
}

///
/// finds white pawn checkers that give check to the black king.
///
void CPos::findWhitePawnCheckers( CBitBoard bbBlackKing )
{

    CBitBoard bbPawns = getPieces( EColor::kWhite, EPieceType::kPawn );
    CBitBoard bbKingDown1 = bbBlackKing.retreatRanks( 1 );
    CBitBoard bbCheckers = bbPawns.get() 
        & ( bbKingDown1.leftFiles( 1 ).get() 
            | bbKingDown1.rightFiles( 1 ).get() );
    mbbCheckers |= bbCheckers;
}

///
/// finds white checkers along the rank and file giving check to
/// the black king.
///
void CPos::findWhiteRankAndFileCheckers( CSqix kingSqix )
{
	CBitBoard bbRooksAndQueens 
		= getPieces( EColor::kWhite, EPieceType::kRook ).get()
		| getPieces( EColor::kWhite, EPieceType::kQueen ).get();
    findRankAndFileCheckers( kingSqix, bbRooksAndQueens );
}

///
/// generates all black quasi-legal moves in the position
///
void CPos::genBlackMoves( CMoves& rMoves )
{
    genBlackPawnQuiets( rMoves );
    genBlackPawnCaptures( rMoves );
    genBlackKnightQuiets( rMoves );
    genBlackKnightCaptures( rMoves );
    genBlackBishopQuiets( rMoves );
    genBlackBishopCaptures( rMoves );
    genBlackRookQuiets( rMoves );
    genBlackRookCaptures( rMoves );
    genBlackQueenQuiets( rMoves );
    genBlackQueenCaptures( rMoves );
    genBlackKingQuiets( rMoves );
    genBlackKingCaptures( rMoves );
}

///
/// generates all black legal moves in the position
///
void CPos::genBlackLegalMoves( CMoves& rMoves )
{
    CMoves			quasiMoves;
	CUndoContext	undoContext;

    genBlackMoves( quasiMoves );
    for ( int moveIx = 0; moveIx < quasiMoves.getNumMoves(); moveIx++ )
    {
        makeMoveForPerft( quasiMoves.get( moveIx ), undoContext );
        if ( !getCheckers().get() )
        {
            rMoves.addMove( quasiMoves.get( moveIx ) );
        }
        unmakeMoveForPerft( quasiMoves.get( moveIx ), undoContext );
    }
}

///
/// generates bishop captures for black
///
/// @param rMoves
///     the bishop captures will be added to rMoves
///
void CPos::genBlackBishopCaptures( CMoves& rMoves )
{
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kBishop );
    genBlackBishopCapturesFrom( rMoves, bbFrom );
}

///
/// generates bishop captures for black
///
/// @param rMoves
///     the bishop captures will be added to rMoves
///
void CPos::genBlackBishopCapturesFrom( CMoves& rMoves, CBitBoard bbFrom )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CSqix           blockerSqix;
    CBitBoard       bbOccRay;
   
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popLsb(); 

        //
        //  Get the rays from the rook position to the edge of the board in
        //  each of the four directions.  (The rook's square is not included
        //  in these rays.
        //
        CBitBoard bbNorthEastRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbNorthEast;
        CBitBoard bbSouthEastRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbSouthEast;
        CBitBoard bbSouthWestRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbSouthWest;
        CBitBoard bbNorthWestRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbNorthWest;
        

        //
        //  Find the square of the blocking piece in each direction, and if 
        //  it's black, generate a capture
        //
        if ( ( bbOccRay = occupied( bbNorthEastRay ) ).get() )
        {
            toSqix = bbOccRay.lsb().get();
            if ( isWhite( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbNorthWestRay ) ).get() )
        {
            toSqix = bbOccRay.lsb().get();
            if ( isWhite( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbSouthEastRay ) ).get() )
        {
            toSqix = bbOccRay.msb().get();
            if ( isWhite( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbSouthWestRay ) ).get() )
        {
            toSqix = bbOccRay.msb().get();
            if ( isWhite( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates bishop non-captures for black
///
/// @param rMoves
///     the bishop moves will be added to rMoves
///
void CPos::genBlackBishopQuiets( CMoves& rMoves )
{
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kBishop );
    genBlackBishopQuietsFrom( rMoves, bbFrom );
}

///
/// generates bishop non-captures for black
///
/// @param rMoves
///     the bishop moves will be added to rMoves
///
void CPos::genBlackBishopQuietsFrom( CMoves& rMoves, CBitBoard bbFrom )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CSqix           blockerSqix;
    CBitBoard       bbOccRay;
   
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popLsb(); 

        //
        //  Get the rays from the bishop position to the edge of the board in
        //  each of the four directions.  (The bishop's square is not included
        //  in these rays.)
        //
        CBitBoard bbNorthEastRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbNorthEast;
        CBitBoard bbSouthEastRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbSouthEast;
        CBitBoard bbSouthWestRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbSouthWest;
        CBitBoard bbNorthWestRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbNorthWest;

        //
        //  For each of thes rays, find the blocker (if any) and mask off
        //  the squares beyond the blocker.
        //
        if ( ( bbOccRay = occupied( bbNorthEastRay ) ).get() )
        {
            bbNorthEastRay 
                &= CGen::mbbBishopRays[bbOccRay.lsb().get()].mbbSouthWest;
        }
        if ( ( bbOccRay = occupied( bbSouthEastRay ) ).get() )
        {
            bbSouthEastRay 
                &= CGen::mbbBishopRays[bbOccRay.msb().get()].mbbNorthWest;
        }
        if ( ( bbOccRay = occupied( bbSouthWestRay ) ).get() )
        {
            bbSouthWestRay 
                &= CGen::mbbBishopRays[bbOccRay.msb().get()].mbbNorthEast;
        }
        if ( ( bbOccRay = occupied( bbNorthWestRay ) ).get() )
        {
            bbNorthWestRay 
                &= CGen::mbbBishopRays[bbOccRay.lsb().get()].mbbSouthEast;
        }
        CBitBoard bbTo 
            = bbNorthEastRay.get() 
            | bbSouthEastRay.get() 
            | bbSouthWestRay.get() 
            | bbNorthWestRay.get();
        while ( bbTo.get() )
        {
            toSqix = bbTo.popLsb(); 
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates knight non-captures for black
///
/// @param rMoves
///     the knight moves will be added to rMoves
///
void CPos::genBlackKnightQuiets( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kKnight );
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popLsb(); 
        CBitBoard bbTo = unoccupied( CGen::mbbKnightAttacks[fromSqix.get()] );
        while ( bbTo.get() )
        {
            toSqix = bbTo.popLsb(); 
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates king captures for black
///
/// @param rMoves
///     the king captures will be added to rMoves
///
void CPos::genBlackKingCaptures( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kKing );
    fromSqix = bbFrom.lsb(); 
    CBitBoard bbTo = CGen::mbbKingAttacks[fromSqix.get()] 
        & mbbColor[U8( EColor::kWhite )].get();
    while ( bbTo.get() )
    {
        toSqix = bbTo.popLsb(); 
        rMoves.addMove( CMove( fromSqix, toSqix ) );
    }
}

///
/// generates king non-captures for white
///
/// @param rMoves
///     the king moves will be added to rMoves
///
void CPos::genBlackKingQuiets( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kKing );
    fromSqix = bbFrom.lsb(); 
    CBitBoard bbTo = unoccupied( CGen::mbbKingAttacks[fromSqix.get()] );
    while ( bbTo.get() )
    {
        toSqix = bbTo.popLsb(); 
        rMoves.addMove( CMove( fromSqix, toSqix ) );
    }
}


///
/// generates knight captures for black
///
/// @param rMoves
///     the knight captures will be added to rMoves
///
void CPos::genBlackKnightCaptures( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kKnight );
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popLsb(); 
        CBitBoard bbTo = CGen::mbbKnightAttacks[fromSqix.get()] 
            & mbbColor[U8( EColor::kWhite )].get();
        while ( bbTo.get() )
        {
            toSqix = bbTo.popLsb(); 
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates pawn captures for black
///
/// @param rMoves
///     the pawn captures will be added to rMoves
///
void CPos::genBlackPawnCaptures( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kPawn );

    //
    //  We can capture on squares that have a white piece or that have just
    //  passed by (for an en-passant).
    //
    CBitBoard bbTargets = mbbColor[U8( EColor::kWhite )];
    if ( mPosRights.isEnPassantLegal() )
    {
        bbTargets.setSquare( 
            CSqix( ERank::kRank3, mPosRights.getEnPassantFile() ).get() );
    }
    CBitBoard bbDown1 = bbFrom.retreatRanks( 1 );
    CBitBoard bbTo = bbTargets.get() 
        & ( bbDown1.leftFiles( 1 ).get() | bbDown1.rightFiles( 1 ).get() );
    while ( bbTo.get() )
    {
        toSqix = bbTo.popLsb();

        //
        //  Is there a capturing pawn to the left of the target?
        //
        if ( toSqix.getFile().get() != EFile::kFileA )
        {
            fromSqix = toSqix.plusRanks( 1 ).minusFiles( 1 );
            if ( bbFrom.atSquare( fromSqix ).get() )
            {
                if ( toSqix.getRank().get() == ERank::kRank1 )
                {
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kQueen ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kRook ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kBishop ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kKnight ) );
                }
                else
                {
                    rMoves.addMove( CMove( fromSqix, toSqix ) );
                }
            }
        }

        //
        //  Is there a capturing pawn to the right of the target?
        //
        if ( toSqix.getFile().get() != EFile::kFileH )
        {
            fromSqix = toSqix.plusRanks( 1 ).plusFiles( 1 );
            if ( bbFrom.atSquare( fromSqix ).get() )
            {
                if ( toSqix.getRank().get() == ERank::kRank1 )
                {
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kQueen ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kRook ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kBishop ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kKnight ) );
                }
                else
                {
                    rMoves.addMove( CMove( fromSqix, toSqix ) );
                }
            }
        }
    }
}

///
/// generates pawn pushes for black
///
/// @param rMoves
///     the pawn moves will be added to rMoves
///
void CPos::genBlackPawnQuiets( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CBitBoard       bbTo;
   
    //
    //  Start with the single pushes
    //
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kPawn );
    CBitBoard bbPop = bbTo = unoccupied( bbFrom.retreatRanks( 1 ) );
    while ( bbPop.get() )
    {
        toSqix = bbPop.popLsb();
        fromSqix = toSqix.plusRanks( 1 );
        if ( toSqix.getRank().get() == ERank::kRank1 )
        {
            rMoves.addMove( CMove( fromSqix, toSqix, EPieceType::kQueen ) );
            rMoves.addMove( CMove( fromSqix, toSqix, EPieceType::kRook ) );
            rMoves.addMove( CMove( fromSqix, toSqix, EPieceType::kBishop ) );
            rMoves.addMove( CMove( fromSqix, toSqix, EPieceType::kKnight ) );
        }
        else
        {
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }

    //
    //  Push forward a second level if we are on the third rand.
    //
    bbPop = unoccupied( bbTo.onRank( ERank::kRank6 ).retreatRanks( 1 ) );
    while ( bbPop.get() )
    {
        toSqix = bbPop.popLsb();
        fromSqix = toSqix.plusRanks( 2 );
        rMoves.addMove( CMove( fromSqix, toSqix ) );
    }
}

///
/// generates rook captures for black
///
/// @param rMoves
///     the rook captures will be added to rMoves
///
void CPos::genBlackRookCaptures( CMoves& rMoves )
{
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kRook );
    genBlackRookCapturesFrom( rMoves, bbFrom );
}

///
/// generates rook captures for black from a set of squares
///
/// @param rMoves
///     the rook captures will be added to rMoves
///
void CPos::genBlackRookCapturesFrom( CMoves& rMoves, CBitBoard bbFrom )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CSqix           blockerSqix;
    CBitBoard       bbOccRay;
   
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popLsb(); 

        //
        //  Get the rays from the rook position to the edge of the board in
        //  each of the four directions.  (The rook's square is not included
        //  in these rays.
        //
        CBitBoard bbNorthRay = CGen::mbbRookRays[fromSqix.get()].mbbNorth;
        CBitBoard bbEastRay = CGen::mbbRookRays[fromSqix.get()].mbbEast;
        CBitBoard bbSouthRay = CGen::mbbRookRays[fromSqix.get()].mbbSouth;
        CBitBoard bbWestRay = CGen::mbbRookRays[fromSqix.get()].mbbWest;

        //
        //  Find the square of the blocking piece in each direction, and if 
        //  it's black, generate a capture
        //
        if ( ( bbOccRay = occupied( bbSouthRay ) ).get() )
        {
            toSqix = bbOccRay.msb().get();
            if ( isWhite( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbEastRay ) ).get() )
        {
            toSqix = bbOccRay.lsb().get();
            if ( isWhite( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbWestRay ) ).get() )
        {
            toSqix = bbOccRay.msb().get();
            if ( isWhite( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbNorthRay ) ).get() )
        {
            toSqix = bbOccRay.lsb().get();
            if ( isWhite( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates captures for the black queen.
///
/// @param rMoves
///     the queen captures will be added to rMoves
///
void CPos::genBlackQueenCaptures( CMoves& rMoves )
{
    genBlackRookCapturesFrom( 
        rMoves, getPieces( EColor::kBlack, EPieceType::kQueen ) );
    genBlackBishopCapturesFrom( 
        rMoves, getPieces( EColor::kBlack, EPieceType::kQueen ) );
}

///
/// generates quiet moves for the black queen.
///
/// @param rMoves
///     the queen moves will be added to rMoves
///
void CPos::genBlackQueenQuiets( CMoves& rMoves )
{
    genBlackRookQuietsFrom( 
        rMoves, getPieces( EColor::kBlack, EPieceType::kQueen ) );
    genBlackBishopQuietsFrom( 
        rMoves, getPieces( EColor::kBlack, EPieceType::kQueen ) );
}

///
/// generates rook non-captures for black
///
/// @param rMoves
///     the rook moves will be added to rMoves
///
void CPos::genBlackRookQuiets( CMoves& rMoves )
{
    CBitBoard bbFrom = getPieces( EColor::kBlack, EPieceType::kRook );
    genBlackRookQuietsFrom( rMoves, bbFrom );
}

///
/// generates rook non-captures for black
///
/// @param rMoves
///     the rook moves will be added to rMoves
///
void CPos::genBlackRookQuietsFrom( CMoves& rMoves, CBitBoard bbFrom )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CSqix           blockerSqix;
    CBitBoard       bbOccRay;
   
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popLsb(); 

        //
        //  Get the rays from the rook position to the edge of the board in
        //  each of the four directions.  (The rook's square is not included
        //  in these rays.
        //
        CBitBoard bbNorthRay = CGen::mbbRookRays[fromSqix.get()].mbbNorth;
        CBitBoard bbEastRay = CGen::mbbRookRays[fromSqix.get()].mbbEast;
        CBitBoard bbSouthRay = CGen::mbbRookRays[fromSqix.get()].mbbSouth;
        CBitBoard bbWestRay = CGen::mbbRookRays[fromSqix.get()].mbbWest;

        //
        //  For each of thes rays, find the blocker (if any) and mask off
        //  the squares beyond the blocker.
        //
        if ( ( bbOccRay = occupied( bbSouthRay ) ).get() )
            bbSouthRay &= CGen::mbbRookRays[bbOccRay.msb().get()].mbbNorth;
        if ( ( bbOccRay = occupied( bbEastRay ) ).get() )
            bbEastRay &= CGen::mbbRookRays[bbOccRay.lsb().get()].mbbWest;
        if ( ( bbOccRay = occupied( bbWestRay ) ).get() )
            bbWestRay &= CGen::mbbRookRays[bbOccRay.msb().get()].mbbEast;
        if ( ( bbOccRay = occupied( bbNorthRay ) ).get() )
            bbNorthRay &= CGen::mbbRookRays[bbOccRay.lsb().get()].mbbSouth;
        CBitBoard bbTo = bbNorthRay.get() 
            | bbEastRay.get() | bbSouthRay.get() | bbWestRay.get();
        while ( bbTo.get() )
        {
            toSqix = bbTo.popLsb(); 
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates all quasi-legal moves in the position
///
void CPos::genWhiteMoves( CMoves& rMoves )
{
    genWhitePawnQuiets( rMoves );
    genWhitePawnCaptures( rMoves );
    genWhiteKnightQuiets( rMoves );
    genWhiteKnightCaptures( rMoves );
    genWhiteBishopQuiets( rMoves );
    genWhiteBishopCaptures( rMoves );
    genWhiteRookQuiets( rMoves );
    genWhiteRookCaptures( rMoves );
    genWhiteQueenQuiets( rMoves );
    genWhiteQueenCaptures( rMoves );
    genWhiteKingQuiets( rMoves );
    genWhiteKingCaptures( rMoves );
}

///
/// generates all quasi-legal moves in the position
///
void CPos::genWhiteLegalMoves( CMoves& rMoves )
{
    CMoves      quasiMoves;

    genWhiteMoves( quasiMoves );
    for ( int moveIx = 0; moveIx < quasiMoves.getNumMoves(); moveIx++ )
    {
        makeMove( quasiMoves.get( moveIx ) );
        if ( !getCheckers().get() )
        {
            rMoves.addMove( quasiMoves.get( moveIx ) );
        }
        unmakeMove( quasiMoves.get( moveIx ) );
    }
}

///
/// generates bishop captures for white
///
/// @param rMoves
///     the bishop captures will be added to rMoves
///
void CPos::genWhiteBishopCaptures( CMoves& rMoves )
{
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kBishop );
    genWhiteBishopCapturesFrom( rMoves, bbFrom );
}

///
/// generates bishop captures for white from the squares in the bitboard that 
/// is passed.
///
/// @param bbFrom
///     the squares from which the diagonal moves are to be generated
///
/// @param rMoves
///     the bishop captures will be added to rMoves
///
void CPos::genWhiteBishopCapturesFrom( CMoves& rMoves, CBitBoard bbFrom )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CSqix           blockerSqix;
    CBitBoard       bbOccRay;
   
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popMsb(); 

        //
        //  Get the rays from the rook position to the edge of the board in
        //  each of the four directions.  (The rook's square is not included
        //  in these rays.
        //
        CBitBoard bbNorthEastRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbNorthEast;
        CBitBoard bbSouthEastRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbSouthEast;
        CBitBoard bbSouthWestRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbSouthWest;
        CBitBoard bbNorthWestRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbNorthWest;
        
        //
        //  Find the square of the blocking piece in each direction, and if 
        //  it's black, generate a capture
        //
        if ( ( bbOccRay = occupied( bbNorthEastRay ) ).get() )
        {
            toSqix = bbOccRay.lsb().get();
            if ( isBlack( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbNorthWestRay ) ).get() )
        {
            toSqix = bbOccRay.lsb().get();
            if ( isBlack( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbSouthEastRay ) ).get() )
        {
            toSqix = bbOccRay.msb().get();
            if ( isBlack( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbSouthWestRay ) ).get() )
        {
            toSqix = bbOccRay.msb().get();
            if ( isBlack( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates bishop non-captures for white
///
/// @param rMoves
///     the bishop moves will be added to rMoves
///
void CPos::genWhiteBishopQuiets( CMoves& rMoves )
{
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kBishop );
    genWhiteBishopQuietsFrom( rMoves, bbFrom );
}

///
/// generates bishop non-captures for white
///
/// @param rMoves
///     the bishop moves will be added to rMoves
///
/// @param bbFrom
///     the set of squares from which the diagonal moves are to be made
///
void CPos::genWhiteBishopQuietsFrom( CMoves& rMoves, CBitBoard bbFrom )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CSqix           blockerSqix;
    CBitBoard       bbOccRay;
   
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popMsb(); 

        //
        //  Get the rays from the bishop position to the edge of the board in
        //  each of the four directions.  (The bishop's square is not included
        //  in these rays.
        //
        CBitBoard bbNorthEastRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbNorthEast;
        CBitBoard bbSouthEastRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbSouthEast;
        CBitBoard bbSouthWestRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbSouthWest;
        CBitBoard bbNorthWestRay 
            = CGen::mbbBishopRays[fromSqix.get()].mbbNorthWest;

        //
        //  For each of thes rays, find the blocker (if any) and mask off
        //  the squares beyond the blocker.
        //
        if ( ( bbOccRay = occupied( bbNorthEastRay ) ).get() )
        {
            bbNorthEastRay 
                &= CGen::mbbBishopRays[bbOccRay.lsb().get()].mbbSouthWest;
        }
        if ( ( bbOccRay = occupied( bbSouthEastRay ) ).get() )
        {
            bbSouthEastRay 
                &= CGen::mbbBishopRays[bbOccRay.msb().get()].mbbNorthWest;
        }
        if ( ( bbOccRay = occupied( bbSouthWestRay ) ).get() )
        {
            bbSouthWestRay 
                &= CGen::mbbBishopRays[bbOccRay.msb().get()].mbbNorthEast;
        }
        if ( ( bbOccRay = occupied( bbNorthWestRay ) ).get() )
        {
            bbNorthWestRay 
                &= CGen::mbbBishopRays[bbOccRay.lsb().get()].mbbSouthEast;
        }
        CBitBoard bbTo 
            = bbNorthEastRay.get() 
            | bbSouthEastRay.get() 
            | bbSouthWestRay.get() 
            | bbNorthWestRay.get();
        while ( bbTo.get() )
        {
            toSqix = bbTo.popMsb(); 
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates king captures for white
///
/// @param rMoves
///     the king captures will be added to rMoves
///
void CPos::genWhiteKingCaptures( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kKing );
    fromSqix = bbFrom.msb(); 
    CBitBoard bbTo = CGen::mbbKingAttacks[fromSqix.get()] 
        & mbbColor[U8( EColor::kBlack )].get();
    while ( bbTo.get() )
    {
        toSqix = bbTo.popMsb(); 
        rMoves.addMove( CMove( fromSqix, toSqix ) );
    }
}

///
/// generates king non-captures for white
///
/// @param rMoves
///     the king moves will be added to rMoves
///
void CPos::genWhiteKingQuiets( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kKing );
    fromSqix = bbFrom.msb(); 
    CBitBoard bbTo = unoccupied( CGen::mbbKingAttacks[fromSqix.get()] );
    while ( bbTo.get() )
    {
        toSqix = bbTo.popMsb(); 
        rMoves.addMove( CMove( fromSqix, toSqix ) );
    }
}

///
/// generates knight captures for white
///
/// @param rMoves
///     the knight captures will be added to rMoves
///
void CPos::genWhiteKnightCaptures( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kKnight );
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popMsb(); 
        CBitBoard bbTo = CGen::mbbKnightAttacks[fromSqix.get()] 
            & mbbColor[U8( EColor::kBlack )].get();
        while ( bbTo.get() )
        {
            toSqix = bbTo.popMsb(); 
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates knight non-captures for white
///
/// @param rMoves
///     the knight moves will be added to rMoves
///
void CPos::genWhiteKnightQuiets( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kKnight );
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popMsb(); 
        CBitBoard bbTo = unoccupied( CGen::mbbKnightAttacks[fromSqix.get()] );
        while ( bbTo.get() )
        {
            toSqix = bbTo.popMsb(); 
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates pawn captures for white
///
/// @param rMoves
///     the pawn captures will be added to rMoves
///
void CPos::genWhitePawnCaptures( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
   
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kPawn );

    //
    //  We can capture on squares that have a black piece or that have just
    //  passed by (for an en-passant).
    //
    CBitBoard bbTargets = mbbColor[U8( EColor::kBlack )];
    if ( mPosRights.isEnPassantLegal() )
    {
        bbTargets.setSquare( 
            CSqix( ERank::kRank6, mPosRights.getEnPassantFile() ).get() );
    }
    CBitBoard bbUp1 = bbFrom.advanceRanks( 1 );
    CBitBoard bbTo = bbTargets.get() 
        & ( bbUp1.leftFiles( 1 ).get() | bbUp1.rightFiles( 1 ).get() );
    while ( bbTo.get() )
    {
        toSqix = bbTo.popMsb();
        //
        //  Is there a capturing pawn to the left of the target?
        //
        if ( toSqix.getFile().get() != EFile::kFileA )
        {
            fromSqix = toSqix.minusRanks( 1 ).minusFiles( 1 );
            if ( bbFrom.atSquare( fromSqix ).get() )
            {
                if ( toSqix.getRank().get() == ERank::kRank8 )
                {
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kQueen ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kRook ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kBishop ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kKnight ) );
                }
                else
                {
                    rMoves.addMove( CMove( fromSqix, toSqix ) );
                }
            }
        }

        //
        //  Is there a capturing pawn to the right of the target?
        //
        if ( toSqix.getFile().get() != EFile::kFileH )
        {
            fromSqix = toSqix.minusRanks( 1 ).plusFiles( 1 );
            if ( bbFrom.atSquare( fromSqix ).get() )
            {
                if ( toSqix.getRank().get() == ERank::kRank8 )
                {
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kQueen ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kRook ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kBishop ) );
                    rMoves.addMove( 
                        CMove( fromSqix, toSqix, EPieceType::kKnight ) );
                }
                else
                {
                    rMoves.addMove( CMove( fromSqix, toSqix ) );
                }
            }
        }
    }
}

///
/// generates pawn pushes for white
///
/// @param rMoves
///     the pawn moves will be added to rMoves
///
void CPos::genWhitePawnQuiets( CMoves& rMoves )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CBitBoard       bbTo;
   
    //
    //  Start with the single pushes
    //
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kPawn );
    CBitBoard bbPop = bbTo = unoccupied( bbFrom.advanceRanks( 1 ) );
    while ( bbPop.get() )
    {
        toSqix = bbPop.popMsb();
        fromSqix = toSqix.minusRanks( 1 );
        if ( toSqix.getRank().get() == ERank::kRank8 )
        {
            rMoves.addMove( CMove( fromSqix, toSqix, EPieceType::kQueen ) );
            rMoves.addMove( CMove( fromSqix, toSqix, EPieceType::kRook ) );
            rMoves.addMove( CMove( fromSqix, toSqix, EPieceType::kBishop ) );
            rMoves.addMove( CMove( fromSqix, toSqix, EPieceType::kKnight ) );
        }
        else
        {
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }

    //
    //  Push forward a second level if we are on the third rand.
    //
    bbPop = unoccupied( bbTo.onRank( ERank::kRank3 ).advanceRanks( 1 ) );
    while ( bbPop.get() )
    {
        toSqix = bbPop.popMsb();
        fromSqix = toSqix.minusRanks( 2 );
        rMoves.addMove( CMove( fromSqix, toSqix ) );
    }
}

///
/// generates captures for the white queen.
///
/// @param rMoves
///     the queen captures will be added to rMoves
///
void CPos::genWhiteQueenCaptures( CMoves& rMoves )
{
    genWhiteRookCapturesFrom( rMoves, 
        getPieces( EColor::kWhite, EPieceType::kQueen ) );
    genWhiteBishopCapturesFrom( rMoves, 
        getPieces( EColor::kWhite, EPieceType::kQueen ) );
}

///
/// generates quiet moves for the white queen.
///
/// @param rMoves
///     the queen moves will be added to rMoves
///
void CPos::genWhiteQueenQuiets( CMoves& rMoves )
{
    genWhiteRookQuietsFrom( rMoves, 
        getPieces( EColor::kWhite, EPieceType::kQueen ) );
    genWhiteBishopQuietsFrom( rMoves, 
        getPieces( EColor::kWhite, EPieceType::kQueen ) );
}

///
/// generates rook captures for white
///
/// @param rMoves
///     the rook captures will be added to rMoves
///
void CPos::genWhiteRookCapturesFrom( CMoves& rMoves, CBitBoard bbFrom )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CSqix           blockerSqix;
    CBitBoard       bbOccRay;
   
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popMsb(); 

        //
        //  Get the rays from the rook position to the edge of the board in
        //  each of the four directions.  (The rook's square is not included
        //  in these rays.
        //
        CBitBoard bbNorthRay = CGen::mbbRookRays[fromSqix.get()].mbbNorth;
        CBitBoard bbEastRay = CGen::mbbRookRays[fromSqix.get()].mbbEast;
        CBitBoard bbSouthRay = CGen::mbbRookRays[fromSqix.get()].mbbSouth;
        CBitBoard bbWestRay = CGen::mbbRookRays[fromSqix.get()].mbbWest;

        //
        //  Find the square of the blocking piece in each direction, and if 
        //  it's black, generate a capture
        //
        if ( ( bbOccRay = occupied( bbNorthRay ) ).get() )
        {
            toSqix = bbOccRay.lsb().get();
            if ( isBlack( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbEastRay ) ).get() )
        {
            toSqix = bbOccRay.lsb().get();
            if ( isBlack( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbWestRay ) ).get() )
        {
            toSqix = bbOccRay.msb().get();
            if ( isBlack( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
        if ( ( bbOccRay = occupied( bbSouthRay ) ).get() )
        {
            toSqix = bbOccRay.msb().get();
            if ( isBlack( toSqix ).get() )
                rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
/// generates rook captures for white
///
/// @param rMoves
///     the rook captures will be added to rMoves
///
void CPos::genWhiteRookCaptures( CMoves& rMoves )
{
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kRook );
    genWhiteRookCapturesFrom( rMoves, bbFrom );
}

///
/// generates rook non-captures for white
///
/// @param rMoves
///     the rook moves will be added to rMoves
///
void CPos::genWhiteRookQuiets( CMoves& rMoves )
{
    CBitBoard bbFrom = getPieces( EColor::kWhite, EPieceType::kRook );
    genWhiteRookQuietsFrom( rMoves, bbFrom );
}

///
/// generates rook non-captures for white
///
/// @param rMoves
///     the rook moves will be added to rMoves
///
/// @param bbFrom
///     the set of squares from w
///     
void CPos::genWhiteRookQuietsFrom( CMoves& rMoves, CBitBoard bbFrom )
{
    CSqix           toSqix;
    CSqix           fromSqix;
    CSqix           blockerSqix;
    CBitBoard       bbOccRay;
   
    while ( bbFrom.get() )
    {
        fromSqix = bbFrom.popMsb(); 

        //
        //  Get the rays from the rook position to the edge of the board in
        //  each of the four directions.  (The rook's square is not included
        //  in these rays.
        //
        CBitBoard bbNorthRay = CGen::mbbRookRays[fromSqix.get()].mbbNorth;
        CBitBoard bbEastRay = CGen::mbbRookRays[fromSqix.get()].mbbEast;
        CBitBoard bbSouthRay = CGen::mbbRookRays[fromSqix.get()].mbbSouth;
        CBitBoard bbWestRay = CGen::mbbRookRays[fromSqix.get()].mbbWest;

        //
        //  For each of thes rays, find the blocker (if any) and mask off
        //  the squares beyond the blocker.
        //
        if ( ( bbOccRay = occupied( bbNorthRay ) ).get() )
            bbNorthRay &= CGen::mbbRookRays[bbOccRay.lsb().get()].mbbSouth;
        if ( ( bbOccRay = occupied( bbEastRay ) ).get() )
            bbEastRay &= CGen::mbbRookRays[bbOccRay.lsb().get()].mbbWest;
        if ( ( bbOccRay = occupied( bbSouthRay ) ).get() )
            bbSouthRay &= CGen::mbbRookRays[bbOccRay.msb().get()].mbbNorth;
        if ( ( bbOccRay = occupied( bbWestRay ) ).get() )
            bbWestRay &= CGen::mbbRookRays[bbOccRay.msb().get()].mbbEast;
        CBitBoard bbTo = bbNorthRay.get() 
            | bbEastRay.get() | bbSouthRay.get() | bbWestRay.get();
        while ( bbTo.get() )
        {
            toSqix = bbTo.popMsb(); 
            rMoves.addMove( CMove( fromSqix, toSqix ) );
        }
    }
}

///
///	makes the specified move in this position, ignoring duplicate
/// positions and the 50 move rule.
///
///	@param m
///		is the move to make
///
///	@param undoContext
///		is the context used to undo the move
///
void CPos::makeMoveForPerft( CMove m, CUndoContext undoContext )
{
	undoContext.setPieceMoved( mBoard[m.getFrom().get()] );
	void setPieceCaptured(CPiece p) { mPieceCaptured = p; }
	void setPosRights(CPosRights pr) { mPosRights = pr; }

	mWhoseMove = mWhoseMove.getOpponent();
	updatePosRights( m );
	++mMoveNum;
	mbbPieceType[mBoard[m.getFrom().get()].getPieceType()].resetSquare(m.getFrom());
	mbbPieceType[mBoard[m.getTo().get()].getPieceType()].setSquare(m.getTo());
	mbbColor[mBoard[m.getFrom().get()].getColor()].resetSquare( m.getFrom() );
	mbbColor[mBoard[m.getTo().get()].getColor()].resetSquare( m.getTo() );
	mBoard[m.getTo().get()] = mBoard[m.getFrom()];
	mBoard[m.getFrom().get()] = CPiece(EPiece::kNone);
	if (mWhoseMove.isWhite())
		findWhiteCheckers();
	else
		findBlackCheckers();
}

///
///  get the next token in the fen string.  
///
///  @param sFen
///      the fen string to tokenize
///
///  @param rIx
///      index to the start of the next token, advaced after each call.
///      std::string::npos indicates we are off the end of the string.
///
///  @returns
///      the next token in the string, or "" at end
///
std::string CPos::nextFenTok( const std::string &sFen, size_t &rIx )
{
    //
    //  Check to see if we are at the end
    //
    if ( rIx == std::string::npos )
        return "";

    //
    // skip spaces
    //
    rIx = sFen.find_first_not_of( ' ', rIx );

    //
    //  fen ends in space?
    //
    if ( rIx == std::string::npos )
        return "";

    //
    //  slash is a token
    //
    if ( sFen[rIx] == '/' )
    {
        rIx++;
        if ( rIx == sFen.length() )
            rIx = std::string::npos;
        return "/";
    }

    //
    //  Take the characters until the next / or space
    //
    size_t nextIx = sFen.find_first_of( "/ ", rIx );
    std::string sRetval = sFen.substr( rIx, 
        ( nextIx == std::string::npos ) ? nextIx : ( nextIx - rIx ) );
    rIx = nextIx;
    return sRetval;
}

///
/// parse the fen format.  For example, the starting position
/// fen is:
///
///     rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
///
///
/// @param  sFen
///     the fen to parse
///
/// @returns
///     ok flag
///
bool CPos::parseFen( 
    const std::string&      sFen, 
    std::string&            rsErrorText )
{
    size_t          fenIx = 0;
    std::string     tok;
    U8              file;

    clearBoard();
    for ( S8 rank = S8( ERank::kRank8 ); rank >= S8( ERank::kRank1 ); rank-- )
    {
        //
        //  Get the pieces in the rank
        //
        file = U8( EFile::kFileA );
        tok = nextFenTok( sFen, fenIx );

        //
        //  Loop through the squares in this rank
        //
        for ( size_t tokIx = 0; tokIx < tok.length(); tokIx++ )
        {
            if ( file > U8( EFile::kFileH ) )
            {
                rsErrorText = CRank( ERank( rank ) ).asStr()
                    +  " has more than 8 squares";
                return false;
            }
            //
            //  Digits define the number or blank squares
            //
            if ( std::isdigit( tok[tokIx] ) )
            {
                U8 numBlankSquares = tok[tokIx] - '0';
                file += numBlankSquares;
            }
            else
            {
                EPiece piece = CPiece::parsePiece( tok.substr( tokIx, 1 ) );
                if ( piece == EPiece::kNone )
                {
                    rsErrorText = "Bad piece: " + tok[tokIx];
                    return false;
                }
                addPiece( piece, CSqix( ERank( rank ), EFile( file ) ) );
                file++;
            }
        }

        //
        //  Get the next slash (unless we just got the first rank )
        //
        if ( rank != S8( ERank::kRank1 ) )
        {
            tok = nextFenTok( sFen, fenIx );
            if ( tok != "/" )
            {
                rsErrorText = "Expected a /";
                return false;
            }
        }
    }

    //
    //  Set whoseMove
    //
    tok = nextFenTok( sFen, fenIx );
    EColor whoseMove = CColor::parseColor( tok );
    if ( whoseMove == EColor::kNone )
    {
        rsErrorText = "Expected a color: " + tok;
        return false;
    }
    mWhoseMove = whoseMove;

    //
    //  Set the casteling rights
    //
    tok = nextFenTok( sFen, fenIx );
    mPosRights.clearCastling();
    if ( tok != "-" )
    {
        for ( size_t j = 0; j < tok.length(); j++ )
        {
            switch ( tok[j] )
            {
            case 'K': 
                mPosRights.setWhiteOO();
                break;
            case 'Q': 
                mPosRights.setWhiteOOO();
                break;
            case 'k': 
                mPosRights.setBlackOO();
                break;
            case 'q': 
				mPosRights.setBlackOOO();
				break;
            default:
				rsErrorText = "Invalid castling flag: " + tok[j];
				return false;
			}
		}
	}

	//
	//  Set the en passant square
	//
	tok = nextFenTok(sFen, fenIx);
	if (tok != "-")
	{
		CSqix           epSqix;
		std::string     errMsg;
		if (!CSqix::parseSqix(tok, epSqix, errMsg))
		{
			rsErrorText = "Invalid en passant: " + tok;
			return false;
		}
		mPosRights.setEnPassantFile(epSqix.getFile());
	}

	//
	//  Set the half move clock (50 move rule).
	//
	tok = nextFenTok(sFen, fenIx);
	char* pzEnd;
	const char* pzNum = tok.c_str();
	long num = std::strtol(tok.c_str(), &pzEnd, 10);
	if (pzEnd == pzNum || pzEnd != pzNum + std::strlen(pzNum))
	{
		rsErrorText = "Invalid half move clock: " + tok;
		return false;
	}
	mHalfMoveClock = U8(num);

	//
	//  Set the move number
	//
	tok = nextFenTok(sFen, fenIx);
	pzNum = tok.c_str();
	num = std::strtol(tok.c_str(), &pzEnd, 10);
	if (pzEnd == pzNum || pzEnd != pzNum + std::strlen(pzNum))
	{
		rsErrorText = "Invalid half move number: " + tok;
		return false;
	}
	mMoveNum = std::uint16_t(num);
	return true;
}

///
///	Updates the position rights after the specified move is made
///
///	@param m
///		the move
void CPos::updatePosRights(CMove m)
{
	//
	//	Figure out the castling rights
	//
	if (mBoard[m.getFrom().get()].getPieceType().get() == EPieceType::kKing)
		mPosRights.onKMove(mBoard[m.getFrom().get()].getColor());
	else if (mBoard[m.getFrom().get()].getPieceType().get()
		== EPieceType::kRook)
	{
		if (mWhoseMove == EColor::kWhite)
		{
			if (m.getFrom().getRank().get() == ERank::kRank1)
				mPosRights.onWqrMove();
			else if (m.getFrom().getRank().get() == ERank::kRank8)
				mPosRights.onWkrMove();
		}
		else
		{
			if (m.getFrom().getRank().get() == ERank::kRank1)
				mPosRights.onBqrMove();
			else if (m.getFrom().getRank().get() == ERank::kRank8)
				mPosRights.onBkrMove();
		}
	}

	//
	//	Figure out the en passant rights.
	//
	if (mWhoseMove == EColor::kWhite)
	{
		//
		//	en passant is legal if the following is true
		//		from square holds a pawn
		//		from the second rank
		//		to the fourth rank
		//
		if (m.getFrom().getRank().get() == ERank::kRank2
			&& m.getFrom().getRank().get() == ERank::kRank4
			&& mBoard[m.getFrom().get()].getPieceType().get()
				== EPieceType::kPawn )
		{
			mPosRights.setEnPassantFile(m.getFrom().getFile());
		}
		else
		{
			mPosRights.clearEnPassantFile(m.getFrom().getFile());
		}
	}
	else
	{
		//
		//	en passant is legal if the following is true
		//		from square holds a pawn
		//		from the 7th rank
		//		to the fifth rank
		//
		if (m.getFrom().getRank().get() == ERank::kRank7
			&& m.getFrom().getRank().get() == ERank::kRank5
			&& mBoard[m.getFrom().get()].getPieceType().get()
				== EPieceType::kPawn)
		{
			mPosRights.setEnPassantFile(m.getFrom().getFile());
		}
		else
		{
			mPosRights.clearEnPassantFile(m.getFrom().getFile());
		}
	}
}

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
/// format rights 
///
std::string CPosRights::asStr() const
{
    std::string     s = castlingAsStr() + " ";

    if ( isEnPassantLegal() ) 
        s.append( getEnPassantFile().asAbbr() );
    else
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