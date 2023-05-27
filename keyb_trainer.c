#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>
#include <stdbool.h>
#include <time.h>

bool inList( int element, const int *list, int length )
{
    for (int i = 0; i < length; i++)
    {
        if (list[ i ] == element)
        {
            return true;
        }
    }

    return false;
}

//========================================================================================== 
// checks for digital, alphanumeric and punctuation characters
// coding of parameter selection
// Bit      : | 5     | 4            | 3     | 2     | 1     | 0     |
// check for: | digit | alphanumeric | row 1 | row 2 | row 3 | row 4 | 
//========================================================================================== 
bool checkSymbol( int asciiCode, int selection )
{
    bool inSelection = false;

    // number row: `~!@#$%^&*()-_=+
    const int symRow1[ ] = { 96, 126, 33, 64, 35, 36, 37, 38, 94, 42, 40, 41, 45, 95, 61, 43 };

    // second row: [{]}
    const int symRow2[ ] = { 91, 123, 93, 125 };

    // third row: ;:'"\|
    const int symRow3[ ] = { 59, 58, 39, 34, 92, 124 };

    // fourth row: ,<.>/?
    const int symRow4[ ] = { 44, 60, 46, 62, 47, 63 };

    // stores number of symbols in every row
    const int numSymbols[ ] =
    {
        sizeof( symRow1 ) / sizeof( *symRow1 ), 
        sizeof( symRow2 ) / sizeof( *symRow2 ),
        sizeof( symRow3 ) / sizeof( *symRow3 ),
        sizeof( symRow4 ) / sizeof( *symRow4 )
    };

    // build up jagged array of rowdata
    const int *symbols[ ] =
    {
        symRow1,
        symRow2,
        symRow3,
        symRow4
    };

    // check less often for digits (first bit of selection)
    if (selection & (1 << 5) && rand( ) < 10000)
    {
        inSelection |= isdigit( asciiCode );
    }

    // check less often for alphanumerics (second bit of selection)
    if (selection & (1 << 4) && rand( ) < 5000)
    {
        inSelection |= isalpha( asciiCode );
    }

    // check remaining bits (symbol rows)
    for (int i = 3; i >= 0; i--)
    {
        inSelection |= ( selection & (1 << i) ) ? inList( asciiCode, symbols[ 3 - i ], numSymbols[ 3 - i ] ) : false;
    }

    return inSelection;
}

main( )
{
    int       i; // input
    int       c; // ascii character

    const int keyGroupSelector = 0b10'1000;

    srand( time( 0 ) );

    puts( "Press ESC to quit" );

    for (;;) 
    {
        for (; !( checkSymbol( c, keyGroupSelector ) ); c = rand( ) % 127);

        putch( c );

        // ignore wrong input
        while ( (i = getch( )) != c)
        {
            if (i == 27) // grab esc key
                exit( 0 );
            else
            {
               puts( "\ntype error" );
               putch( c );
            }
        }

        c = 0; // reset
    }
}
