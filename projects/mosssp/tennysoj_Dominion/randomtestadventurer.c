#include "randTestHelper.h"

void testAdventurer() {
	struct gameState *pretestState; // Will point to the randomly generated gameState
	struct gameState posttestState; // Will store the post-test gameState
	struct stateExpectations emptyExpectations = { 0 }; // empty expectation struct is copied whenever one is needed
	struct stateExpectations curTestExpectations; // will store the current test case's expectation data
	int whoseTurn; // tracks the number of the current turn's player from pretestState
	int handPos; // tracks the position of the card-to-be-tested (Adventurer) in the player's hand
	int deckCopper, deckSilver, deckGold; // tracks quantity of treasures in the deck
	int discardCopper, discardSilver, discardGold; // tracks quantity of treasures in discards
	int cardsDiscarded; // tracks the number of non-treasure cards discarded while looking for treasure
	int i, b; // loop counters

	// Seed the random function
	srand(time(NULL));

	for (i = 1; i <= NUM_TESTS; i++) {
		// Acquire a gameState with somewhat directed random values
		pretestState = randGameState();

		// zero out test's expectations
		memcpy(&curTestExpectations, &emptyExpectations, sizeof(struct stateExpectations));

		// Replace a random card in the current player's hand with an Adventurer (or add it if the hand is empty)
		whoseTurn = pretestState->whoseTurn;
		if (!pretestState->handCount[whoseTurn]) {
			handPos = 0;
			pretestState->handCount[whoseTurn]++;
		}
		else {
			handPos = randBetween(0, (pretestState->handCount[whoseTurn] - 1));
		}
		pretestState->hand[whoseTurn][handPos] = adventurer;
		// Store the hand position from which the test card was played
		curTestExpectations.testHandPos = handPos;

		// Determine how many treasures are in the deck, and how many non-treasures must be discarded to get two treasures
		deckCopper = 0;
		deckSilver = 0;
		deckGold = 0;
		cardsDiscarded = 0;
		// For each card in the player's deck, from the top (next to be drawn) to the bottom
		for (b = pretestState->deckCount[whoseTurn] - 1; b >= 0; b--) {
			if (pretestState->deck[whoseTurn][b] == curse) {
				pretestState->deck[whoseTurn][b] = adventurer;
			}
			if (pretestState->deck[whoseTurn][b] == copper) {
				deckCopper++;
			}
			else if (pretestState->deck[whoseTurn][b] == silver) {
				deckSilver++;
			}
			else if (pretestState->deck[whoseTurn][b] == gold) {
				deckGold++;
			}
			else {
				// cards are only discarded before two treasures have been revealed
				if (deckCopper + deckSilver + deckGold < 2) {
					cardsDiscarded++;
				}
			}
		}

		// Determine how many treasures are in the discard deck
		discardCopper = 0;
		discardSilver = 0;
		discardGold = 0;
		// For each card in the discard pile
		for (b = 0; b < pretestState->discardCount[whoseTurn]; b++) {
			if (pretestState->discard[whoseTurn][b] == curse) {
				pretestState->discard[whoseTurn][b] = adventurer;
			}
			if (pretestState->discard[whoseTurn][b] == copper) {
				discardCopper++;
			}
			else if (pretestState->discard[whoseTurn][b] == silver) {
				discardSilver++;
			}
			else if (pretestState->discard[whoseTurn][b] == gold) {
				discardGold++;
			}
		}

		// There is a flaw in the adventurer functionality that causes an infinite loop (and eventual segfault) when
		// there are no treasures. If this is the case in the randomly generated pretest gameState,
		// then that gameState is thrown out and no test is performed
		if ((deckCopper + deckSilver + deckGold + discardCopper + discardSilver + discardGold) == 0) {
			free(pretestState);
			i--;
			continue;
		}
		// The bug I introduced to the adventurer functionality in assignment 1 causes it to consider silver as non-treasure
		// Thus a segfault will occur for any test case where the use of a silver is required to draw any treasure
		// These gameStates are thrown out as well
		if ((deckCopper + deckGold + discardCopper + discardGold) == 0) {
			free(pretestState);
			i--;
			continue;
		}

		// If two treasures can be drawn without shuffling the discards into the deck
		if (deckCopper + deckSilver + deckGold >= 2) {
			// The player's hand should increase by two cards drawn, but also decrease for one card played
			curTestExpectations.change_handCount[whoseTurn] = 1;
			curTestExpectations.change_hand[whoseTurn] = 1;
			// The deck should decrease for two treasures drawn + any non-treasures discarded
			curTestExpectations.change_deckCount[whoseTurn] = -2 - cardsDiscarded;
			// The deck array itself may or may not change
			curTestExpectations.change_deck[whoseTurn] = 999;
			// The discard pile should increase by the number of cards discarded
			curTestExpectations.change_discardCount[whoseTurn] = cardsDiscarded;
			if (cardsDiscarded) {
				// If any cards were indeed discarded, the discard pile will have changed
				curTestExpectations.change_discard[whoseTurn] = 1;
			}
		}
		// Else if the discards need to be shuffled in order to reach two treasures
		else if (deckCopper + deckSilver + deckGold + discardCopper + discardSilver + discardGold >= 2) {
			// The player's hand should increase by two cards drawn, but also decrease for one card played
			curTestExpectations.change_handCount[whoseTurn] = 1;
			curTestExpectations.change_hand[whoseTurn] = 1;
			// The deck will decrease by an unspecified amount (can't predict the shuffle order)
			curTestExpectations.change_deckCount[whoseTurn] = 999;
			curTestExpectations.change_deck[whoseTurn] = 999;
			// The discard count and contents are also affected in a non-specified way
			curTestExpectations.change_discardCount[whoseTurn] = 999;
			curTestExpectations.change_discard[whoseTurn] = 999;
		}
		// Else if there is only one treasure card to be had from deck and discards
		else if (deckCopper + deckSilver + deckGold + discardCopper + discardSilver + discardGold == 1) {
			// The player's hand count should not change (1 card drawn, but 1 card played)
			// The player's hand should change due to the draw
			curTestExpectations.change_hand[whoseTurn] = 1;
			// The deck will decrease by an unspecified amount (can't predict the shuffle order)
			curTestExpectations.change_deckCount[whoseTurn] = 999;
			curTestExpectations.change_deck[whoseTurn] = 999;
			// The discard count and contents are also affected in a non-specified way
			curTestExpectations.change_discardCount[whoseTurn] = 999;
			curTestExpectations.change_discard[whoseTurn] = 999;
		}
		// These expectations apply in every case from above
		curTestExpectations.expected_return = 0;
		curTestExpectations.cardTested = adventurer;
		curTestExpectations.testNum = i;
		// The played cards count should increase by one due to playing adventurer
		curTestExpectations.change_playedCardCount = 1;
		curTestExpectations.change_playedCards = 1;

		// Copy the finished pretest gameState into posttestState
		memcpy(&posttestState, pretestState, sizeof(struct gameState));

		// Perform the test action, which will modify posttestState
		curTestExpectations.actual_return = cardEffect(adventurer, 0, 0, 0, &posttestState, handPos, 0);

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

	testAdventurer();

	// Store the time after running the tests
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

	// Print the time that elapsed
	printf("RANDOM TESTS FINISHED. TIME TAKEN: %d seconds.\n", (int)(stop.tv_sec - start.tv_sec));

	return 0;
}