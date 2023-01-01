#include <math.h>   // for log() and ceil() functions
#include <time.h>   // for time() function to seed srand()
#include <ctype.h>  // for tolower() function
#include <stdlib.h> // for malloc() and free() functions
#include <stdio.h>  // for printf(), sprintf(), and scanf() functions
#include <string.h> // for strlen() function
#include "console_util.h"

// Minimum allowed guessing number (potentailly could be 0)
#define MIN_NUMBER 1

// Error codes
#define STATUS_PLAYER_WINS 1
#define STATUS_PLAYER_LOSES 2
#define STATUS_ENGINE_ERROR 3

// Help screen to display when the program is run incorrectly
void print_usage(char* program)
{
    printf("Guess-a-Number Game\n");
    printf("Play by guessing a secret number between %d and MAX\n", MIN_NUMBER);
    printf("If player guesses the number quickly enough, the player wins!\n");
    printf("Usage: %s <MAX>\n", program);
    printf("  MAX: the highest number that can be chosen\n\n");
    printf("Example: %s 100\n", program);
}

// Calculates the maximum number of guesses to allow for a given game
int guess_count(int min, int max)
{
    // sanity check that max is larger than min
    if (min >= max)
    {
        return -1;
    }

    // binary search requires log_2(n) tries
    int range = max - min;
    float log2 = log(range) / log(2);   // log comes from math.h
    return (int)ceil(log2);             // ceil comes from math.h
}

// Play a single round of the guessing game
int play_game(int min, int max)
{
    // Seed the random number generator
    srand(time(NULL));

    // Basic variables to play the game
    char buffer[16];                    // user input is stored here
    int guess = 0;                      // initial value doesn't matter
    //int secret = 37;                  // known secret for debugging
    int secret = MIN_NUMBER + rand() % (max - MIN_NUMBER); // secret number
    int count = guess_count(min, max);  // maximum allowed guesses
    int rc = STATUS_ENGINE_ERROR;       // return code from each function

    // Continue playing until all of the guesses are used up
    while(count > 0)
    {
        // Prompt the user and convert their input to an integer
        printf("Guess a number between %d and %d (you have %d guesses): ",
               min, max, count);
        rc = get_line_static(buffer, sizeof(buffer));
        if (rc == EXIT_FAILURE)
        {
            debug("get_line() failed");
            return STATUS_ENGINE_ERROR;
        }
        rc = to_integer(buffer, &guess);
        if (rc == EXIT_FAILURE)
        {
            debug("to_integer() failed");
            return STATUS_ENGINE_ERROR;
        }

        // Determine if the guess was too high or low
        if (guess > secret)
        {
            printf("'%d' was too high\n", guess);
        }
        else if (guess < secret)
        {
            printf("'%d' was too low\n", guess);
        }
        else
        {
            printf("'%d' was the secret number\n", guess);
            break;
        }

        // This used up one guess before the game ends
        count--;
    }

    // Return a status code to signify whether if player won
    if (guess == secret)
    {
        return STATUS_PLAYER_WINS;
    }
    else
    {
        return STATUS_PLAYER_LOSES;
    }
}


int main(int argc, char* argv[])
{
    // Common and repeatedly used variables
    int rc = EXIT_FAILURE;          // return code from each function call
    int min = MIN_NUMBER;           // the minimum number for the guessing game
    int max = -1;                   // the maximum number for the guessing game
    int res = STATUS_ENGINE_ERROR;  // assume the worst result

    // Sanity check for enough arguments
    if (argc < 2)
    {
        // argv[0]: the first argument is name of the program
        debug("You must specify a MAX parameter\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Sanity check for correct arguments
    rc = to_integer(argv[1], &max);
    if (rc == EXIT_FAILURE)
    {
        debug("Could not convert the MAX parameter to a valid number\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    // Run the actual program
    res = play_game(min, max);
    if (res == STATUS_PLAYER_WINS)
    {
        printf("You won! Either you got lucky or you know binary search\n\n");
    }
    else if (res == STATUS_PLAYER_LOSES)
    {
        printf("Good try. Thanks for playing.\n\n");
    }
    else
    {
        debug("Quitting due to unexpected error\n\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
