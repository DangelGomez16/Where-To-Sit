// Daniel A. Gomez
// COP 3502C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 50

int *permute(int *perm, int **pair, int numPeop, int tracker, int *popCorn);
int *nextPerm(int *perm, int length);

int main()
{
    // Receives user information on the number of people and the number
    // of pairs for a given row.
    int numPeop, pairs;
    scanf("%d %d", &numPeop, &pairs);

    // Based on the user's inputed information, allocate the appropriate amount
    // of memory for a number of arrays.
    char **peopArr = malloc(numPeop * sizeof(char*));
    int **pairArr = calloc(numPeop, sizeof(int*));
    int *rowArr = malloc(numPeop * sizeof(int));
    int *popCorn = calloc(numPeop, sizeof(int));

    // Assign each name to cell, and represent each person using a number
    // to make future permutations much easier.
    for(int i = 0; i < numPeop; i++)
    {
        peopArr[i] = malloc((MAXLEN + 1) * sizeof(char));
        pairArr[i] = calloc(numPeop, sizeof(int));
        rowArr[i] = i;
        char name[MAXLEN + 1];

        scanf("%s %d", name, &popCorn[i]);
        strcpy(peopArr[i], name);
    }

    // Based on which cell each person is at, upon receiving the names of the pairs
    // find out which number they are each associated with, then mark down a one
    // in a 2D array representing which two people cannot sit next to each other.
    for(int i = 0; i < pairs; i++)
    {
        int trackerOne = -1;
        int trackerTwo = -1;
        char pairOne[MAXLEN + 1];
        char pairTwo[MAXLEN + 1];

        scanf("%s %s", pairOne, pairTwo);
        for(int i = 0; i < numPeop; i++)
        {
            if(strcmp(pairOne, peopArr[i]) == 0)
                trackerOne = i;

            if(strcmp(pairTwo, peopArr[i]) == 0)
                trackerTwo = i;

            if(trackerOne != -1 && trackerTwo != -1)
            {
                pairArr[trackerOne][trackerTwo] = 1;
                pairArr[trackerTwo][trackerOne] = 1;
                break;
            }
        }
    }

    // Call the permute function to see how many valid permutations exist.
    int tracker = 0;
    rowArr = permute(rowArr, pairArr, numPeop, tracker, popCorn);

    for(int i = 0; i < numPeop; i++)
        printf("%s\n", peopArr[rowArr[i]]);

    for(int i = 0; i < numPeop; i++)
    {
        free(peopArr[i]);
        free(pairArr[i]);
    }
    free(peopArr);
    free(pairArr);
    free(rowArr);
    free(popCorn);

}

// Check if the current permutation is a valid order. If it is not, either
// because someone does not have access to popcorn or is seated with someone
// they aren't supposed to, the tracker will not increase. If the for loop
// manages to go through numPeop number of times without problems, the tracker
// increases by one. After this, a new permutation is passed onto the next recursion,
// where this loop will continue until no further valid recursions can be created,
// whereby the recursion function will return the number of valid permutations.
int *permute(int *perm, int **pair, int numPeop, int tracker, int *popCorn)
{
    // Checking the current permutation to see if it is valid.
    for(int i = 0; i < numPeop; i++)
    {
        int popCheck = 0;

        // If anyone, whether the current person, or a person to their left
        // or right has popcorn, they pass the popcorn check. Otherwise, break.
        if(popCorn[perm[i]] == 1)
            popCheck++;

        // If anyone has someone they are not supposed to sit next to next to them
        // break, no matter what.
        if(i != 0)
        {
            if(popCorn[perm[i - 1]] == 1)
                popCheck++;

            if(pair[perm[i]][perm[i - 1]] == 1)
                break;
        }
        if(i != numPeop - 1)
        {
            if(popCorn[perm[i + 1]] == 1)
                popCheck++;

            if(pair[perm[i]][perm[i + 1]] == 1)
                break;
        }

        if(popCheck == 0)
            break;

        if(i == numPeop - 1)
            tracker++;
    }

    if(tracker == 1)
        return perm;

    // Iterate the next permutation for the next recursion call.
    else
    {
        perm = nextPerm(perm, numPeop);
        return(permute(perm, pair, numPeop, tracker, popCorn));
    }

}

// This is the permutation algorithm given in class with some modifications.
// This function will generate the next permutation. If there are no more possible
// permutations to generate, it will mark the current permutation's first element with
// a -1 to indicate we have reached the end of our possible permutations.
int *nextPerm(int *perm, int length)
{
    int i = length - 1;
    while(i > 0 && perm[i] < perm[i - 1])
        i--;
    i--;

    // If we've reached our last permutation, return modified permutation.
    if(i == -1)
    {
        perm[0] = -1;
        return perm;
    }

    int j = length - 1;
    while(j > i && perm[j] < perm[i])
        j--;

    int temp = perm[i];
    perm[i] = perm[j];
    perm[j] = temp;

    int k, m;
    for(k = i + 1, m = length - 1; k < m; k++, m--)
    {
        temp = perm[k];
        perm[k] = perm[m];
        perm[m] = temp;
    }

    // Return the generated permutation.
    return perm;
}
