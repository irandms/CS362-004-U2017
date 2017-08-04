#include "testhelper.h"

void testDrawCard() {
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


	// TEST CASE: Card Draw (unittest1.c; drawCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Card Draw (unittest1.c; drawCard)";
	curTestExpectations.testDescription = "A basic test of the drawCard function.\n" 
		"Player 0 attempts to draw a card at the beginning of a 3-player game.";
	// Note that drawCard doesn't actually remove an element from the deck, just decrements deckCount
	curTestExpectations.change_hand[0] = 1; // Player 0's hand should change due to drawing
	curTestExpectations.change_handCount[0] = 1; // Player 0's hand count should increase by 1
	curTestExpectations.change_deckCount[0] = -1; // Player 0's deck count should decrease by 1
	curTestExpectations.expected_return = 0;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = drawCard(0, &posttestState);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);



	// TEST CASE: Card Draw with Empty Deck (unittest1.c; drawCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState2P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (2P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Card Draw with Empty Deck (unittest1.c; drawCard)";
	curTestExpectations.testDescription = "Player 1 of a 2-player game attempts to draw a card while their deck is empty.\n"
		"Discarded cards should be added to the deck, which should be shuffled before allowing the draw.";
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change due to drawing
	curTestExpectations.change_handCount[1] = 1; // Player 1's hand count should increase by 1
	curTestExpectations.change_deck[1] = 1; // Player 1's deck should change due to shuffling
	curTestExpectations.change_deckCount[1] = 2; // Player 1's deck count should increase from 0 to 2 (3 discards - 1 draw)
	curTestExpectations.change_discardCount[1] = -3; // Player 1's discard count should drop from 3 to 0
	curTestExpectations.change_discard[1] = 1; // Player 1's discard pile should be changed (all cards moved to deck)
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.discardCount[1] = 3; // Adding 3 discards to player 1's discard pile
	pretestState.discard[1][0] = silver;
	pretestState.discard[1][1] = ambassador;
	pretestState.discard[1][2] = salvager;
	// For each of the 10 cards in player 1's deck from the base state, remove it (empty their deck)
	for (i = 0; i < 10; i++) {
		pretestState.deck[1][i] = 0;
	}
	pretestState.deckCount[1] = 0; // and set their deckCount to 0

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = drawCard(1, &posttestState);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);



	// TEST CASE: Card Draw with Both Deck and Discards Empty (unittest1.c; drawCard)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState4P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (4P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Card Draw with Both Deck and Discards Empty (unittest1.c; drawCard)";
	curTestExpectations.testDescription = "Player 1 of a 4-player game attempts to draw a card while their deck is empty.\n"
		"The attempt to draw should fail.";
	curTestExpectations.expected_return = -1;

	// Modify the pretest gameState as needed for this particular test case
	// For each of the 10 cards in player 1's deck from the base state, remove it (empty their deck)
	for (i = 0; i < 10; i++) {
		pretestState.deck[1][i] = 0;
	}
	pretestState.deckCount[1] = 0; // and set their deckCount to 0

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = drawCard(1, &posttestState);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);
}


int main(int argc, char *argv[])
{
	testDrawCard();
	return 0;
}