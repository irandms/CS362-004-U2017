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

#define TESTCARD "adventurer"

int main() {
    int seed = 1234;
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int bonus = 0;
    int p;
    int r;
    int i;
    int handPos = 0;
    int numPlayers = 2;
    int treasureCountPre;
    int treasureCountPost;
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
            if(emptyDeck) {
                G.deckCount[p] = 0;
            }
            memcpy(&pre, &G, sizeof(struct gameState));

            treasureCountPre = 0;
            for(i = 0; i < G.handCount[p]; i++) {
                switch(G.hand[p][i]) {
                    case copper:
                    case silver:
                    case gold:
                        treasureCountPre++;
                    default:
                        break;
                }
            }

            r = cardEffect(adventurer, choice1, choice2, choice3, &G, handPos, &bonus);

            treasureCountPost = 0;
            for(i = 0; i < G.handCount[p]; i++) {
                switch(G.hand[p][i]) {
                    case copper:
                    case silver:
                    case gold:
                        treasureCountPost++;
                    default:
                        break;
                }
            }

            if(treasureCountPost != treasureCountPre + 2 && !emptyDeck) {
                printf("FAILED TEST: playing card adventurer failed to draw 2 treasure cards in fresh game.\n");
            }

            if(G.handCount[p] != pre.handCount[p] + 2) {
                printf("FAILED TEST: playing card adventurer drew more than 2 cards after discarding non-treasures.\n");
                printf("    pre: %d post: %d\n", pre.handCount[p], G.handCount[p]);
                printf("    emptyDeck: %d\n", emptyDeck);
            }

            if(r != 0) {
                printf("FAILED TEST: playing card adventurer resulted in nonzero return code, should always return 0.\n");
            }

            preTotalCards = pre.handCount[p] + pre.deckCount[p] + pre.discardCount[p];
            postTotalCards = G.handCount[p] + G.deckCount[p] + G.discardCount[p];

            if(preTotalCards != postTotalCards) {
                printf("FAILED TEST: playing card adventurer changed the total number of cards in player %d's deck, hand, and discard.\n", p);
                printf("    had %d cards before playing adventurer. post: %d\n", preTotalCards, postTotalCards);
            }

            // Dont check that the card at handPos is no longer Smithy,
            // as it is possible to draw a new Smithy card and have it
            // swapped to that location.

            // Validate that other players' data is unchanged.
            for(i = 0; i < numPlayers; i++) {
                if(i != p) {
                    if(G.handCount[i] != pre.handCount[i]) {
                        printf("FAILED TEST: handCount for player %d who did not play %s changed after player %d played %s.\n", i, TESTCARD, p, TESTCARD);
                    }
                    if(G.deckCount[i] != pre.deckCount[i]) {
                        printf("FAILED TEST: deckCount for player %d who did not play %s changed after player %d played %s.\n", i, TESTCARD, p, TESTCARD);
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


