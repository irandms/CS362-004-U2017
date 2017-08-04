#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

#define MAX_BONUS 10

int main() {
    struct gameState G;
    struct gameState pre;

    int seed = 1234;
    int p;
    int r;
    int k[10] = { adventurer,
                  council_room,
                  feast,
                  gardens,
                  mine,
                  remodel,
                  smithy,
                  village,
                  baron,
                  great_hall
                };

    // Call updateCoins for all players
    for(p = 0; p < MAX_PLAYERS; p++) {
        r = initializeGame(MAX_PLAYERS, k, seed, &G);
        memcpy(&pre, &G, sizeof(struct gameState));

        r = drawCard(p, &G);
        if( r != 0 ) {
            printf("FAILED TEST: drawCard returned nonzero integer when drawing from freshly initialized game.\n");
        }

        if(G.handCount[p] != (pre.handCount[p] + 1)) {
            printf("FAILED TEST: player %d's handcount did not increase when drawCard was called.\n", p);
        }
        if(G.hand[p][G.handCount[p]-1] != pre.deck[p][pre.deckCount[p] - 1]) {
            printf("post: %d pre: %d\n", G.hand[p][G.handCount[p]-1], pre.deck[p][pre.deckCount[p]]);
            printf("FAILED TEST: player %d's last card in hand is not the same as the top card of the deck prior to calling drawCard.\n", p);
        }

        // Test empty deck and discard
        while(G.deckCount[p] > 0) {
            r = drawCard(p, &G);
        }

        r = drawCard(p, &G);

        if(G.discardCount[p] == 0) {
            if( r != -1) {
                printf("%d\n", G.discardCount[p]);
                printf("%d\n", G.deckCount[p]);
                printf("FAILED TEST: drawCard did not exit with error code -1 with empty deck and discard pile.\n");
            }
        }
        else if (G.discardCount[p] > 0) {
            if(G.handCount[p] != (pre.handCount[p] + 1)) {
                printf("FAILED TEST: player %d's handcount did not increase when drawCard was called.\n", p);
            }
            if(G.hand[p][G.handCount[p]-1] != pre.deck[p][pre.deckCount[p] - 1]) {
                printf("post: %d pre: %d\n", G.hand[p][G.handCount[p]-1], pre.deck[p][pre.deckCount[p]]);
                printf("FAILED TEST: player %d's last card in hand is not the same as the top card of the deck prior to calling drawCard.\n", p);
            }
        }

        // Reset gameState to test shuffling the discard pile into a deck
        memset(&G, 0, sizeof(struct gameState));
        r = initializeGame(MAX_PLAYERS, k, seed, &G);
        memcpy(&pre, &G, sizeof(struct gameState));

        // Test empty deck and discard
        while(G.deckCount[p] > 0) {
            r = drawCard(p, &G);
        }

        gainCard(adventurer, &G, 0, p);

        r = drawCard(p, &G);
        if(r != 0) {
            printf("FAILED TEST: drawCard did not return 0 when the discard pile should have been shuffled into a deck.\n");
        }
        if(G.hand[p][G.handCount[p] - 1] != adventurer) {
            printf("decj count: %d\n", G.deckCount[p]);
            printf("FAILED TEST: drawCard did not shuffle the one Adventurer card in the discard pile into the deck.\n");
        }
    }

    puts("Tests for drawCard completed.");

    return 0;
}
