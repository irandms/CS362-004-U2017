#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main() {
    struct gameState G;
    struct gameState pre;

    int seed = 1234;
    int p;
    int i;
    int trashFlag;
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

    // Call discardCard for all players
    for(p = 0; p < MAX_PLAYERS; p++) 
    {
        // Call discardCard for all varieties of trashFlag
        for(trashFlag = 0; trashFlag < 2; trashFlag++) {
            // Call discardCard for all possible positions of the hand from 0 to 4.
            for(i = 0; i < 5; i++) {
                initializeGame(MAX_PLAYERS, k, seed, &G);
                memcpy(&pre, &G, sizeof(struct gameState));
                // Call discardCard
                discardCard(i, p, &G, trashFlag);
    
                // Assert that changes we expected to happen actually did happen
                if(G.handCount[p] != ( pre.handCount[p] - 1)) {
                    printf("FAILED TEST: player %d's handCount did not decrease when discarding card with hand position %d.\n", p, i);
                }
                // If card was not trashed, we expect playedCards to have changed
                if(!trashFlag) {
                    if(G.playedCardCount != ( pre.playedCardCount + 1)) {
                        printf("FAILED TEST: played cards count did not increase when discarding card in handpos %d from player %d's hand.\n", i, p);
                    }
                    if(G.playedCards[G.playedCardCount - 1] != pre.hand[p][i]) {
                        printf("FAILED TEST: played cards does not contain card at handpos %d from player %d's hand.\n", i, p);
                    }
                }
                else {
                    if(G.playedCardCount != pre.playedCardCount) {
                        printf("FAILED TEST: played cards changed when trashing card in handpos %d from player %d's hand.\n", i, p);
                    }
                }
                if(G.handCount[p] != ( pre.handCount[p] - 1)) {
                    printf("FAILED TEST: handCount did not decrease when discarding card from handpos %d in %d's hand.\n", i, p);
                }
                // If the card discarded wasn't their last card, their last playable card should've swapped locations with their discarded card at handPos.
                if(i != (pre.handCount[p] - 1)) {
                    if(G.hand[p][i] != pre.hand[p][pre.handCount[p]-1]) {
                        printf("FAILED TEST: post gamestate's discarded card was not replaced with the last card in hand prior to discard.\n"); 
                        printf("post: %d pre: %d\n", G.hand[p][i], pre.hand[p][pre.handCount[p]-1]);
                        printf("player: %d, trashflag: %d\n", p, trashFlag);
                        printf("handpos: %d\n", i);
                    }
                }
            }

            // Call discardCard to discard an entire hand starting from the first pos
            memset(&G, 0, sizeof(struct gameState)); // Reset game state
            initializeGame(MAX_PLAYERS, k, seed, &G);
            for(i = 0; i < 5; i++) {
                memcpy(&pre, &G, sizeof(struct gameState));
                // Call discardCard
                discardCard(0, p, &G, trashFlag);

                // Assert that changes we expected to happen actually did happen
                if(G.handCount[p] != ( pre.handCount[p] - 1)) {
                    printf("FAILED TEST: player %d's handCount did not decrease when discarding card with hand position %d.\n", p, i);
                }
                // If card was not trashed, we expect playedCards to have changed
                if(!trashFlag) {
                    if(G.playedCardCount != ( pre.playedCardCount + 1)) {
                        printf("FAILED TEST: played cards count did not increase when discarding card in handpos %d from player %d's hand.\n", i, p);
                    }
                    if(G.playedCards[G.playedCardCount - 1] != pre.hand[p][0]) {
                        printf("FAILED TEST: played cards does not contain card at handpos %d from player %d's hand.\n", i, p);
                    }
                }
                else {
                    if(G.playedCardCount != pre.playedCardCount) {
                        printf("FAILED TEST: played cards changed when trashing card in handpos %d from player %d's hand.\n", i, p);
                    }
                }
                if(G.handCount[p] != ( pre.handCount[p] - 1)) {
                    printf("FAILED TEST: handCount did not decrease when discarding card from handpos %d in %d's hand.\n", i, p);
                }
                // If the card discarded wasn't their last card, their last playable card should've swapped locations with their discarded card at handPos.
                if(0 != (pre.handCount[p] - 1)) {
                    if(G.hand[p][0] != pre.hand[p][pre.handCount[p]-1]) {
                        printf("FAILED TEST: post gamestate's discarded card was not replaced with the last card in hand prior to discard.\n"); 
                        printf("post: %d pre: %d\n", G.hand[p][0], pre.hand[p][pre.handCount[p]-1]);
                        printf("player: %d, trashflag: %d\n", p, trashFlag);
                        printf("handpos: %d\n", 0);
                        printf("iteration: %d\n", i);
                    }
                }
            }
        }
    }

    puts("Tests for discardCard completed.");

    return 0;
}
