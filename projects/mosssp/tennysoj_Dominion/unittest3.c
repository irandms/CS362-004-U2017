#include "testhelper.h"

void testBuyCard() {
	struct gameState baseState2P = {0}; // will store a basic initialized gamestate with 2 players
	struct gameState baseState3P = { 0 }; // will store a basic initialized gamestate with 3 players
	struct gameState baseState4P = { 0 }; // will store a basic initialized gamestate with 4 players
	struct gameState pretestState; // will store the current test case's pre-test gameState
	struct gameState posttestState; // will store the current test case's post-test gameState
	struct stateExpectations emptyExpectations = { 0 }; // empty expectation struct is copied whenever one is needed
	struct stateExpectations curTestExpectations; // will store the current test case's expectation data
	// below values are used to set up the initialized base gamestates:
	int kingdomCards[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy };
	int randomSeed = 9422;

	// populates the members of the base gameState structs as if game were beginning
	initializeGame(2, kingdomCards, randomSeed, &baseState2P);
	initializeGame(3, kingdomCards, randomSeed, &baseState3P);
	initializeGame(4, kingdomCards, randomSeed, &baseState4P);


	// TEST CASE: Buy Card from Supply (unittest3.c; buyCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Buy Card from Supply (unittest3.c; buyCard)";
	curTestExpectations.testDescription = "A basic test of the buyCard function.\n" 
		"Player 0 attempts to buy an estate at the beginning of a 3-player game. Attempt should succeed.";
	curTestExpectations.change_discard[0] = 1; // Player 0's discards should change due to buying
	curTestExpectations.change_discardCount[0] = 1; // Player 0's discard count should increase by 1
	curTestExpectations.change_supplyCount[estate] = -1; // Count of estates in the supply should decrease by 1
	curTestExpectations.change_phase = 1; // Phase should change from 0 to 1
	curTestExpectations.change_coins = -2; // Two coins should be spent on the estate
	curTestExpectations.change_numBuys = -1; // Player should use one of their buys
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.coins = 5; // player should be left with 3 coins after test
	pretestState.numBuys = 2; // player should have one more buy after test
	pretestState.whoseTurn = 0;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = buyCard(estate, &posttestState);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Attempt to Buy with No More Buys (unittest3.c; buyCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState2P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (2P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Attempt to Buy with No More Buys (unittest3.c; buyCard)";
	curTestExpectations.testDescription = "Player 1 of a 2-player game attempts to buy a card while they have 0 buys.\n"
		"The attempt should fail.";
	curTestExpectations.expected_return = -1;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.numBuys = 0; // Adding 3 discards to player 1's discard pile
	pretestState.whoseTurn = 1;
	pretestState.coins = 6;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = buyCard(gold, &posttestState);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Attempt to Buy with No More in Supply (unittest3.c; buyCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Attempt to Buy with No More in Supply (unittest3.c; buyCard)";
	curTestExpectations.testDescription = "Player 2 of a 4-player game attempts to buy a card for which the supply is exhausted.\n"
		"The attempt should fail.";
	curTestExpectations.expected_return = -1;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.numBuys = 1; // Adding 3 discards to player 1's discard pile
	pretestState.whoseTurn = 2;
	pretestState.coins = 6;
	pretestState.supplyCount[minion] = 0;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = buyCard(minion, &posttestState);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Attempt to Buy Card Not in Supply (unittest3.c; buyCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Attempt to Buy Card Not in Supply (unittest3.c; buyCard)";
	curTestExpectations.testDescription = "Player 3 of a 4-player game attempts to buy a card that is not in the supply.\n"
		"The attempt should fail.";
	curTestExpectations.expected_return = -1;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.numBuys = 1; // Adding 3 discards to player 1's discard pile
	pretestState.whoseTurn = 3;
	pretestState.coins = 4;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = buyCard(outpost, &posttestState);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Attempt to Buy with No More in Supply (unittest3.c; buyCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Attempt to Buy Without Enough Gold (unittest3.c; buyCard)";
	curTestExpectations.testDescription = "Player 2 of a 3-player game attempts to buy a card they cannot afford.\n"
		"The attempt should fail.";
	curTestExpectations.expected_return = -1;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.numBuys = 1; // Adding 3 discards to player 1's discard pile
	pretestState.whoseTurn = 2;
	pretestState.coins = 0;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = buyCard(minion, &posttestState);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);
}


int main(int argc, char *argv[])
{
	testBuyCard();
	return 0;
}