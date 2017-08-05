/*
 * cardtest1.c
 */

#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "smithy"

int main() {
    int seed = 1234;
    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int p;
    int r;
    int i;
    int handPos;
    int numPlayers = 2;
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
        for(handPos = 0; handPos < 5; handPos++) {
            memset(&G, 0, sizeof(struct gameState));
            initializeGame(numPlayers, k, seed, &G);
            G.whoseTurn = p;
            G.hand[p][handPos] = smithy;

            memcpy(&pre, &G, sizeof(struct gameState));

            r = playCard(handPos, choice1, choice2, choice3, &G);

            if(r != 0) {
                printf("FAILED TEST: playing card smithy resulted in nonzero return code, smithy should always return 0.\n");
            }

            if(G.handCount[p] != pre.handCount[p] + 2) {
                printf("FAILED TEST: player %d's handCount is %d after playing smithy. Expected: %d\n", p, G.handCount[p], pre.handCount[p]+2);
            }

            preTotalCards = pre.handCount[p] + pre.deckCount[p] + pre.discardCount[p] + G.playedCardCount;
            postTotalCards = G.handCount[p] + G.deckCount[p] + G.discardCount[p] + G.playedCardCount;

            if(preTotalCards != postTotalCards) {
                printf("FAILED TEST: playing card %s changed the total number of cards in player %d's deck, hand, and discard.\n", TESTCARD, p);
                printf("    had %d cards before playing %s. post: %d\n", preTotalCards, TESTCARD, postTotalCards);
            }

            // Validate that other players' data is unchanged.
            for(i = 0; i < numPlayers; i++) {
                if(i != p) {
                    if(G.handCount[i] != pre.handCount[i]) {
                        printf("FAILED TEST: handCount for player %d who did not play Smithy changed after player %d played Smithy.\n", i, p);
                    }
                    if(G.deckCount[i] != pre.deckCount[i]) {
                        printf("FAILED TEST: deckCount for player %d who did not play Smithy changed after player %d played Smithy.\n", i, p);
                    }
                    if(G.discardCount[i] != pre.discardCount[i]) {
                        printf("FAILED TEST: discardCount for player %d who did not play Smithy changed after player %d played Smithy.\n", i, p);
                    }
                }
            }
        }
    }

    puts("Tests for Smithy card completed.");

    return 0;
}


