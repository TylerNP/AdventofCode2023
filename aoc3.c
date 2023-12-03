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
#define INTIAL_CAPACITY 10
#define SYMBOL_MAX 22500

// struct defining symbol location
struct position 
{
    int x;
    char * y;
};

// Determine if char is a number
int isDigit( char * ptr )
{
    return (*ptr >= '0' && *ptr <= '9')? 1 : 0;
}



// Find symbols locations in line
int * findSymbol( char * ptr )
{
    // Variables
    int * arr = malloc( (strlen(ptr) + 1) * sizeof(int) );
    int i = 0;
    int index = 0;

    // Loop through string and add locations to array
    while (*ptr != '\0')
    {
        if ( !isDigit(ptr) && *ptr != '.' )
        {
            *(arr + index) = i;
            index++;
        } 
        
        i++;
        ptr++;
    }

    // Signal end of array
    if (index == 0) 
    {
        *arr = -1;
    } else {
        *(arr + index) = -1;
        
    }

    return arr;
}

// Convert String to Number
int convertInt ( char * ptr )
{
    int n = 100;
    int count = 0;
    int sum = 0;
    char * num = ptr;

    // Convert to Digits (Assuming Max In 100s)
    while( isDigit(num) )
    {
        sum += n*( *num-'0');
        n /= 10;
        num++;
        count++;
        
    }

    // Removes Extra Zeros
    for (int i = 3; i > count; i--) sum /= 10;
    
    return sum;
}

// Return num to the left and count
int * leftNum ( char * input )
{
    // Variables
    int * left = malloc( 2 * sizeof(int) );
    *left = 0;
    *(left + 1) = 0;
    char * ptr = input;

    // Check if there is a number to start with
    if ( isDigit( ptr ) )
    {
        int num = 0;
        *(left + 1) = 1;

        // Look for digits to the left
        char * index = (input-1);
        while ( isDigit(index) )
        {
            index--;
        }
        index++;
        
        // Convert and return
        num = convertInt( index );

        *left = num;
    }

    return left;

}

// Return num to the right
int * rightNum ( char * input )
{
    int * right = malloc( 2 * sizeof(int) );
    *right = 0;
    *(right + 1) = 0;
    if ( isDigit(input) )
    {
        *(right + 1) = 1;
        int num = 0;
        num = convertInt(input);
        *right = num; 
    }

    return right;
}

// Return nums in middle
int * sumMid ( char * input )
{
    // Variables
    int sum = 0;
    int * mid = malloc( 3 * sizeof(int) );
    *mid = 0;
    *(mid + 1) = 0;
    *(mid + 2) = 0;
    char * num = input;

    // Check cases of adjacency 
    if ( !isDigit(input) )
    {
        int * left = leftNum(input-1);
        int * right = rightNum(input+1);

        // Determine if there is two nums or not
        if (left[1])
        {
            *mid += left[0];
            *(mid + 1) += right[0];
        } else {
            *mid += right[0];
            *(mid + 1) += left[0];
        }
        
        *(mid + 2) += left[1] + right[1];

        free(left);
        free(right);

    } else if ( !isDigit(input-1) ) {
        int * right = rightNum(input);
        *mid += right[0];
        *(mid + 2) += right[1];

        free(right);

    } else if ( !isDigit(input+1) ) {
        int * left = leftNum(input);
        *mid += left[0];
        *(mid + 2) += left[1];

        free(left);

    } else {
        int * right = rightNum(input-1);
        *mid += right[0];
        *(mid +2) += right[1];

        free(right);

    }

    return mid;
}

// Return sum of adjacent numbers
int * sumAdj ( char ** arr, int row, int elm, int end )
{
    /// Arrays for each part
    int * adj = malloc( 2* sizeof(int) );
    int * left = leftNum( &arr[row][elm-1] );
    int * right = rightNum( &arr[row][elm+1] );
    int * top = malloc( 3* sizeof(int) );
    int * bot = malloc( 3* sizeof(int) );

    // Set initial values
    *adj = 0;
    *top = 0;
    *bot = 0;
    *(adj + 1) = 0;
    *(top + 1) = 0;
    *(bot + 1) = 0;
    *(top + 2) = 0;
    *(bot + 2) = 0;

    // Don't search above and below if there isn't any more
    if (row != 0) top = sumMid( &arr[row-1][elm] );
    if ( row + 2 <= end ) bot = sumMid( &arr[row+1][elm] );

    // Sum numbers adjacent to symbol
    *adj += left[0] +  right[0] + top[0] + bot[0] + top[1] + bot[1];

    // Determine if its a gear (2 adjacent)
    if ( (left[1] + right[1] + top[2] + bot[2]) == 2 )
    {
        // Find Product & Determine which two nums is adjacent
        *(adj + 1) = (left[0]*left[1]) * (right[0]*right[1]);

        if ( top[2] == 2 ) 
        {
            *(adj + 1) = top[0]*top[1];

        } else if ( bot[2] == 2 ) {
            *(adj + 1) = bot[0]*bot[1];

        } else if ( top[2] + bot[2] == 2) {
            *(adj + 1) = top[0] * bot[0];

        } else if (top[2] + bot[2] == 1) {
            *(adj + 1) = (left[0]*left[1] + right[0]*right[1]) * (top[0]*top[2] + bot[0]*bot[2]);

        }

    }

    // Free meory
    free(left);
    free(right);
    free(top);
    free(bot);

    // printf("%d ", *adj);
    return adj;
}

int main ( int argc, char * argv[] )
{
    // Ensure minimum args
    if (argc < 2)
    {
        printf("Need a game engine schematic");
        exit(1);
    }

    // Open Log file
    FILE *engineFile = fopen(argv[1], "r");
    if (!engineFile)
    {
        printf("Can't open %s file for reading\n", argv[1]);
        exit(1);
    }

    // Variables
    int size = 0;
    int index = 0;
    int sum = 0;
    int sumGear = 0;
    int capacity = INTIAL_CAPACITY;
    char * input = malloc( LINE_MAX * sizeof(char) );
    char ** arr = malloc( capacity * sizeof(char *) );
    int ** posList = malloc( capacity * sizeof(int *) );
    struct position * list = malloc (SYMBOL_MAX * sizeof( struct position ));

    // Read file in line by line
    while (fgets(input, LINE_MAX, engineFile) != NULL)
    {
        // Trim new line
        char * nl = strchr(input, '\n');
        if (nl) *nl = '\0';

        // Incrase array size by 50% when full
        if (size == capacity)
        {
            capacity = ( capacity * 3/2 );
            arr = realloc( arr, capacity * sizeof(char *) );
            posList = realloc( posList, capacity * sizeof(int *) );
        }

        // Allocate space for new string
        *(arr + size) = malloc( strlen(input) + 1 );

        // Copy over string and increment size
        strcpy( *(arr + size), input );
        (size)++;

        // Locate symbol and save
        *(posList + index)= findSymbol(input);
        index++;
        
    }

    // Loop through symbol positions and add adjancent nums
    for (int i = 0; i < index; i++)
    {
        printf("%d| ", i+1);
        for ( int j = 0; posList[i][j] != -1; j++)
        {   
            int * adj = sumAdj( arr, i, posList[i][j], index);
            sum += adj[0];
            sumGear += adj[1];

            // Free meory
            free(adj);

        }
        printf("\n");

    }

    // Display results
    printf("Part1: %d Part2: %d\n", sum, sumGear);

    // Free memory
    free (input);
    for ( int i = 0; i < size; i++ )
    {
        free( *(arr+i) );
    }
    free(list);
    free(arr);

}