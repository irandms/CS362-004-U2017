#include "testhelper.h"

void testSalvager() {
	struct gameState baseState2P = {0}; // will store a basic initialized gamestate with 2 players
	struct gameState baseState3P = { 0 }; // will store a basic initialized gamestate with 3 players
	struct gameState baseState4P = { 0 }; // will store a basic initialized gamestate with 4 players
	struct gameState pretestState; // will store the current test case's pre-test gameState
	struct gameState posttestState; // will store the current test case's post-test gameState
	struct stateExpectations emptyExpectations = { 0 }; // empty expectation struct is copied whenever one is needed
	struct stateExpectations curTestExpectations; // will store the current test case's expectation data
	// below values are used to set up the initialized base gamestates:
	int kingdomCards[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy };
	int randomSeed = 1627;

	// populates the members of the base gameState structs as if game were beginning
	initializeGame(2, kingdomCards, randomSeed, &baseState2P);
	initializeGame(3, kingdomCards, randomSeed, &baseState3P);
	initializeGame(4, kingdomCards, randomSeed, &baseState4P);


	// TEST CASE: Salvage No-Cost Card (cardtest4.c; Salvager)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Salvage No-Cost Card (cardtest4.c; Salvager)";
	curTestExpectations.testDescription = "Player 1 of a 3-player game plays salvager.\n"
		"They choose to trash a card worth 0 coins; they should gain a buy and no coins for the turn.";
	curTestExpectations.change_numBuys = 1; // Player should gain one buy
	curTestExpectations.change_hand[1] = 1; // Player's hand should change
	curTestExpectations.change_handCount[1] = -2; // Player trashes one, and plays one
	curTestExpectations.change_playedCards = 1; // Played cards should change
	curTestExpectations.change_playedCardCount = 1; // 1 card is played
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = salvager;
	pretestState.hand[1][1] = copper;
	pretestState.handCount[1] = 2;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(salvager, 1, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Salvage Cheap Card (cardtest4.c; Salvager)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Salvage Cheap Card (cardtest4.c; Salvager)";
	curTestExpectations.testDescription = "Player 1 of a 3-player game plays salvager.\n"
		"They choose to trash a card worth 2 coins; they should gain a buy and 2 coins for the turn.";
	curTestExpectations.change_numBuys = 1; // Player should gain one buy
	curTestExpectations.change_coins = 2; // Player should gain two coins
	curTestExpectations.change_hand[1] = 1; // Player's hand should change
	curTestExpectations.change_handCount[1] = -2; // Player trashes one, and plays one
	curTestExpectations.change_playedCards = 1; // Played cards should change
	curTestExpectations.change_playedCardCount = 1; // 1 card is played
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = estate;
	pretestState.hand[1][1] = salvager;
	pretestState.handCount[1] = 2;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(salvager, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Salvage Expensive (cardtest4.c; Salvager)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

																   // Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Salvage Expensive Card (cardtest4.c; Salvager)";
	curTestExpectations.testDescription = "Player 1 of a 3-player game plays salvager.\n"
		"They choose to trash a card worth 0 coins; they should gain a buy and no coins for the turn.";
	curTestExpectations.change_numBuys = 1; // Player should gain one buy
	curTestExpectations.change_coins = 6; // Player should gain six coins
	curTestExpectations.change_hand[1] = 1; // Player's hand should change
	curTestExpectations.change_handCount[1] = -2; // Player trashes one, and plays one
	curTestExpectations.change_playedCards = 1; // Played cards should change
	curTestExpectations.change_playedCardCount = 1; // 1 card is played
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = salvager;
	pretestState.hand[1][1] = gold;
	pretestState.handCount[1] = 2;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(salvager, 1, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	
}


int main(int argc, char *argv[])
{
	testSalvager();
	return 0;
}