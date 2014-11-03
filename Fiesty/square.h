/// file square.h
///
/// Fiesty (C) 2014 by Jeffery A Esposito
///
/// headers having to do with positions
///
///
#ifndef Fiesty_square_h
#define Fiesty_square_h
#include "fiesty.h"

///
/// Class for a rank
///
class CRank
{
public:
    CRank() {}
    CRank( ERank r ) 
    { 
        mRank = r; 
        assert( isValid() );
    }
    ERank get() const { return mRank; }
    bool isValid() const { return mRank < ERank::kNum; }
    std::string asStr() const;
    std::string asAbbr() const;
    static CRank parseRank( const std::string& s );

private:
    ERank                       mRank;
    static const char*          kStrs[ERank::kNum];
    static const char*          kAbbrs[ERank::kNum];
};

///
/// Class for a file
///
class CFile
{
public:
    CFile() {}
    CFile( EFile f ) 
    { 
        mFile = f; 
        assert( isValid() );
    }
    EFile get() const { return mFile; }
    bool isValid() const { return mFile < EFile::kNum; }
    std::string asStr() const;
    std::string asAbbr() const;
    static CFile parseFile( const std::string& s );

private:
    EFile                       mFile;
    static const char*          kStrs[EFile::kNum];
    static const char*          kAbbrs[EFile::kNum];
};

///
/// class for a square index, 0 to 63
///
class CSqix
{
public:
    CSqix() {}
    CSqix( YSqix s ) { mSqix = s; }
    CSqix( CRank r, CFile f ) { mSqix = ( U8( r.get() ) << 3 ) + U8( f.get() ); }
    YSqix get() const { return mSqix; }
    CRank getRank() const { return ERank( mSqix >> 3 ); }
    CFile getFile() const { return EFile( mSqix & 0x07 ); }
    bool isNone() const 
    { 
        return getRank().get() == ERank::kNone 
            && getFile().get() == EFile::kNone; 
    }  
    YBitBoard asBitBoard() const { return 0x1ULL << mSqix; }
    std::string asStr() const;
    std::string asAbbr() const;
    static CSqix parseSqix( const std::string& s );

private:
    YSqix                   mSqix;
};

#endif      //  square.h
