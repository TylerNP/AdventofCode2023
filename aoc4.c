/*
 * Author: Tyler Pham
 *
 * Filename: aoc3.c
 * 
 * Version: 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_MAX 150
#define CARD_MAX 200
#define INTIAL_CAPACITY 10

// Determine if char is a number
int isDigit( char * ptr )
{
    return (*ptr >= '0' && *ptr <= '9')? 1 : 0;
}

// Convert String to Number with lenght
int * convertInt ( char * ptr )
{
    int * output = malloc( 2 * sizeof(int) );
    *output = 0;
    *(output + 1) = 0;
    int n = 10;
    char * num = ptr;

    // Convert to Digits (Assuming Max In 10s)
    // printf("%s \n", num);
    while( isDigit(num) )
    {
        *output += n*( *num-'0');
        n /= 10;
        num++;
        (*(output+1))++;
        
    }

    // Removes Extra Zeros
    for (int i = 2; i > *(output+1); i--) *output /= 10;

    return output;
}

// Qsort Sorting Function
// Returns num1 > num2
int numeric( const void *num1, const void*num2 )
{
    return  *((int *)num1) - *((int *) num2);
}

// Bsearch Function
// Returns num1 - num2
int numSearch( const void * num1, const void * num2 )
{
    return *((int *) num1) - *((int *) num2);
}


// Returns value of card
int winValue( int count )
{
    int sum = 0;
    if (count != 0) sum = 1;
    for (int i = 0; i < count-1; i++) sum *= 2;
    return sum;
}
int main ( int argc, char * argv[] )
{
    // Ensure minimum args
    if (argc < 2)
    {
        printf("Need a scratch file");
        exit(1);
    }

    // Open Scratch file
    FILE *scratchFile = fopen(argv[1], "r");
    if (!scratchFile)
    {
        printf("Can't open %s file for reading\n", argv[1]);
        exit(1);
    }

    // Variables
    int size = 0;
    int total = 0;
    int cards = 0;
    int cardIndex = 0;
    int capacity = INTIAL_CAPACITY;
    char ** arr = malloc( capacity * sizeof(char *) );
    char * input = malloc( LINE_MAX * sizeof(char) );
    int * cardCount = malloc( CARD_MAX * sizeof(int) );
    int * cardMult = malloc( CARD_MAX * sizeof(int) );

    while (fgets(input, LINE_MAX, scratchFile) != NULL)
    {
        // Trim new line
        char * nl = strchr(input, '\n');
        if (nl) *nl = '\0';

        // Incrase array size by 50% when full
        if (size == capacity)
        {
            capacity = ( capacity * 3/2 );
            arr = realloc( arr, capacity * sizeof(char *) );
        }

        // Allocate space for new string
        *(arr + size) = malloc( strlen(input) + 1 );

        // Copy over string and increment size
        strcpy( *(arr + size), input );
        (size)++;

        // Search for : and |
        char * winStr = strchr(input, ':');
        winStr += 1;
        char * numsStr = strchr(input, '|');
        numsStr += 1;

        // Create Winning Num Array and Nums Array
        int * winningNum = malloc( LINE_MAX * sizeof(int) );
        int winIndex = 0;
        int * numsArr = malloc( LINE_MAX * sizeof(int) );
        int numsIndex = 0;

        // Fill array utnil dividing line
        while (*winStr != '|')
        {
            // Skip spaces
            if ( *winStr == ' ' ) 
            {
                winStr++; 
            // Convert to int
            } else if (isDigit(winStr)) {
                int * arr = convertInt(winStr);
                *(winningNum + winIndex) = *arr;
                winIndex++;
                winStr += 1 + *(arr + 1);

                free(arr);
            }

        }

        // Fill array till end of line
        while ( *numsStr != '\0')
        {
            // Skip spaces
            if ( *numsStr == ' ' ) 
            {
                numsStr++; 
            // Convert to int
            } else if (isDigit(numsStr)) {
                int * arr = convertInt(numsStr);
                *(numsArr + numsIndex) = *arr;
                numsIndex++;
                numsStr += 1 + *(arr + 1);

                free(arr);
            }

        }

        // Sort and search for winning values
        qsort( winningNum, winIndex, sizeof(int), numeric);
        qsort( numsArr, numsIndex, sizeof(int), numeric);
        int winCount = 0;

        // Loop through winning values
        for (int i = 0; i < winIndex; i++)
        {
            // Match winning value to values
            int * m = bsearch( (winningNum + i), numsArr, numsIndex, sizeof(int), numSearch );
            if (m) winCount++; // Add to count for wins
        }

        // Save number of winning numbers 
        *(cardCount + cardIndex) = winCount;
        cardIndex++;

        total += winValue(winCount);
        
        // Free memory
        free(winningNum);
        free(numsArr);
    }

    // Fill CardMult Array
    for (int i = 0; i < cardIndex; i++)
    {
        *(cardMult + i) = 1;
        *(cardMult + i + 1) = -1;
    }

    // Find number of cards won
    for (int i = 0; i < cardIndex; i++)
    {
        // Check if adding cards is necessary
        if (*(cardCount + i) > 0)
        {
            // Use current multiplier
            for (int j = 0; j < *(cardMult + i); j++)
            {
                // Add new multipliers to next set of cards
                for (int k = 0; k < *(cardCount + i); k++)
                {
                    *(cardMult + i + k + 1) += 1;
                }
            }
            
        }
    }

    // Sum up cards
    for (int i = 0; i < cardIndex; i++)
    {
        cards += *(cardMult + i);
    }
    printf("\nPart 1: %d | Part 2: %d \n", total, cards);

    // Free memory
    for ( int i = 0; i < size; i++ )
    {
        free( *(arr+i) );
    }
    free(arr);

}