///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with squares
///
///
#include <cctype>
#include <cstring>
#include "square.h"

//
//  statics
//
const char* CRank::kStrs[ERank::kNum] = 
{ 
    "1st rank", 
    "2nd rank", 
    "3rd rank", 
    "4th rank", 
    "5th rank", 
    "6th rank", 
    "7th rank", 
    "8th rank" 
};

const char* CRank::kAbbrs[ERank::kNum] = 
    { "1", "2", "3", "4", "5", "6", "7", "8" };

const char* CFile::kStrs[EFile::kNum] = 
{ 
    "a file", 
    "b file", 
    "c file", 
    "d file", 
    "e file", 
    "f file", 
    "g file", 
    "h file" 
};

const char* CFile::kAbbrs[EFile::kNum] = 
    { "a", "b", "c", "d", "e", "f", "g", "h" };

///
/// Abbreviation string representation for a rank
///
std::string CRank::asAbbr() const
{
    return std::string( kAbbrs[U8( mRank )] );
}

///
/// String representation for a color
///
std::string CRank::asStr() const
{
    return std::string( kStrs[ U8( mRank )] );
}

///
/// parses the string as a rank.  Returns ERank::kNone on an error
///
bool CRank::parseRank( const std::string& s, CRank& rank, std::string& errText )
{
    if ( s.length() != 1 ) 
    {
        errText = "invalid rank: bad length";
        return false;
    }
    for ( size_t ix = 0; ix < U8( ERank::kNum ); ix++ )
        if ( s[0] == kAbbrs[ix][0] )
        {
            rank = ERank( ix );
            return true;
        }
    errText = "invalid rank: bad character";
    return false;
}

///
/// Abbreviation string representation for a File
///
std::string CFile::asAbbr() const
{
    return std::string( kAbbrs[U8( mFile )] );
}

///
/// String representation for a color
///
std::string CFile::asStr() const
{
    return std::string( kStrs[ U8( mFile )] );
}

///
/// parses the string as a file.  Returns EFile::kNone on an error.
///
bool CFile::parseFile( const std::string& s, CFile& file, std::string& errText )
{
    if ( s.length() != 1 ) 
    {
        errText = "invalid file: bad length";
        return false;
    }
    for ( size_t ix = 0; ix < U8( EFile::kNum ); ix++ )
    {
        if ( std::toupper( s[0] ) == std::toupper( kAbbrs[ix][0] ) )
        {
            file = EFile( ix );
            return true;
        }
    }
    errText = "invalid file: bad character";
    return false;
}

///
/// String representation for a square as a two character abbreviation
/// 
std::string CSqix::asAbbr() const
{
    return getFile().asAbbr() + getRank().asAbbr();
}

///
/// String representation for a square as a two character abbreviation
/// and numeric
///
std::string CSqix::asStr() const
{
    return getFile().asAbbr() + getRank().asAbbr()
        + "(" + std::to_string( mSqix ) + ")";
}

///
/// String representation for a square as a two character abbreviation
/// and numeric
///
bool CSqix::parseSqix( const std::string& s, CSqix& rSqix, std::string errMsg )
{
    CFile           file;
    CRank           rank;
    std::string     err;

    if ( s.length() != 2 ) 
    {
        errMsg = "invalid square: bad length";
        return false;
    }
    if ( !CFile::parseFile( s.substr( 0, 1 ), file, err ) )
    {
        errMsg = "invalid square: " + err;
        return false;
    }
    if ( !CRank::parseRank( s.substr( 1, 1 ), rank, err ) )
    {
        errMsg = "invalid square: " + err;
        return false;
    }
    rSqix = CSqix( rank, file );
    return true;
}


