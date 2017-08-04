#include "testhelper.h"

void testDiscardCard() {
	struct gameState baseState2P = { 0 }; // will store a basic initialized gamestate with 2 players
	struct gameState baseState3P = { 0 }; // will store a basic initialized gamestate with 3 players
	struct gameState baseState4P = { 0 }; // will store a basic initialized gamestate with 4 players
	struct gameState pretestState; // will store the current test case's pre-test gameState
	struct gameState posttestState; // will store the current test case's post-test gameState
	struct stateExpectations emptyExpectations = { 0 }; // empty expectation struct is copied whenever one is needed
	struct stateExpectations curTestExpectations; // will store the current test case's expectation data
	int i; // loop counter
	// below values are used to set up the initialized base gamestates:
	int kingdomCards[10] = { adventurer, gardens, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy };
	int randomSeed = 8330;
	int testHand[5] = { silver, council_room, remodel, baron, cutpurse };

	// populates the members of the base gameState structs as if game were beginning
	initializeGame(2, kingdomCards, randomSeed, &baseState2P);
	initializeGame(3, kingdomCards, randomSeed, &baseState3P);
	initializeGame(4, kingdomCards, randomSeed, &baseState4P);

	// TEST CASE: Discard from End of Hand (unittest4.c; discardCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Discard from End of Hand (unittest4.c; discardCard)";
	curTestExpectations.testDescription = "Player 1 of a 4-player game attempts to discard the last card in their hand.\n"
		"The card should be successfully discarded.";
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change
	curTestExpectations.change_handCount[1] = -1; // Player 1's hand count should decrease by 1
	/* Note that despite the function's name, discardCard's job is to place the cards into the played set.
	** Later, during the cleanup phase, they will be moved to the actual discard pile. */
	curTestExpectations.change_playedCards = 1; // Player 1's played set should change due to the discard
	curTestExpectations.change_playedCardCount = 1; // Player 1's played count should increase by 1
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	for (i = 0; i < 5; i++) {
		pretestState.hand[1][i] = testHand[i]; // ensure player has five cards in hand
	}
	pretestState.handCount[1] = 5;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = discardCard(4, 1, &posttestState, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Discard from Middle of Hand (unittest4.c; discardCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState2P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (2P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Discard from Middle of Hand (unittest4.c; discardCard)";
	curTestExpectations.testDescription = "Player 1 of a 2-player game attempts to discard a middle card in their hand.\n"
		"The card should be successfully discarded.";
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change
	curTestExpectations.change_handCount[1] = -1; // Player 1's hand count should decrease by 1
	curTestExpectations.change_playedCards = 1; // Player 1's played set should change due to the discard
	curTestExpectations.change_playedCardCount = 1; // Player 1's played count should increase by 1
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	for (i = 0; i < 5; i++) {
		pretestState.hand[1][i] = testHand[i]; // ensure player has five cards in hand
	}
	pretestState.handCount[1] = 5;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = discardCard(2, 1, &posttestState, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Discard First Card in Hand (unittest4.c; discardCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState2P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (2P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Discard First Card in Hand (unittest4.c; discardCard)";
	curTestExpectations.testDescription = "Player 0 of a 2-player game attempts to discard the first card in their hand.\n"
		"The card should be successfully discarded.";
	curTestExpectations.change_hand[0] = 1; // Player 0's hand should change
	curTestExpectations.change_handCount[0] = -1; // Player 0's hand count should decrease by 1
	curTestExpectations.change_playedCards = 1; // Player 0's played set should change due to the discard
	curTestExpectations.change_playedCardCount = 1; // Player 0's played count should increase by 1
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	for (i = 0; i < 5; i++) {
		pretestState.hand[0][i] = testHand[i]; // ensure player has five cards in hand
	}
	pretestState.handCount[0] = 5;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = discardCard(0, 0, &posttestState, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Trash Card from Hand (unittest4.c; discardCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Trash Card from Hand (unittest4.c; discardCard)";
	curTestExpectations.testDescription = "Player 0 of a 3-player game attempts to trash a card from their hand.\n"
		"The card should be successfully trashed.";
	curTestExpectations.change_hand[0] = 1; // Player 0's hand should change
	curTestExpectations.change_handCount[0] = -1; // Player 0's hand count should decrease by 1
	// No changes to played set expected for this test, as card is expected to be trashed instead
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	for (i = 0; i < 5; i++) {
		pretestState.hand[0][i] = testHand[i]; // ensure player has five cards in hand
	}
	pretestState.handCount[0] = 5;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = discardCard(2, 0, &posttestState, 1);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);
}


int main(int argc, char *argv[])
{
	testDiscardCard();
	return 0;
}