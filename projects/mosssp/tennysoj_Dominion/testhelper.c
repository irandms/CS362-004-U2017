#include "testhelper.h"
#include <stdio.h>
#include "interface.h"

/* Given a change value (numerical, i.e. -2) and the expected change value, determines if the test passed or
** failed and outputs the results. Returns 0 when test passes and 1 when test fails. */
int reportNumerical(int change, int expectedChange, int preValue, int postValue, const char *description) {
	int returnVal = 0;
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

	// Report the title and description of the test case being evaluated
	printf("TEST CASE: %s\n", expectedChanges->testTitle);
	printf("Description: %s\n", expectedChanges->testDescription);

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
	printf("TEST CASE (%s) COMPLETE; TOTAL FAILED TESTS: %d\n\n\n", expectedChanges->testTitle, fails);

	return 0;
}
