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
    int r;
    int i;
    int j;
    int card;
    int card_index;
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

    int not_in_k[10] = { minion,
                         steward,
                         tribute,
                         ambassador,
                         cutpurse,
                         embargo,
                         outpost,
                         salvager,
                         sea_hag,
                         treasure_map
                       };


    for(p = 0; p < MAX_PLAYERS; p++) 
    {
        //printf("TESTING gainCard adding supply/kingdom cards to players' hand, deck, and discard.\n");
        for(card_index = 0; card_index < 10; card_index++)
        {
            // Setting up the gameState for this test
            card = k[card_index];
            initializeGame(MAX_PLAYERS, k, seed, &G);
            memcpy(&pre, &G, sizeof(struct gameState));

            // calling gainCard
            gainCard(card, &G, 0, p);
            gainCard(card, &G, 1, p);
            gainCard(card, &G, 2, p);

            // executing what we expect gainCard to do
            pre.hand[p][pre.handCount[p]] = card;
            pre.handCount[p] += 1;
            pre.supplyCount[card] -= 1;

            pre.deck[p][pre.deckCount[p]] = card;
            pre.deckCount[p] += 1;
            pre.supplyCount[card] -= 1;

            pre.discard[p][pre.discardCount[p]] = card;
            pre.discardCount[p] += 1;
            pre.supplyCount[card] -= 1;

            // ASSERTIONS ON GAME STATE
            // Compare results for this player's hand
            for(i = 0; i < MAX_HAND; i++) {
                if( pre.hand[p][i] != G.hand[p][i] ) {
                    printf("pre: %d, post: %d\n", pre.hand[p][i], G.hand[p][i]);
                    printf("FAILED TEST: adding card %d to player %d's hand, at handpos %d.\n", card, p, i);
                }
                if( pre.handCount[p] != G.handCount[p] ) {
                    printf("FAILED TEST: adding card %d to player %d's hand, at handpos %d.\n", card, p, i);
                }
            }

            // Check results for this player's deck and discard
            for(i = 0; i < MAX_DECK; i++) {
                if( (pre.deck[p][i] != G.deck[p][i]) || (pre.deckCount[p] != G.deckCount[p]) ) {
                    printf("FAILED TEST: adding card %d to player %d's deck.\n", card, p);
                }
                if( (pre.discard[p][i] != G.discard[p][i]) || (pre.discardCount[p] != G.discardCount[p]) ) {
                    printf("FAILED TEST: adding card %d to player %d's discard.\n", card, p);
                }
            }
            
            // Check that other players' state is the same
            for(i = 0; i < MAX_PLAYERS; i++) {
                if( i != p ) {
                    for(j = 0; j < MAX_DECK; j++) {
                        if( pre.deck[i][j] != G.deck[i][j] ) {
                            printf("FAILED TEST: Player %d's deck was changed when player %d had card %d added to their hand.\n", i, p, card);
                        }
                        if( pre.discard[i][j] != G.discard[i][j] ) {
                            printf("FAILED TEST: Player %d's discard was changed when player %d had card %d added to their hand.\n", i, p, card);
                        }
                    }
                    for(j = 0; j < MAX_HAND; j++) {
                        if( pre.hand[i][j] != G.hand[i][j] ) {
                            printf("FAILED TEST: Player %d's hand was changed when player %d had card %d added to their hand.\n", i, p, card);
                        }
                    }
                }
            }
            
            // Check that the supply count has gone down
            if( pre.supplyCount[card] != G.supplyCount[card] ) {
                printf("FAILED TEST: Supply count for card %d did not decrease when added to player %d's hand.\n", card, p);
            }
        }

        //printf("TESTING gainCard adding supply/kingdom cards not in this game to players' hand, deck, and discard.\n");
        for(card_index = 0; card_index < 10; card_index++)
        {
            // Setting up the gameState for this test
            card = not_in_k[card_index];
            initializeGame(MAX_PLAYERS, k, seed, &G);
            memcpy(&pre, &G, sizeof(struct gameState));

            // calling gainCard
            r = gainCard(card, &G, 0, p);
            if ( r != -1 ) {
                printf("FAILED TEST: gainCard did not return -1 for an invalid card in this game.\n");
            }
            r = gainCard(card, &G, 1, p);
            if ( r != -1 ) {
                printf("FAILED TEST: gainCard did not return -1 for an invalid card in this game.\n");
            }
            r = gainCard(card, &G, 2, p);
            if ( r != -1 ) {
                printf("FAILED TEST: gainCard did not return -1 for an invalid card in this game.\n");
            }

            // executing what we expect gainCard to do
            // nothing

            // ASSERTIONS ON GAME STATE
            // Assert that everyone's hands, discards, and decks remain the same
            for(i = 0; i < MAX_PLAYERS; i++) {
                for(j = 0; j < MAX_HAND; j++) {
                    if( pre.hand[i][j] != G.hand[i][j] ) {
                        printf("FAILED TEST: player %d's hand changed when drawing card %c not in game.\n", p, card);
                    }
                }
                for(j = 0; j < MAX_DECK; j++) {
                    if( pre.deck[i][j] != G.deck[i][j] ) {
                        printf("FAILED TEST: player %d's hand changed when drawing card %c not in game.\n", p, card);
                    }
                    if( pre.discard[i][j] != G.discard[i][j] ) {
                        printf("FAILED TEST: player %d's hand changed when drawing card %c not in game.\n", p, card);
                    }
                }
            }
            
            // Check that the supply count has NOT gone down
            if( pre.supplyCount[card] != G.supplyCount[card]) {
                printf("FAILED TEST: Supply count for card %d changed when trying to add card to %d's hand.\n", card, p);
            }
        }

        // Test drawing estate/province/duchy cards and depleting a stack of cards in the supply.
        for(card = estate; card <= province; card++) {
            initializeGame(MAX_PLAYERS, k, seed, &G);
            for(i = 0; i < 12; i++) {
                r = gainCard(card, &G, 2, p);
                if(r != 0) {
                    printf("FAILED TEST: Failed to draw one of the 12 estate/duchy/province cards.\n");
                }
            }

            r = gainCard(card, &G, 2, p);
            if(r != -1) {
                printf("FAILED TEST: Attempt to draw a 13th estate/duchy/province card succeeded.\n");
            }
        }
        
        // Depleting a Kingdom card
        initializeGame(MAX_PLAYERS, k, seed, &G);
        memcpy(&pre, &G, sizeof(struct gameState));
        for(i = 0; i < 10; i++) {
            r = gainCard(adventurer, &G, 2, p);
        }
        r = gainCard(adventurer, &G, 2, p);
        if(r != -1) {
            printf("FAILED TEST: Attempt to draw a 11th supply card succeeded.\n");
        }
    }

    puts("Tests for gainCard completed.");

    return 0;
}
