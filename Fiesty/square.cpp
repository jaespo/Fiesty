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
/// parses the string as a color
///
CRank CRank::parseRank( const std::string& s )
{
    if ( s.length() != 1 ) 
        return ERank::kNone;
    for ( size_t ix = 0; ix < U8( EColor::kNum ); ix++ )
        if ( std::toupper( s[0] ) == kAbbrs[0][0] )
            return ERank( ix );
    return ERank::kNone;
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
/// parses the string as a color
///
CFile CFile::parseFile( const std::string& s )
{
    if ( s.length() != 1 ) 
        return EFile::kNone;
    for ( size_t ix = 0; ix < U8( EColor::kNum ); ix++ )
        if ( std::toupper( s[0] ) == kAbbrs[0][0] )
            return EFile( ix );
    return EFile::kNone;
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
CSqix CSqix::parseSqix( const std::string& s )
{
    if ( s.length() != 2 ) 
        return CSqix( ERank::kNone, EFile::kNone );
    CFile f = CFile::parseFile( s.substr( 0, 1 ) );
    if ( f.get() == EFile::kNone )
        return CSqix( ERank::kNone, EFile::kNone );
    CRank r = CRank::parseRank( s.substr( 1, 1 ) );
    if ( r.get() == ERank::kNone )
        return CSqix( ERank::kNone, EFile::kNone );
    return CSqix( r, f );
}


