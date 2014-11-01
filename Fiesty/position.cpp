///
/// file position.cpp
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with positions
///
///
#include <cctype>
#include "position.h"

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
    for ( std::int8_t rank = U8( ERank::kRank8 ); 
        rank < U8( ERank::kRank1 ); rank++ )
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
            //
            //  Digits define the number or blank squares
            //
            if ( std::isdigit( tok[tokIx] ) )
            {
                U8 numBlankSquares = tok[tokIx] - '0';
                if ( numBlankSquares + file > U8( EFile::kFileH ) )
                {
                    rsErrorText = CRank( ERank( rank ) ).asStr()
                        +  " has more than 8 squares";
                    return false;
                }
                file += numBlankSquares;
            }
            CPiece piece = CPiece::parsePiece( tok[tokIx] );
            if ( piece.get() == EPiece::kNone )
            {
                rsErrorText = "Bad piece: " + tok[tokIx];
                return false;
            }
            addPiece( piece, CSqix( ERank( rank ), EFile( file ) ) );
        }

        //
        //  Get the next slash (unless we just got the first rank)
        //
        if ( rank != U8( ERank::kRank1 ) )
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
    mWhoseMove = CColor::parseColor( tok );
    if ( mWhoseMove == EColor::kNone )
    {
        rsErrorText = "Expected a color: " + tok;
        return false;
    }

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
    if ( pzEnd == pzNum || pzEnd != pzEnd + std::strlen( pzNum ) )
    {
        rsErrorText = "Invalid half move clock: " + tok;
        return false;
    }
    mHalfMoveClock = num;

    //
    //  Set the move number
    //
    tok = nextFenTok( sFen, fenIx );
    pzNum = tok.c_str();
    num = std::strtol( tok.c_str(), &pzEnd, 10 );
    if ( pzEnd == pzNum || pzEnd != pzEnd + std::strlen( pzNum ) )
    {
        rsErrorText = "Invalid half move number: " + tok;
        return false;
    }
    mMoveNum = num;
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