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
    int i;
    int handCount;
    int bonus;
    int treasure;
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
        for(treasure = copper; treasure <= gold; treasure++) {
            for(handCount = 0; handCount < MAX_HAND; handCount++) {
                for(bonus = 0; bonus <= MAX_BONUS; bonus++) {
                    initializeGame(MAX_PLAYERS, k, seed, &G);
                    for(i = 0; i <= handCount; i++) {
                        G.hand[p][i] = treasure; // Set all cards in hand to treasure
                        G.handCount[p] = handCount;
                    }
                    memcpy(&pre, &G, sizeof(struct gameState));

                    updateCoins(p, &G, bonus);

                    if(G.coins != (((treasure-3) * handCount) + bonus)) {
                        printf("FAILED TEST: Game does not have %d coins given %d of treasure type %d in hand with bonus of %d. Game has %d coins.\n", ((treasure-3) * handCount + bonus), handCount, treasure, bonus, G.coins);
                    }
                }
            }
        }
    }

    puts("Tests for updateCoins completed.");

    return 0;
}
