/*
 *
 * Author: Tyler Pham
 *
 * Filename: aoc2.c
 *
 * Version: 1.2
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED_MAX 12
#define GREEN_MAX 13
#define BLUE_MAX 14
#define LINE_MAX 500
#define MAX_CUBE 1000

// Struct defining bag containing red, green, and blue cubes
// Stores count of the 3 different cube colrs
struct bag 
{
    int red;
    int green;
    int blue;
};

// Return the number of digits in an int
int countDigit (int input)
{
    int count = 0;

    do 
    {
        count++;
        // Removes 1 digit at the end
        input /= 10;
    } while (input != 0);

    return count;
}

// Returns a bag with count for colored cubes
struct bag countGame( char * ptr )
{
    struct bag temp;
    temp.red = 0;
    temp.green = 0;
    temp.blue = 0;

    // Loop until end of game (;)
    for (char * tempPtr = ptr; tempPtr < strchr(ptr, ';'); )
    {
        // Variables
        int count = 0;
        int increment;
        char color[7];

        // Extract values
        sscanf(tempPtr, " %d %[^,^;]", &count, color);

        // Remove ; character when extracted
        char * nG = strchr(color, ';');
        if (nG) *nG = '\0';

        // Initial value of 3 from 2 spaces and 1 comma
        increment = 3 + countDigit(count);

        // Save Cube Count to Appropiate Color
        if ( strcmp(color, "red" ) == 0 )
        {
            temp.red = count;
        } else if ( strcmp(color, "blue" ) == 0 ) {
            temp.blue = count;
        } else if ( strcmp(color, "green" ) ==0 ) {
            temp.green = count;
        }

        // Increment to next block of inputs
        tempPtr = (tempPtr + strlen(color) + increment);

    }

    return temp;
}

// Returns a bag with the maximum cubes of both bags
struct bag consolidateBag( struct bag a, struct bag b )
{
    struct bag temp;
    temp.red = ( a.red > b.red ) ? a.red : b.red;
    temp.blue = ( a.blue > b.blue ) ? a.blue : b.blue;
    temp.green = ( a.green > b.green ) ? a.green : b.green;

    return temp;

}

// Compare bag
int compareBag( struct bag a, struct bag b)
{
    int output = 0;

    if ( (a.red < b.red)  || (a.green < b.green) || (a.blue < b.blue)  )
    {
        output = 0;
    } else {
        output = 1;
    }

    return output;
}

int main (int argc, char *argv[] )
{
    // Ensure minimum arguments
    if (argc < 2)
    {
        printf("Need a game log file");
        exit(1);
    }

    // Open Log file
    FILE *logFile = fopen(argv[1], "r");
    if (!logFile)
    {
        printf("Can't open %s file for reading\n", argv[1]);
        exit(1);
    }

    // Variables
    int idSum = 0;
    int cubePower = 0;
    char * input = malloc( LINE_MAX * sizeof(char) );
    struct bag test;
    int index = 1;
    test.red = 12;
    test.green = 13;
    test.blue = 14;

    // Read file in line by line
    while (fgets(input, LINE_MAX, logFile) != NULL)
    {
        // Variables
        struct bag largestBag;
        struct bag smallestBag;
        largestBag.red = 0;
        largestBag.green = 0;
        largestBag.blue = 0;
        int gameID = 0;
        char * ptr = input;

        // Remove nl char
        char * nl = strchr(input, '\n');
        if (nl) *nl = '\0';

        // Add another ; at the end of each line
        char * nc = strchr(input, '\0');
        if (nc) *nc = ';';
        *(nc + 1) = '\0';

        // Strip Game ID and find starting point
        sscanf(input, "%*s %d %*s", &gameID);
        ptr = strchr(ptr, ':') + 1;

        // Search through string
        while ( *ptr != '\0' )
        {
            // Obtain values and find maximums
            struct bag temp = countGame( ptr );
            largestBag = consolidateBag( largestBag, temp );

            // Increment pointer to next game
            ptr = strchr(ptr, ';')+ 1;

        }

        cubePower += (largestBag.red * largestBag.green * largestBag.blue);

        // Test if game position is possible
        if (compareBag(test, largestBag) )      
        {
            idSum += gameID;
            // printf("%d). Red %d | Green %d | Blue %d\n", index, largestBag.red, largestBag.green, largestBag.blue);  
        }

        index++;

    }

    // Display Results
    printf("%d\n%d", idSum, cubePower);

    // Free memory and close files
    fclose(logFile);
    free(input);
}