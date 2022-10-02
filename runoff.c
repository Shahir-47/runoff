#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int a = 0; a < voter_count; a++)
    {
        for (int b = 0; b < candidate_count; b++)
        {
            if (candidates[preferences[a][b]].eliminated == false) //check to see that the candidates are not eliminated
            {
                candidates[preferences[a][b]].votes = candidates[preferences[a][b]].votes + 1; //increment the votes for each candidate if found
                break;
            }
        }

    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    int total = 0;
    for (int b = 0; b < candidate_count; b++)
    {
        total = total + candidates[b].votes; //counter to count the total number of votes
    }
    for (int b = 0; b < candidate_count; b++)
    {
        if (candidates[b].votes > 0.5 * total) //check to see if any candidate has majority of the votes
        {
            printf("%s\n", candidates[b].name);
            return true;
        }
    }

    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int lowest = 999;
    for (int b = 0; b < candidate_count; b++)
    {   
        // min value in the candidates[].votes array is returned
        if (candidates[b].eliminated == false && candidates[b].votes < lowest)
        {
            lowest = candidates[b].votes;
        }
    }
    return lowest;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    for (int b = 0; b < candidate_count; b++)
    {
        if (candidates[b].eliminated == false)
        {
            //all the remaining values will be either same as the minimum value or the other value wil be greater than the minimum
            if (candidates[b].votes > min) 
            {
                return false;
            }
                   
        }
    }

    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int b = 0; b < candidate_count; b++)
    {
        //check to see if any of the candidates has the same number of votes as the return value  of find_min , so we can eliminate that candidate
        if (candidates[b].votes == min)
        {
            candidates[b].eliminated = "true";
        }
    }

    return;
}
