#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TESTCARD "smithy"
#define MAX_TESTS 1000000

void getRandomKingdom(int *k, int size, int required_card) {
    memset(k, required_card, size); // all set to smithy
    int kingdom_set_cards = 1;
    int random_num_is_unique = 1;
    int i;
    int current_random_num;

    // Ensure there are no duplicate Kingdom cards
    // We will exit this loop if kingdom_set_cards is 10 via break construct
    while(1) {
        current_random_num = (rand() % 20) + 7; // From adventurer to last Kingdom card
        random_num_is_unique = 1;
        for(i = 0; i < 10; i++) {
            if(k[i] == current_random_num) {
                random_num_is_unique = 0;
                break;
            }
        }

        if(random_num_is_unique) {
            k[kingdom_set_cards] = current_random_num;
            kingdom_set_cards++;
        }

        if(kingdom_set_cards == 10) {
            break;
        }
    }
}

void randomizeDecksHandsDiscards(struct gameState *G, int *k, int numPlayers) {
    int i, p;

    enum { KINGDOM_C, TREASURE_C, VICTORY_C , NUM_CARD_TYPES };

    // Randomize deck and hands
    for(p = 0; p < numPlayers; p++) {

        // Set deck and discard
        G->deckCount[p] = 1 + rand() % MAX_DECK;
        G->discardCount[p] = 1 + rand() % MAX_DECK;

        for(i = 0; i < G->deckCount[p]; i++) {
            int card_type = rand() % NUM_CARD_TYPES;
            switch(card_type) {
                case KINGDOM_C:
                    G->deck[p][i] = k[rand() % 10];
                    break;
                case TREASURE_C:
                    G->deck[p][i] = copper + rand() % 3;
                    break;
                case VICTORY_C:
                    G->deck[p][i] = estate + rand() % 3;
                default:
                    break;
            }
        }

        for(i = 0; i < G->discardCount[p]; i++) {
            int card_type = rand() % NUM_CARD_TYPES;
            switch(card_type) {
                case KINGDOM_C:
                    G->discard[p][i] = k[rand() % 10];
                    break;
                case TREASURE_C:
                    G->discard[p][i] = copper + rand() % 3;
                    break;
                case VICTORY_C:
                    G->discard[p][i] = estate + rand() % 3;
                default:
                    break;
            }
        }

        // Set hands
        int handCount = rand() % G->deckCount[p];
        for(i = 0; i < handCount ; i++) {
            drawCard(p, G);
        }
    }
}

void randomInitialize(struct gameState *G, int seed) {
    int i,j;
    int numPlayers = 2 + rand() % 2;

    int k[10];
    getRandomKingdom(k, 10, smithy);

    memset(G, 0, sizeof(struct gameState));
    initializeGame(numPlayers, k, seed, G);

    // Randomize treasure supply values
    int cur_copper = G->supplyCount[copper];
    G->supplyCount[copper] -= rand() % (cur_copper+1);
    G->supplyCount[silver] -= rand() % 41;
    G->supplyCount[gold]  -= rand() % 31;

    // Randomize kingdom supply values
    for(i = adventurer; i <= treasure_map; i++) {
        for(j = 0; j < 10; j++) {
            if(k[j] == i) {
                G->supplyCount[i] = rand() % 11;
            }
        }
    }

    randomizeDecksHandsDiscards(G, k, numPlayers);
}

int main() {
    int seed = 1234;
    srand(seed);

    struct gameState G; 
    struct gameState pre; 
    int i;
    int r;
    int p;
    int preTotalCards;
    int postTotalCards;

    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int handPos = 0;
    int bonus = 0;

    int tests_done = 0;

    int totalCardsErrCount = 0;
    int failedToDrawErrorCount = 0;
    int drewTooManyErrorCount = 0;
    int errorCount = 0;

    for(tests_done = 0; tests_done < MAX_TESTS; tests_done++) {
        if( (tests_done % 100000) == 0 ) {
            printf("On test %d of %d\n", tests_done, MAX_TESTS);
        }

        randomInitialize(&G, seed);
        p = rand() % G.numPlayers;
        G.whoseTurn = p;
        if( G.handCount[p] > 0 ) {
            handPos = rand() % G.handCount[p];
            G.hand[p][handPos] = smithy;
        }
        memcpy(&pre, &G, sizeof(struct gameState));

        // TESTS

        r = cardEffect(smithy, choice1, choice2, choice3, &G, handPos, &bonus);

        if(r != 0) {
            printf("FAILED TEST: playing card %s resulted in nonzero return code, should always return 0.\n", TESTCARD);
            printf("    Test #%d\n", tests_done);
            errorCount++;
        }

        if(G.handCount[p] != pre.handCount[p] + 2) {
            if(drewTooManyErrorCount < 10) {
                printf("FAILED TEST: player %d's handCount is %d after playing %s. Expected: %d\n", p, G.handCount[p], TESTCARD, pre.handCount[p] + 2);
                printf("    Test #%d\n", tests_done);
                drewTooManyErrorCount++;
            }
            errorCount++;
        }

        preTotalCards = pre.handCount[p] + pre.deckCount[p] + pre.discardCount[p] + pre.playedCardCount;
        postTotalCards = G.handCount[p] + G.deckCount[p] + G.discardCount[p] + G.playedCardCount;

        if(preTotalCards != postTotalCards) {
            if(totalCardsErrCount < 10) {
                printf("FAILED TEST: playing card %s changed the total number of cards in player %d's deck, hand, and discard.\n", TESTCARD, p);
                printf("    had %d cards before playing %s. post: %d\n", preTotalCards, TESTCARD, postTotalCards);
                printf("    Test #%d\n", tests_done);
                totalCardsErrCount++;
            }
            errorCount++;
        }

        // Validate that other players' data is unchanged.
        for(i = 0; i < G.numPlayers; i++) {
            if(i != p) {
                if(G.handCount[i] != pre.handCount[i]) {
                    printf("FAILED TEST: handCount for player %d who did not play %s changed after player %d played %s.\n", i, TESTCARD, p, TESTCARD);
                    printf("    Test #%d\n", tests_done);
                    errorCount++;
                }
                if(G.deckCount[i] != pre.deckCount[i]) {
                    printf("FAILED TEST: deckCount for player %d who did not play %s changed after player %d played %s.\n", i, TESTCARD, p, TESTCARD);
                    printf("    Test #%d\n", tests_done);
                    errorCount++;
                }
                if(G.discardCount[i] != pre.discardCount[i]) {
                    printf("FAILED TEST: discardCount for player %d who did not play %s changed after player %d played %s.\n", i, TESTCARD, p, TESTCARD);
                    printf("    Test #%d\n", tests_done);
                    errorCount++;
                }
            }
        }
    }

    printf("Tests for %s card completed. %d total errors, %d suppressed.", TESTCARD, errorCount, errorCount - totalCardsErrCount - failedToDrawErrorCount - drewTooManyErrorCount);

    return 0;
}
