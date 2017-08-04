#include "dominion.h"
#include "dominion_helpers.h"
#include <stdlib.h>
#include <string.h>

// If set  to 0, only failed state tests will be reported (return value test remains)
#define SHOW_PASSES 1

/* Struct definition. Used to store the expected change in each field between two gamestates */
struct stateExpectations {
	const char *testTitle;
	const char *testDescription;
	int change_numPlayers;
	int change_supplyCount[treasure_map + 1];
	int change_embargoTokens[treasure_map+1];
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