/*
 * Author: Tyler Pham
 *
 * Filename: aoc1.c
 * 
 * Version: 2.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_LEN 80 // Assume lines read are within 80 

// Struct defining calibration numbers
// Stores calibration numbers as two ints for first and last 
struct caliNum 
{
    int first;
    int last;
};

// Function that infds single digit numbers in word form from string and replace
char findNumWord ( char * input )
{
    char firstNums[9][6] = { "one", "two", "three" , "four", "five", "six", "seven" , "eight", "nine" };
    char numbers[9] = { '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    char output = '@';

    // Loop through arrays matching chars
    for (int i = 0; i < 9; i++)
    {
        // Variables
        int word = 1;

        // Dtermine if number in word form is in string
        for (int j = 0; j < strlen(firstNums[i]); j++ )
        {
            if ( *(input + j) != firstNums[i][j])
            {
                word = 0;
                break;
            }
        }

        // Save word form for output
        if (word)
        {
            output = numbers[i];
            break;
        }
    }

    return output;

}

// Function that converts caliNum into real ints
int convertInt ( struct caliNum num)
{
    return ( (num.first*10) + num.last );
}

int main (int argc, char *argv[])
{
    // Ensure minimum arguments
    if (argc < 2)
    {
        printf("Need an calibration file");
        exit(1);
    }

    // Open calibration file (simple txt file)
    FILE *caliFile = fopen(argv[1], "r");
    if (!caliFile)
    {
        printf("Can't open %s file for reading\n", argv[1]);
        exit(1);
    }

    // Variables
    int sum1 = 0;
    int sum2 = 0;
    char * input = malloc( LINE_LEN * sizeof(char) );

    // Read file line by line
    while (fgets(input, LINE_LEN, caliFile) != NULL )
    {
        // Variables
        struct caliNum cNum1;
        struct caliNum cNum2;
        int flag1 = 1;
        int flag2 = 1;
        char * ptr = input;

        // Strip new line char
        char * nl = strchr(input, '\n');
        if (nl) *nl = '\0';

        // Fill in calibration numbers
        while ( *ptr != '\0' )
        {
            //Vars       
            char num = findNumWord( ptr );
            int inputNum1 = 0;
            int inputNum2 = 0;

            if ( *ptr >= '1' && *ptr<='9') 
            {
                inputNum1 = *ptr - '0';
                inputNum2 = *ptr - '0';
                
            } else if (num != '@') {
                inputNum1 = num - '0';
            }

            // Save First Num Only Once
            if (inputNum1 > 0)
            {
                if (flag1)
                {
                    cNum1.first = inputNum1;
                    cNum1.last = inputNum1;
                    flag1 = 0;

                // Save Last Num until last 
                } else {
                    cNum1.last = inputNum1;

                }
            }

            // Save First Num Only Once
            if (inputNum2 > 0)
            {
                if (flag2)
                {
                    cNum2.first = inputNum2;
                    cNum2.last = inputNum2;
                    flag2 = 0;

                // Save Last Num until Last
                } else {
                    cNum2.last = inputNum2;

                }
            }

            // Move to next letter
            ptr++;
        }

        // Add to sum
        sum1 += convertInt(cNum1); 
        sum2 += convertInt(cNum2);

        // printf("%d). %d %d = %d %d\n", count, cNum1.first, cNum1.last, convertInt(cNum1), sum1);
    }

    // Display output
    printf("Sum for Part 2: %d\n", sum1);
    printf("Sum for Part 1: %d\n", sum2);

    // Close files
    fclose(caliFile);
    free(input);

}
