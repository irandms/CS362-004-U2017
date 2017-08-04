#include "testhelper.h"

void testVillage() {
	struct gameState baseState2P = {0}; // will store a basic initialized gamestate with 2 players
	struct gameState baseState3P = { 0 }; // will store a basic initialized gamestate with 3 players
	struct gameState baseState4P = { 0 }; // will store a basic initialized gamestate with 4 players
	struct gameState pretestState; // will store the current test case's pre-test gameState
	struct gameState posttestState; // will store the current test case's post-test gameState
	struct stateExpectations emptyExpectations = { 0 }; // empty expectation struct is copied whenever one is needed
	struct stateExpectations curTestExpectations; // will store the current test case's expectation data
	// below values are used to set up the initialized base gamestates:
	int kingdomCards[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy };
	int randomSeed = 4627;

	// populates the members of the base gameState structs as if game were beginning
	initializeGame(2, kingdomCards, randomSeed, &baseState2P);
	initializeGame(3, kingdomCards, randomSeed, &baseState3P);
	initializeGame(4, kingdomCards, randomSeed, &baseState4P);


	// TEST CASE: Basic Village Test (cardtest3.c; Village)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Basic Village Test (cardtest3.c; Village)";
	curTestExpectations.testDescription = "Player 2 of a 4-player game plays village.\n"
		"They should get two actions and one card (from deck to hand).";
	curTestExpectations.change_hand[2] = 1; // Player 2's hand should change due to drawing a card
	// Note that the player's handCount shouldn't change, as they're discarding 1 and drawing 1
	curTestExpectations.change_deckCount[2] = -1; // Player 2's deck count should decrease by 1
	curTestExpectations.change_playedCards = 1; // Player 2's played set should change due to use of discardCard
	curTestExpectations.change_playedCardCount = 1;
	curTestExpectations.change_numActions = 2; // The player should gain two actions
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.whoseTurn = 2;
	pretestState.hand[1][0] = village;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(village, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Village With Empty Deck (cardtest3.c; Village)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Village With Empty Deck (cardtest3.c; Village)";
	curTestExpectations.testDescription = "Player 2 of a 4-player game plays village.\n"
		"They have no cards in their deck and 1 discard. The discard should be shuffled and acquired.";
	curTestExpectations.change_hand[2] = 1; // Player 2's hand should change due to drawing a card
	curTestExpectations.change_playedCards = 1; // Player 2's played set should change due to use of discardCard
	curTestExpectations.change_playedCardCount = 1;
	curTestExpectations.change_numActions = 2; // The player should gain two actions
	curTestExpectations.change_discard[2] = 1; // The discard pile should change due to the shuffle
	curTestExpectations.change_discardCount[2] = -1; // 1 card should be removed from the discard pile
	curTestExpectations.change_deck[2] = 1; // Player 2's deck array should change due to the shuffle
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.whoseTurn = 2;
	pretestState.hand[2][0] = village;
	pretestState.handCount[2] = 1;
	pretestState.deckCount[2] = 0;
	pretestState.discardCount[2] = 1;
	pretestState.discard[2][0] = estate;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(village, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Village With Empty Deck and Empty Discards (cardtest3.c; Village)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Village With Empty Deck and Empty Discards (cardtest3.c; Village)";
	curTestExpectations.testDescription = "Player 2 of a 3-player game plays village.\n"
		"They have no cards in their deck and no discards. They should gain actions but not draw a card.";
	curTestExpectations.change_hand[2] = 1; // Player 2's hand should change
	curTestExpectations.change_handCount[2] = -1; // Village should be discarded but no new card is drawn
	curTestExpectations.change_playedCards = 1; // Player 2's played set should change due to use of discardCard
	curTestExpectations.change_playedCardCount = 1;
	curTestExpectations.change_numActions = 2; // The player should gain two actions
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.whoseTurn = 2;
	pretestState.hand[2][0] = village;
	pretestState.handCount[2] = 1;
	pretestState.deckCount[2] = 0;
	pretestState.discardCount[2] = 0;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(village, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);
}


int main(int argc, char *argv[])
{
	testVillage();
	return 0;
}