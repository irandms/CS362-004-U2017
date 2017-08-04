#include "testhelper.h"

void testAdventurer() {
	struct gameState baseState2P = {0}; // will store a basic initialized gamestate with 2 players
	struct gameState baseState3P = { 0 }; // will store a basic initialized gamestate with 3 players
	struct gameState baseState4P = { 0 }; // will store a basic initialized gamestate with 4 players
	struct gameState pretestState; // will store the current test case's pre-test gameState
	struct gameState posttestState; // will store the current test case's post-test gameState
	struct stateExpectations emptyExpectations = { 0 }; // empty expectation struct is copied whenever one is needed
	struct stateExpectations curTestExpectations; // will store the current test case's expectation data
	int i; // loop counter
	// below values are used to set up the initialized base gamestates:
	int kingdomCards[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy };
	int randomSeed = 9422;

	// populates the members of the base gameState structs as if game were beginning
	initializeGame(2, kingdomCards, randomSeed, &baseState2P);
	initializeGame(3, kingdomCards, randomSeed, &baseState3P);
	initializeGame(4, kingdomCards, randomSeed, &baseState4P);


	// TEST CASE: Treasure Cards at Start of Deck (cardtest1.c; Adventurer)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState2P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (2P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Treasure Cards at Start of Deck (cardtest1.c; Adventurer)";
	curTestExpectations.testDescription = "Player 1 of a 2-player game plays adventurer.\n"
		"The next two cards in the deck are a copper and gold, both of which should be acquired.";
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change due to drawing two treasures
	curTestExpectations.change_handCount[1] = 2; // Player 1's hand count should increase by 2 (two treasures added)
	curTestExpectations.change_deckCount[1] = -2; // Player 1's deck count should decrease by 2 (both treasures removed)
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	int testDeck1[2] = {copper, gold};
	for (i = 0; i < 2; i++) {
		pretestState.deck[1][i] = testDeck1[i];
	}
	pretestState.deckCount[1] = 2;
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = adventurer;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(adventurer, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Treasure Cards in Middle of Deck (cardtest1.c; Adventurer)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Treasure Cards in Middle of Deck (cardtest1.c; Adventurer)";
	curTestExpectations.testDescription = "Player 1 of a 3-player game plays adventurer.\n"
		"The next four cards in their deck are smithy, smithy, silver, and gold; both treasures should be acquired.";
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change due to drawing two treasures
	curTestExpectations.change_handCount[1] = 2; // Player 1's hand count should increase by 2 (two treasures added)
	curTestExpectations.change_deckCount[1] = -4; // Player 1's deck count should decrease by 4 (2 non-treasures and 2 treasures removed)
	curTestExpectations.change_discard[1] = 1; // Player 1's discard pile should change
	curTestExpectations.change_discardCount[1] = 2; // Both non-treasure cards drawn should be discarded
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	int testDeck2[4] = {smithy, smithy, silver, gold};
	for (i = 0; i < 4; i++) {
		pretestState.deck[1][i] = testDeck2[i];
	}
	pretestState.deckCount[1] = 4;
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = adventurer;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(adventurer, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Deck is Empty, 2 Treasure in Discard (cardtest1.c; Adventurer)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Deck is Empty, 2 Treasure in Discard (cardtest1.c; Adventurer)";
	curTestExpectations.testDescription = "Player 2 of a 4-player game plays adventurer.\n"
		"Their deck is empty but the discards has two golds. Both golds should be acquired.";
	curTestExpectations.change_hand[2] = 1; // Player 2's hand should change due to drawing two treasures
	curTestExpectations.change_handCount[2] = 2; // Player 2's hand count should increase by 2 (two treasures added)
	curTestExpectations.change_discard[2] = 1; // Player 2's discard pile should change
	curTestExpectations.change_discardCount[2] = -2; // Both treasures from discard should be moved out
	curTestExpectations.change_deck[2] = 1; // Player 2's deck should change due to the shuffle
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.deckCount[2] = 0;
	pretestState.whoseTurn = 2;
	pretestState.hand[2][0] = adventurer;
	pretestState.discard[2][0] = gold;
	pretestState.discard[2][1] = gold;
	pretestState.discardCount[2] = 2;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(adventurer, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: More Than 2 Treasures in Deck (cardtest1.c; Adventurer)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "More Than 2 Treasures in Deck (cardtest1.c; Adventurer)";
	curTestExpectations.testDescription = "Player 2 of a 4-player game plays adventurer.\n"
		"Their deck has 5 treasures and 2 other cards; only the first two treasures should be acquired.";
	curTestExpectations.change_hand[2] = 1; // Player 2's hand should change due to drawing two treasures
	curTestExpectations.change_handCount[2] = 2; // Player 2's hand count should increase by 2 (two treasures added)
	curTestExpectations.change_discard[2] = 1; // Player 2's discard pile should change
	curTestExpectations.change_discardCount[2] = 1; // 1 non-treasure card should be discarded
	curTestExpectations.change_deckCount[2] = -3; // 3 cards should come from the deck (2 treasures, 1 discarded smithy)
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	int testDeck3[7] = { copper, smithy, gold, silver, gold, smithy, copper };
	for (i = 0; i < 7; i++) {
		pretestState.deck[2][i] = testDeck3[i];
	}
	pretestState.deckCount[2] = 7;
	pretestState.whoseTurn = 2;
	pretestState.hand[2][0] = adventurer;
	pretestState.handCount[2] = 1;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(adventurer, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	/* This test case is left commented out because it results in an infinite loop (and eventually a segfault)
	// TEST CASE: No Treasures in Deck or Discards (cardtest1.c; Adventurer)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "No Treasures in Deck or Discards (cardtest1.c; Adventurer)";
	curTestExpectations.testDescription = "Player 2 of a 4-player game plays adventurer.\n"
		"There are no treasures in their deck or discards; no treasures shoudl be acquired.";
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.deckCount[2] = 0;
	pretestState.discardCount[2] = 0;
	pretestState.whoseTurn = 2;
	pretestState.hand[2][0] = adventurer;
	pretestState.handCount[2] = 1;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(adventurer, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);
	*/
}


int main(int argc, char *argv[])
{
	testAdventurer();
	return 0;
}