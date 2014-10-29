//
// Fiesty.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include "fiesty.h"
#include "test.h"

int main( int argc, const char* argv[] )
{
    std::cout << "Fiesty (C) 2014 by Jeffery A Esposito" << std::endl;
    if ( argc > 1 && strcmp( argv[1],"test" ) == 0 )
    {
        CTester::testAll();
        return 0;
    }
    return 0;
}

 