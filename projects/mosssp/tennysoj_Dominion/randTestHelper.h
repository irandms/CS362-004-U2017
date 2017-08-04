#include "dominion.h"
#include "dominion_helpers.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

// If set  to 0, only failed state tests will be reported (return value test remains)
#define SHOW_PASSES 1

// number of random gamestates that will be tested with each card
#define NUM_TESTS 10000

// If set to 1, extra information about the pre and post test hand, deck, discard, and played card contents will be printed
#define SHOW_EXTRA_INFO 1

/* Struct definition. Used to store the expected change in each field between two gamestates */
struct stateExpectations {
	int testNum;
	int testHandPos;
	int cardTested;
	int change_numPlayers;
	int change_supplyCount[treasure_map + 1];
	int change_embargoTokens[treasure_map + 1];
	int change_outpostPlayed;
	int change_outpostTurn;
	int change_whoseTurn;
	int change_phase;
	int change_numActions;
	int change_coins;
	int change_numBuys;
	int change_hand[MAX_PLAYERS];
	int change_handCount[MAX_PLAYERS];
	int change_deck[MAX_PLAYERS];
	int change_deckCount[MAX_PLAYERS];
	int change_discard[MAX_PLAYERS];
	int change_discardCount[MAX_PLAYERS];
	int change_playedCards;
	int change_playedCardCount;
	int expected_return;
	int actual_return;
};

/* Given two gamestates and a test's state change expectation struct, determines which state
** fields changed properly and which did not- printing out PASS/FAIL results for each pairing
** of a gameState field and the test's expected/desired change in that field */
int evaluateTest(struct gameState *pretestState, struct gameState *posttestState,
	struct stateExpectations *expectedChanges);

// Returns a pointer to a gameState that has been allocated and populated with random values (with direction)
struct gameState* randGameState();

// Helper function that returns a random integer between min and max (inclusive)
int randBetween(int min, int max);