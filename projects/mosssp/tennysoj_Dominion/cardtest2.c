#include "testhelper.h"

void testSmithy() {
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


	// TEST CASE: Drawing with Smithy (cardtest2.c; Smithy)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState2P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (2P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Drawing with Smithy (cardtest2.c; Smithy)";
	curTestExpectations.testDescription = "Player 1 of a 2-player game plays smithy.\n"
		"Three cards should be drawn from their deck.";
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change due to drawing three cards
	curTestExpectations.change_handCount[1] = 2; // Player 1's hand count should increase by 2 (+3 draw -1 discarding smithy)
	curTestExpectations.change_deckCount[1] = -3; // Player 1's deck count should decrease by 3
	curTestExpectations.change_playedCards = 1; // Player 1's played set should change due to use of discardCard
	curTestExpectations.change_playedCardCount = 1;
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	int testDeck1[5] = { copper, gold, estate, minion, cutpurse };
	for (i = 0; i < 5; i++) {
		pretestState.deck[1][i] = testDeck1[i];
	}
	pretestState.deckCount[1] = 5;
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = smithy;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(smithy, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Smithy With Empty Deck (cardtest2.c; Smithy)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Smithy With Empty Deck (cardtest2.c; Smithy)";
	curTestExpectations.testDescription = "Player 1 of a 3-player game plays smithy.\n"
		"Their deck is empty, but they have three discards. Those discards should move to the deck and be drawn.";
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change due to drawing three cards
	curTestExpectations.change_handCount[1] = 2; // Player 1's hand count should increase by 2 (+3 draw -1 discarding smithy)
	curTestExpectations.change_playedCards = 1; // Player 1's played set should change due to use of discardCard
	curTestExpectations.change_playedCardCount = 1;
	curTestExpectations.change_discard[1] = 1; // Player 1's discard pile should change due to shuffle
	curTestExpectations.change_discardCount[1] = -3; // All 3 cards in discard should be removed from discard pile
	curTestExpectations.change_deck[1] = 1; // Player 1's deck should change due to shuffle
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	int testDeck2[3] = { copper, gold, estate };
	for (i = 0; i < 3; i++) {
		pretestState.discard[1][i] = testDeck2[i];
	}
	pretestState.discardCount[1] = 3;
	pretestState.deckCount[1] = 0;
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = smithy;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(smithy, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Smithy With Empty Deck and Discards (cardtest2.c; Smithy)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

	// Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Smithy With Empty Deck and Discards (cardtest2.c; Smithy)";
	curTestExpectations.testDescription = "Player 1 of a 3-player game plays smithy.\n"
		"Their deck is empty, and they have no discards. No cards should be acquired.";
	curTestExpectations.change_playedCards = 1; // Player 1's played set should change due to use of discardCard
	curTestExpectations.change_playedCardCount = 1;
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change due to playing their smithy
	curTestExpectations.change_handCount[1] = -1; // Player 1's hand count should decrease by one (losing smithy)
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.discardCount[1] = 0;
	pretestState.deckCount[1] = 0;
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = smithy;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(smithy, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);


	// TEST CASE: Smithy With 1 Card in Deck, 1 Card in Discard (cardtest2.c; Smithy)
	memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations)); // zero out test's expectations
	memcpy(&pretestState, &baseState3P, sizeof(struct gameState)); // reset pretestState to be same as base gamestate (3P)

																   // Define this test case's expected gamestate changes by populating the expectation struct
	curTestExpectations.testTitle = "Smithy With 1 Card in Deck, 1 Card in Discard (cardtest2.c; Smithy)";
	curTestExpectations.testDescription = "Player 1 of a 3-player game plays smithy.\n"
		"Their deck has one card, and they have one discard. Both cards should be acquired.";
	curTestExpectations.change_playedCards = 1; // Player 1's played set should change due to use of discardCard
	curTestExpectations.change_playedCardCount = 1;
	curTestExpectations.change_hand[1] = 1; // Player 1's hand should change
	curTestExpectations.change_handCount[1] = 1; // Player 1's hand count should increase by 1 (+2 draw, -1 lose smithy)
	curTestExpectations.change_discard[1] = 1; // Player 1's discard should change due to shuffle
	curTestExpectations.change_discardCount[1] = -1; // One card is shuffled out of discard pile
	curTestExpectations.change_deck[1] = 1; // Player 1's deck should change due to shuffle
	curTestExpectations.change_deckCount[1] = -1; // Deck starts with 1 card and should end with 0
	curTestExpectations.expected_return = 0;

	// Modify the pretest gameState as needed for this particular test case
	pretestState.discardCount[1] = 1;
	pretestState.discard[1][0] = minion;
	pretestState.deckCount[1] = 1;
	pretestState.deck[1][0] = cutpurse;
	pretestState.whoseTurn = 1;
	pretestState.hand[1][0] = smithy;

	// Copy the finished pretest gameState into posttestState
	memcpy(&posttestState, &pretestState, sizeof(struct gameState));

	// Perform the test action, which will modify posttestState
	curTestExpectations.actual_return = cardEffect(smithy, 0, 0, 0, &posttestState, 0, 0);

	// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
	evaluateTest(&pretestState, &posttestState, &curTestExpectations);
}


int main(int argc, char *argv[])
{
	testSmithy();
	return 0;
}