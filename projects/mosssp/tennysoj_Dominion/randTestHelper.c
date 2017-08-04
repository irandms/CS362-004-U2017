#include "randTestHelper.h"
#include "interface.h"


// Helper function that returns a random integer between min and max (inclusive)
int randBetween(int min, int max) {
	return (rand() % (max + 1 - min)) + min;
}

// Returns a pointer to a gameState that has been allocated and populated with random values (with direction)
struct gameState* randGameState() {
	int i, b; // loop counters
	int randResult; // used to store values returned by randBetween

	// Allocate a new gamestate
	struct gameState *state = malloc(sizeof(struct gameState));
	// Equal chance of 2, 3, or 4 players
	state->numPlayers = randBetween(2, 4);
	// 30% chance the supply is empty
	if (randBetween(1, 10) < 4) {
		for (i = 0; i <= treasure_map; i++) {
			state->supplyCount[i] = 0;
		}
	}
	// 70% chance the supply is non-empty
	else {
		for (i = 0; i <= treasure_map; i++) {
			// Each supply card has a 50% chance of being above 0 in this case
			// And a 50% chance of a random supply quantity from 1 to 20
			randResult = randBetween(1, 40);
			if (randResult <= 20) {
				state->supplyCount[i] = randResult;
				// Each non-zero card stack in the supply has a 1% chance of having 1-3 embargo tokens
				if (randBetween(1, 100) == 100) {
					state->embargoTokens[i] = randBetween(1, 3);
				}
			}
			else {
				state->supplyCount[i] = 0;
			}
		}
	}
	// There is a 2% chance that an outpost has just been played
	if (randBetween(1, 100) > 98) {
		state->outpostPlayed = 1;
		// If an outpost has been played, there's a 50% chance that it is currently the outpost turn
		if (randBetween(1, 2) == 2) {
			state->outpostTurn = 1;
		}
		else {
			state->outpostTurn = 0;
		}
	}
	else {
		state->outpostPlayed = 0;
		state->outpostTurn = 0;
	}
	// Every player has an equal chance of having the current turn
	state->whoseTurn = randBetween(0, state->numPlayers - 1);
	// 70% chance that the phase is 0 (action), 15% chance phase it is 1, 15% chance it is 2
	if (randBetween(1, 10) < 4) {
		state->phase = 0;
	}
	else {
		state->phase = randBetween(1, 2);
	}
	// 50% chance there is 1 action remaining, 25% chance of none, 25% chance of 2-4
	if (randBetween(1, 2) == 2) {
		state->numActions = 1;
	}
	else if (randBetween(1, 2) == 2) {
		state->numActions = 0;
	}
	else {
		state->numActions = randBetween(2, 4);
	}
	// 30% chance of having 0 coins currently, 70% chance of having 1-15
	if (randBetween(1, 10) < 4) {
		state->coins = 0;
	}
	else {
		state->coins = randBetween(1, 15);
	}
	// 50% chance there is 1 buy remaining, 25% chance of none, 25% chance of 2-4
	if (randBetween(1, 2) == 2) {
		state->numBuys = 1;
	}
	else if (randBetween(1, 2) == 2) {
		state->numBuys = 0;
	}
	else {
		state->numBuys = randBetween(2, 4);
	}

	// For each player in the game, populate their hand, deck, and discards
	for (i = 0; i < state->numPlayers; i++) {
		// 25% chance the player's hand is empty
		if (randBetween(1, 4) == 4) {
			state->handCount[i] = 0;
		}
		// 75% chance the player has 1-25 cards in their hand
		else {
			state->handCount[i] = randBetween(1, 25);
			// For each card placed in the hand
			for (b = 0; b < state->handCount[i]; b++) {
				// 20% chance it is a treasure card
				randResult = randBetween(1, 100);
				if (randResult < 21) {
					state->hand[i][b] = randBetween(copper, gold);
				}
				// 20% chance it is a standard victory card
				else if (randResult < 41) {
					state->hand[i][b] = randBetween(estate, province);
				}
				// 1% chance it is a curse card
				else if (randResult == 100) {
					state->hand[i][b] = curse;
				}
				// 59% chance it is another of the available cards
				else {
					state->hand[i][b] = randBetween(adventurer, treasure_map);
				}
			}
		}

		// 25% chance the player's deck is empty
		if (randBetween(1, 4) == 4) {
			state->deckCount[i] = 0;
		}
		// 75% chance the player has 1-25 cards in their deck
		else {
			state->deckCount[i] = randBetween(1, 25);
			// For each card placed in the deck
			for (b = 0; b < state->deckCount[i]; b++) {
				// 20% chance it is a treasure card
				randResult = randBetween(1, 100);
				if (randResult < 21) {
					state->deck[i][b] = randBetween(copper, gold);
				}
				// 20% chance it is a standard victory card
				else if (randResult < 41) {
					state->deck[i][b] = randBetween(estate, province);
				}
				// 1% chance it is a curse card
				else if (randResult == 100) {
					state->deck[i][b] = curse;
				}
				// 59% chance it is another of the available cards
				else {
					state->deck[i][b] = randBetween(adventurer, treasure_map);
				}
			}
		}

		// 25% chance the player's discards is empty
		if (randBetween(1, 4) == 4) {
			state->discardCount[i] = 0;
		}
		// 75% chance the player has 1-25 cards in their discards
		else {
			state->discardCount[i] = randBetween(1, 25);
			// For each card placed in the hand
			for (b = 0; b < state->discardCount[i]; b++) {
				// 20% chance it is a treasure card
				randResult = randBetween(1, 100);
				if (randResult < 21) {
					state->discard[i][b] = randBetween(copper, gold);
				}
				// 20% chance it is a standard victory card
				else if (randResult < 41) {
					state->discard[i][b] = randBetween(estate, province);
				}
				// 1% chance it is a curse card
				else if (randResult == 100) {
					state->discard[i][b] = curse;
				}
				// 59% chance it is another of the available cards
				else {
					state->discard[i][b] = randBetween(adventurer, treasure_map);
				}
			}
		}
	} // End of loop for all players

	// 25% chance there are no currently played cards
	if (randBetween(1, 4) == 4) {
		state->playedCardCount = 0;
	}
	// 75% chance there are 1-10 currently played cards
	else {
		state->playedCardCount = randBetween(1, 10);
		// For each currently played card
		for (i = 0; i < state->playedCardCount; i++) {
			// 20% chance it is a treasure card
			randResult = randBetween(1, 100);
			if (randResult < 21) {
				state->playedCards[i] = randBetween(copper, gold);
			}
			// 80% chance it is another of the available cards (no standard victory cards or curses)
			else {
				state->playedCards[i] = randBetween(adventurer, treasure_map);
			}
		}
	}

	// All fields of the gameState have been populated
	return state;
}


/* Given a change value (numerical, i.e. -2) and the expected change value, determines if the test passed or
** failed and outputs the results. Returns 0 when test passes and 1 when test fails. */
int reportNumerical(int change, int expectedChange, int preValue, int postValue, const char *description) {
	int returnVal = 0;
	// If the expected change could not be specified (mainly due to being unable to predict post-shuffle card order)
	if (expectedChange == 999) {
		// Then no test is performed
		return 0;
	}
	// If the expected change and actual change match, test passes
	if (change == expectedChange) {
		if (!SHOW_PASSES) {
			return 0;
		}
		printf("%5s[ PASS ] ", "");
	}
	// If the expected change and the actual change do not match, test fails
	else {
		returnVal = 1;
		printf("%2s[ FAIL ] ", "");
	}
	// Print the description of this particular test
	printf("%s | ", description);
	// Print a summary of the expected change and actual change
	printf("Expected Change: %+d | Actual Change: %+d | ", expectedChange, change);
	// Print the actual values of the state field being tested (pretest -> posttest)
	printf("Field Value: %d -> %d\n", preValue, postValue);

	return returnVal;
}

/* Given a change value (0 = no change, 1 = change) and the expected change value, determines if the
** test passed or failed and outputs the results. Returns 0 when test passes and 1 when test fails. */
int reportChanged(int change, int expectedChange, const char *description) {
	int returnVal = 0;
	// If the expected change could not be specified (mainly due to being unable to predict post-shuffle card order)
	if (expectedChange == 999) {
		// Then no test is performed
		return 0;
	}
	// If the expected change and actual change match, test passes
	if (change == expectedChange) {
		if (!SHOW_PASSES) {
			return 0;
		}
		printf("%5s[ PASS ] ", "");
	}
	// If the expected change and the actual change do not match, test fails
	else {
		returnVal = 1;
		printf("%2s[ FAIL ] ", "");
	}
	// Print the description of this particular test
	printf("%s | ", description);
	// Print a summary of the expected change and actual change
	if (expectedChange == 1) {
		printf("Expected: Field Changed/Updated | ");
	}
	else {
		printf("Expected: No Change in Field | ");
	}
	if (change == 1) {
		printf("Actual: Field Changed/Updated\n");
	}
	else {
		printf("Actual: No Change in Field\n");
	}
	return returnVal;
}


/* Given two gamestates and a test's state change expectation struct, determines which state
** fields changed properly and which did not- printing out PASS/FAIL results for each pairing
** of a gameState field and the test's expected/desired change in that field */
int evaluateTest(struct gameState *pretestState, struct gameState *posttestState,
	struct stateExpectations *expectedChanges) {
	int change; // when two numerical gamestate fields are compared, stores the difference between them
	int fails = 0; // running total counting the tests that fail
	int i, j; // loop counters
	char useString[50], useString2[50]; // used to hold strings when needed

	// Print info about the test case
	printf("RANDOM TEST CASE %d\n", expectedChanges->testNum);
	// Print the hand position that the tested card is played from
	printf("Playing from hand position %d\n\n", expectedChanges->testHandPos);

	if (SHOW_EXTRA_INFO) {
		// Print extra info about the deck, hand, discard, and played card array contents for the current player
		printf("Current player's pre-test hand:\n");
		printHand(pretestState->whoseTurn, pretestState);
		printf("Current player's post-test hand:\n");
		printHand(posttestState->whoseTurn, posttestState);
		printf("Current player's pre-test deck:\n");
		printDeck(pretestState->whoseTurn, pretestState);
		printf("Current player's post-test deck:\n");
		printDeck(posttestState->whoseTurn, posttestState);
		printf("Current player's pre-test discards:\n");
		printDiscard(pretestState->whoseTurn, pretestState);
		printf("Current player's post-test discards:\n");
		printDiscard(posttestState->whoseTurn, posttestState);
		printf("Current player's pre-test played cards:\n");
		printPlayed(pretestState->whoseTurn, pretestState);
		printf("Current player's post-test played cards:\n");
		printPlayed(posttestState->whoseTurn, posttestState);
	}

	// Evaluate and report on changes to the game's state. Each field of gameState is tested individually:

	// Test change in whoseTurn
	change = posttestState->whoseTurn - pretestState->whoseTurn;
	fails += reportNumerical(change, expectedChanges->change_whoseTurn,
		pretestState->whoseTurn, posttestState->whoseTurn, "Current Turn Player");

	// Test change in phase number
	change = posttestState->phase - pretestState->phase;
	// This helper function returns 1 if the test fails, and outputs detailed results for viewing
	fails += reportNumerical(change, expectedChanges->change_phase,
		pretestState->phase, posttestState->phase, "Turn Phase");

	// Test change in number of actions
	change = posttestState->numActions - pretestState->numActions;
	fails += reportNumerical(change, expectedChanges->change_numActions,
		pretestState->numActions, posttestState->numActions, "Remaining Turn Actions");

	// Test change in number of coins this turn
	change = posttestState->coins - pretestState->coins;
	fails += reportNumerical(change, expectedChanges->change_coins,
		pretestState->coins, posttestState->coins, "Remaining Turn Coins");

	// Test change in number of buys this turn
	change = posttestState->numBuys - pretestState->numBuys;
	fails += reportNumerical(change, expectedChanges->change_numBuys,
		pretestState->numBuys, posttestState->numBuys, "Remaining Turn Buys");

	// Test change in set of cards played this turn
	change = 0; // 0 indicates that playedCards has not been altered
	// For each element in the playedCards array, compare pretest with posttest
	for (i = 0; i < MAX_DECK; i++) {
		// If two cards don't match, then playedCards must have been altered in some way
		if (pretestState->playedCards[i] != posttestState->playedCards[i]) {
			change = 1; // 1 indicates a change did take place
		}
		if (change == 1) {
			break;
		}
	}
	// This helper function returns 1 if the test fails, and outputs detailed results for viewing
	fails += reportChanged(change, expectedChanges->change_playedCards, "Array of Cards Played This Turn");

	// Test change in count of cards played this turn
	change = posttestState->playedCardCount - pretestState->playedCardCount;
	fails += reportNumerical(change, expectedChanges->change_playedCardCount,
		pretestState->playedCardCount, posttestState->playedCardCount, "Count of Cards Played This Turn");
	change = 0;

	// For each possible player (even if fewer players are actually playing)
	for (i = 0; i < MAX_PLAYERS; i++) {
		// Test the change status of the player's hand array
		for (j = 0; j < MAX_HAND; j++) {
			// If two cards don't match, then the hand must have been altered in some way
			if (pretestState->hand[i][j] != posttestState->hand[i][j]) {
				change = 1; // 1 indicates a change did take place
			}
			if (change == 1) {
				break;
			}
		}
		sprintf(useString, "Player %d's Hand Array", i);
		fails += reportChanged(change, expectedChanges->change_hand[i], useString);

		// Test change in the player's hand count
		change = posttestState->handCount[i] - pretestState->handCount[i];
		sprintf(useString, "Player %d's Hand Count", i);
		fails += reportNumerical(change, expectedChanges->change_handCount[i],
			pretestState->handCount[i], posttestState->handCount[i], useString);
		change = 0;

		// Test the change status of the player's deck array
		for (j = 0; j < MAX_DECK; j++) {
			// If two cards don't match, then the deck must have been altered in some way
			if (pretestState->deck[i][j] != posttestState->deck[i][j]) {
				change = 1; // 1 indicates a change did take place
			}
			if (change == 1) {
				break;
			}
		}
		sprintf(useString, "Player %d's Deck Array", i);
		fails += reportChanged(change, expectedChanges->change_deck[i], useString);

		// Test change in the player's deck count
		change = posttestState->deckCount[i] - pretestState->deckCount[i];
		sprintf(useString, "Player %d's Deck Count", i);
		fails += reportNumerical(change, expectedChanges->change_deckCount[i],
			pretestState->deckCount[i], posttestState->deckCount[i], useString);
		change = 0;

		// Test the change status of the player's discard array
		for (j = 0; j < MAX_DECK; j++) {
			// If two cards don't match, then the discard pile must have been altered in some way
			if (pretestState->discard[i][j] != posttestState->discard[i][j]) {
				change = 1; // 1 indicates a change did take place
			}
			if (change == 1) {
				break;
			}
		}
		sprintf(useString, "Player %d's Discard Array", i);
		fails += reportChanged(change, expectedChanges->change_discard[i], useString);

		// Test change in the player's deck count
		change = posttestState->discardCount[i] - pretestState->discardCount[i];
		sprintf(useString, "Player %d's Discard Count", i);
		fails += reportNumerical(change, expectedChanges->change_discardCount[i],
			pretestState->discardCount[i], posttestState->discardCount[i], useString);
		change = 0;
	}

	/* For each possible card (including cards not currently available for play), test supply
	** count and embargo token count */
	for (i = curse; i <= treasure_map; i++) {
		// Test supply count
		change = posttestState->supplyCount[i] - pretestState->supplyCount[i];
		cardNumToName(i, useString2); // puts name of card into useString2 (from interface.h)
		sprintf(useString, "Supply Count (%s)", useString2);
		fails += reportNumerical(change, expectedChanges->change_supplyCount[i],
			pretestState->supplyCount[i], posttestState->supplyCount[i], useString);
		// Test embargo token count
		change = posttestState->embargoTokens[i] - pretestState->embargoTokens[i];
		cardNumToName(i, useString2); // puts name of card into useString2 (from interface.h)
		sprintf(useString, "Embargo Count (%s)", useString2);
		fails += reportNumerical(change, expectedChanges->change_embargoTokens[i],
			pretestState->embargoTokens[i], posttestState->embargoTokens[i], useString);
	}

	// Test change in outpost flag
	change = posttestState->outpostPlayed - pretestState->outpostPlayed;
	fails += reportNumerical(change, expectedChanges->change_outpostPlayed,
		pretestState->outpostPlayed, posttestState->outpostPlayed, "Outpost Played Flag");

	// Test change in outpostTurn
	change = posttestState->outpostTurn - pretestState->outpostTurn;
	fails += reportNumerical(change, expectedChanges->change_outpostTurn,
		pretestState->outpostTurn, posttestState->outpostTurn, "Current Turn Outpost Flag");

	// Test change in numPlayers
	change = posttestState->numPlayers - pretestState->numPlayers;
	fails += reportNumerical(change, expectedChanges->change_numPlayers,
		pretestState->numPlayers, posttestState->numPlayers, "Number of Players");

	// Check expected and received return values
	if (expectedChanges->expected_return == expectedChanges->actual_return) {
		printf("%5s[ PASS ] ", "");
	}
	else {
		printf("%2s[ FAIL ] ", "");
		fails++;
	}
	printf("Return Value | ");
	// Print a summary of the expected return and actual return
	printf("Expected Return: %d | Actual Return: %d\n",
		expectedChanges->expected_return, expectedChanges->actual_return);

	// Output final count of failed tests
	printf("RANDOM TEST CASE %d COMPLETE; TOTAL FAILED TESTS: %d\n\n\n", expectedChanges->testNum, fails);

	return 0;
}