#include "testhelper.h"

void testGainCard() {
	struct gameState baseState2P = { 0 }; // will store a basic initialized gamestate with 2 players
	struct gameState baseState3P = { 0 }; // will store a basic initialized gamestate with 3 players
	struct gameState baseState4P = { 0 }; // will store a basic initialized gamestate with 4 players
	struct gameState pretestState; // will store the current test case's pre-test gameState
	struct gameState posttestState; // will store the current test case's post-test gameState
	struct stateExpectations emptyExpectations = { 0 }; // empty expectation struct is copied whenever one is needed
	struct stateExpectations curTestExpectations; // will store the current test case's expectation data
	// below values are used to set up the initialized base gamestates:
	int kingdomCards[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy };
	int randomSeed = 4211;

	// populates the members of the base gameState structs as if game were beginning
	initializeGame(2, kingdomCards, randomSeed, &baseState2P);
	initializeGame(3, kingdomCards, randomSeed, &baseState3P);
	initializeGame(4, kingdomCards, randomSeed, &baseState4P);

	// TEST CASE: Gain Card to Hand (unittest2.c; gainCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState2P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (2P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Gain Card to Hand (unittest2.c; gainCard)";
	curTestExpectations.testDescription = "Player 1 of a 2-player game attempts to gain a card from supply to hand.\n"
		"The card is present in the supply and should be gained.";
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change due to gaining a card
	curTestExpectations.change_handCount[1] = 1; // Player 1's hand count should increase by 1
	curTestExpectations.change_supplyCount[village] = -1; // The quantity of supply card village should decrease by 1
	curTestExpectations.expected_return = 0;


	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = gainCard(village, &posttestState, 2, 1);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Gain Card to Deck (unittest2.c; gainCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Gain Card to Deck (unittest2.c; gainCard)";
	curTestExpectations.testDescription = "Player 3 of a 4-player game attempts to gain a card from supply to deck.\n"
		"The card is present in the supply and should be gained.";
	curTestExpectations.change_deck[3] = 1; // Player 3's deck should change due to gaining a card
	curTestExpectations.change_deckCount[3] = 1; // Player 3's deck count should increase by 1
	curTestExpectations.change_supplyCount[adventurer] = -1; // The quantity of supply card adventurer should decrease by 1
	curTestExpectations.expected_return = 0;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = gainCard(adventurer, &posttestState, 1, 3);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Gain Card to Discards (unittest2.c; gainCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Gain Card to Discards (unittest2.c; gainCard)";
	curTestExpectations.testDescription = "Player 0 of a 3-player game attempts to gain a card from supply to discards.\n"
		"The card is present in the supply and should be gained.";
	curTestExpectations.change_discard[0] = 1; // Player 0's discards should change due to gaining a card
	curTestExpectations.change_discardCount[0] = 1; // Player 0's discard count should increase by 1
	curTestExpectations.change_supplyCount[cutpurse] = -1; // The quantity of supply card cutpurse should decrease by 1
	curTestExpectations.expected_return = 0;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = gainCard(cutpurse, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Gain Card Not in Supply (unittest2.c; gainCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Gain Card Not in Supply (unittest2.c; gainCard)";
	curTestExpectations.testDescription = "Player 3 of a 4-player game attempts to gain a card from supply to deck.\n"
		"The card is not available in the supply, so gainCard should fail.";
	curTestExpectations.expected_return = -1;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = gainCard(baron, &posttestState, 1, 3);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: No More of Card in Supply (unittest2.c; gainCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState2P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (2P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "No More of Card in Supply (unittest2.c; gainCard)";
	curTestExpectations.testDescription = "Player 1 of a 2-player game attempts to gain a card from supply to hand.\n"
		"The card is present in the supply, but there are none left. gainCard should fail.";
	curTestExpectations.expected_return = -1;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.supplyCount[village] = 0; // Empty out supply of village

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = gainCard(village, &posttestState, 2, 1);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);
}


int main(int argc, char *argv[])
{
	testGainCard();
	return 0;
}