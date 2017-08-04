/*
 * cardtest2.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "council_room"

int main() {
    int seed = 1234;
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int p;
    int r;
    int i;
    int numPlayers = 2;
    int emptyDeck;
    int preTotalCards;
    int postTotalCards;

	struct gameState G, pre;
	int k[10] = {adventurer, 
	             embargo, 
	             village, 
	             minion, 
	             mine, 
	             cutpurse,
	             sea_hag, 
	             tribute, 
	             smithy, 
	             council_room
	            };

	// initialize a game state and player cards
	for(p = 0; p < numPlayers; p++) {
	    for(emptyDeck = 0; emptyDeck < 2; emptyDeck++) {
            memset(&G, 0, sizeof(struct gameState));
            initializeGame(numPlayers, k, seed, &G);

            G.whoseTurn = p;
            G.hand[p][0] = council_room; 
            if(emptyDeck) {
                G.deckCount[p] = 0;
            }

            memcpy(&pre, &G, sizeof(struct gameState));

            r = playCard(0, choice1, choice2, choice3, &G);

            if(G.handCount[p] != (pre.handCount[p] + 3) && !emptyDeck) {
                printf("FAILED TEST: playing card %s did not changeplayer %d's handCount. They should have drawn 4 cards and discarded village for a net +3 change.\n", TESTCARD, p);
                printf("    pre: %d post: %d\n", pre.handCount[p], G.handCount[p]);
                printf("    emptyDeck: %d\n", emptyDeck);
            }

            if(G.numBuys != pre.numBuys + 1) {
                printf("FAILED TEST: playing card %s did not result in numBuys increasing by 1.\n", TESTCARD);
                printf("    pre: %d post: %d\n", pre.numBuys, G.numBuys);
            }

            if(r != 0) {
                printf("FAILED TEST: playing card %s resulted in nonzero return code, should always return 0.\n", TESTCARD);
            }

            preTotalCards = pre.handCount[p] + pre.deckCount[p] + pre.discardCount[p] + pre.playedCardCount;
            postTotalCards = G.handCount[p] + G.deckCount[p] + G.discardCount[p] + G.playedCardCount;

            if(preTotalCards != postTotalCards) {
                printf("FAILED TEST: playing card %s changed the total number of cards in player %d's deck, hand, and discard.\n", TESTCARD, p);
                printf("    had %d cards before playing %s. post: %d\n", preTotalCards, TESTCARD, postTotalCards);
            }

            // Validate that other players' data is unchanged.
            for(i = 0; i < numPlayers; i++) {
                if(i != p) {
                    if(G.handCount[i] != pre.handCount[i] + 1) {
                        printf("FAILED TEST: handCount for player %d who did not play %s did not increase after player %d played %s.\n", i, TESTCARD, p, TESTCARD);
                    }
                    if(G.deckCount[i] != pre.deckCount[i] - 1) {
                        printf("FAILED TEST: deckCount for player %d who did not play %s did not decrease after player %d played %s.\n", i, TESTCARD, p, TESTCARD);
                    }
                    if(G.discardCount[i] != pre.discardCount[i]) {
                        printf("FAILED TEST: discardCount for player %d who did not play %s changed after player %d played %s.\n", i, TESTCARD, p, TESTCARD);
                    }
                }
            }
        }
    }

    printf("Tests for %s card completed.", TESTCARD);

	return 0;
}


