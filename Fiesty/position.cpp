///
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
#include "test.h"

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

    for ( I8 r = I8( ERank::kRank8 ); r >= I8( ERank::kRank1 ); r-- )
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

    for ( I8 r = I8( ERank::kRank8 ); r >= I8( ERank::kRank1 ); r-- )
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
        if ( r != I8( ERank::kRank1 ) )
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

    std::memset( 
        mBoard, U8( EPiece::kNone ), U8( ERank::kNum ) * U8( EFile::kNum ) );
    std::memset( mbbPieceType, 0, sizeof( mbbPieceType ) );
    std::memset( mbbColor, 0, sizeof( mbbColor ) );
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
    CBitBoard bbFrom = mbbPieceType[U8( EPieceType::kPawn )].get() 
        & mbbColor[U8( EColor::kBlack )].get();
    CBitBoard bbPop = bbTo = notOccupied( bbFrom.retreatRanks( 1 ) );
    while ( bbPop.get() )
    {
        toSqix = bbPop.popLsb();
        fromSqix = toSqix.plusRank( 1 );
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
    bbPop = notOccupied( bbTo.onRank( ERank::kRank6 ).retreatRanks( 1 ) );
    while ( bbPop.get() )
    {
        toSqix = bbPop.popLsb();
        fromSqix = toSqix.plusRank( 2 );
        rMoves.addMove( CMove( fromSqix, toSqix ) );
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
    CBitBoard bbFrom = mbbPieceType[U8( EPieceType::kPawn )].get() 
        & mbbColor[U8( EColor::kWhite )].get();
    CBitBoard bbPop = bbTo = notOccupied( bbFrom.advanceRanks( 1 ) );
    while ( bbPop.get() )
    {
        toSqix = bbPop.popLsb();
        fromSqix = toSqix.minusRank( 1 );
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
    bbPop = notOccupied( bbTo.onRank( ERank::kRank3 ).advanceRanks( 1 ) );
    while ( bbPop.get() )
    {
        toSqix = bbPop.popLsb();
        fromSqix = toSqix.minusRank( 2 );
        rMoves.addMove( CMove( fromSqix, toSqix ) );
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
    CBitBoard       bbTo;
   
    CBitBoard bbFrom = mbbPieceType[U8( EPieceType::kPawn )].get() 
        & mbbColor[U8( EColor::kWhite )].get();
    CBitBoard bbTo = bbFrom.advanceRanks( 1 ) ;

    //
    //  We can capture on squares that have a black piece or that have just
    //  passed by (for an en-passant).
    //
    CBitBoard bbTargets = mbbColor[U8( EColor::kBlack )];
    if ( mPosRights.isEnPassantLegal() )
        bbTargets.setSquare( 
            CSqix( ERank::kRank6, mPosRights.getEnPassantFile() ).get() );
    bbTo = bbTargets.get() 
        & ( bbTo.leftFiles( 1 ).get() | bbTo.rightFiles( 1 ).get() );
    while ( bbTo.get() )
    {
        toSqix = bbTo.popLsb();
        //
        //  Is there a capturing pawn to the left of the target?
        //
        if ( toSqix.getFile().get() != EFile::kFileA )
        {
            fromSqix = toSqix.minusRank( 1 ).minusFile( 1 );
            if ( bbTargets.atSquare( fromSqix ).get() )
            {
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
        }

        //
        //  Is there a capturing pawn to the right of the target?
        //
        if ( toSqix.getFile().get() != EFile::kFileH )
        {
            fromSqix = toSqix.minusRank( 1 ).plusFile( 1 );
            if ( bbTargets.atSquare( fromSqix ).get() )
            {
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
        }
    }
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
        ( nextIx == std::string::npos ) ? nextIx : nextIx- rIx );
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
    for ( I8 rank = I8( ERank::kRank8 ); rank >= I8( ERank::kRank1 ); rank-- )
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
        if ( rank != I8( ERank::kRank1 ) )
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
    tok = nextFenTok( sFen, fenIx );
    if ( tok != "-" )
    {
        CSqix epSqix = CSqix::parseSqix( tok );
        if ( epSqix.isNone() )
        {
            rsErrorText = "Invalid en passant: " + tok;
            return false;
        }
        mPosRights.setEnPassantFile( epSqix.getFile() );
    }

    //
    //  Set the half move clock (50 move rule).
    //
    tok = nextFenTok( sFen, fenIx );
    char* pzEnd;
    const char* pzNum = tok.c_str();
    long num = std::strtol( tok.c_str(), &pzEnd, 10 );
    if ( pzEnd == pzNum || pzEnd != pzNum + std::strlen( pzNum ) )
    {
        rsErrorText = "Invalid half move clock: " + tok;
        return false;
    }
    mHalfMoveClock = U8( num );

    //
    //  Set the move number
    //
    tok = nextFenTok( sFen, fenIx );
    pzNum = tok.c_str();
    num = std::strtol( tok.c_str(), &pzEnd, 10 );
    if ( pzEnd == pzNum || pzEnd != pzNum + std::strlen( pzNum ) )
    {
        rsErrorText = "Invalid half move number: " + tok;
        return false;
    }
    mMoveNum = std::uint16_t( num );
    return true;
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