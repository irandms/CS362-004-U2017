#include "randTestHelper.h"

void testVillage() {
	struct gameState *pretestState; // Will point to the randomly generated gameState
	struct gameState posttestState; // Will store the post-test gameState
	struct stateExpectations emptyExpectations = { 0 }; // empty expectation struct is copied whenever one is needed
	struct stateExpectations curTestExpectations; // will store the current test case's expectation data
	int whoseTurn; // tracks the number of the current turn's player from pretestState
	int handPos; // tracks the position of the card-to-be-tested (Village) in the player's hand
	int cardsFromDeck, cardsFromDiscard; // tracks expected number of cards to be drawn from deck and hand
	int i; // loop counter

	// Seed the random function
	srand(time(NULL));

	for (i = 1; i <= NUM_TESTS; i++) {
		// Acquire a gameState with somewhat directed random values
		pretestState = randGameState();
		// zero out test's expectations
		memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations));

		// Replace a random card in the current player's hand with a Village (or add it if the hand is empty)
		whoseTurn = pretestState->whoseTurn;
		if (!pretestState->handCount[whoseTurn]) {
			handPos = 0;
			pretestState->handCount[whoseTurn]++;
		}
		else {
			handPos = randBetween(0, (pretestState->handCount[whoseTurn] - 1));
		}
		pretestState->hand[whoseTurn][handPos] = village;
		// Store the hand position from which the test card was played
		curTestExpectations.testHandPos = handPos;

		// Determine the expected changes to gameState when Village is played by the current player

		// If there is at least one card in the player's deck
		if (pretestState->deckCount[whoseTurn]) {
			// Then it should be drawn from the deck, and discards won't be touched
			cardsFromDeck = 1;
			cardsFromDiscard = 0;
		}
		// If there are no cards in the player's deck
		else {
			cardsFromDeck = 0;
			// If there is a card available in the discard pile
			if (pretestState->discardCount[whoseTurn]) {
				// Then it is taken from the discard pile (via shuffling into a new deck)
				cardsFromDiscard = 1;
			}
			// If there aren't any discards either
			else {
				// Then no cards will be drawn
				cardsFromDiscard = 0;
			}
		}

		// Store the expected changes to gameState after Smithy is played
		curTestExpectations.testNum = i;
		// The player's hand should change due to drawing
		curTestExpectations.change_hand[whoseTurn] = 1;
		// Hand count should increase by number of cards drawn, but decrease by 1 for the Village that is played
		curTestExpectations.change_handCount[whoseTurn] = (cardsFromDeck + cardsFromDiscard) - 1;
		// If at least one card was in the discard and then got shuffled into the deck
		if (cardsFromDiscard) {
			// Then both the discard pile and deck should have changed
			curTestExpectations.change_discard[whoseTurn] = 1;
			curTestExpectations.change_deck[whoseTurn] = 1;
			// All discards would have been shuffled into the deck
			curTestExpectations.change_discardCount[whoseTurn] = -1 * pretestState->discardCount[whoseTurn];
			// The discards that were shuffled into the deck but not drawn cause the deck count to increase
			curTestExpectations.change_deckCount[whoseTurn] = ((-1 * cardsFromDeck) +
				(pretestState->discardCount[whoseTurn] - cardsFromDiscard));
		}
		// If the discards were not shuffled into the deck
		else {
			// Deck count should decrease by number of cards taken from the deck
			curTestExpectations.change_deckCount[whoseTurn] = -1 * cardsFromDeck;
		}
		// Village should also always grant +2 actions
		curTestExpectations.change_numActions = 2;
		// The player's played set may or may not change
		curTestExpectations.change_playedCards = 999;
		// The player's played card count should increase by 1
		curTestExpectations.change_playedCardCount = 1;
		// The return value should be 0
		curTestExpectations.expected_return = 0;
		curTestExpectations.cardTested = village;

		// Copy the finished pretest gameState into posttestState
		memcpy(&posttestState, pretestState, sizeof(struct gameState));

		// Perform the test action, which will modify posttestState
		curTestExpectations.actual_return = cardEffect(village, 0, 0, 0, &posttestState, handPos, 0);

		// Submit the pretest and posttest states, along with the test case's expectations struct, for evaluation and reporting
		evaluateTest(pretestState, &posttestState, &curTestExpectations);

		// Free the gameState allocated and returned by randGameState()
		free(pretestState);
	}
}


int main(int argc, char *argv[])
{
	struct timespec start, stop; // used to measure how long running the tests takes

	// Store the time before running the tests
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);

	testVillage();

	// Store the time after running the tests
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

	// Print the time that elapsed
	printf("RANDOM TESTS FINISHED. TIME TAKEN: %d seconds.\n", (int)(stop.tv_sec - start.tv_sec));

	return 0;
}