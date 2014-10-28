///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// code having to do with squares
///
///
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

