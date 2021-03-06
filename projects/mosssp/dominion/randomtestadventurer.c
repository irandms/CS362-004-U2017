#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TESTCARD "adventurer"
#define MAX_TESTS 1000000

void getRandomKingdom(int *k, int size, int required_card) {
    memset(k, required_card, size); // all set to adventurer
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
    getRandomKingdom(k, 10, adventurer);

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

int countTreasures(struct gameState *G) {
    int treasureCount = 0;
    int i;

    for(i = 0; i < G->handCount[G->whoseTurn]; i++) {
        switch(G->hand[G->whoseTurn][i]) {
            case copper:
            case silver:
            case gold:
                treasureCount++;
            default:
                break;
        }
    }

    return treasureCount;
}

int main() {
    int seed = 1234;
    srand(seed);

    struct gameState G; 
    struct gameState pre; 
    int i;
    int r;
    int p;
    int treasureCountPre;
    int treasureCountPost;
    int preTotalCards;
    int postTotalCards;

    int choice1 = 0;
    int choice2 = 0;
    int choice3 = 0;
    int handPos = 0;
    int bonus = 0;
    int emptyDeck = 0;

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
        memcpy(&pre, &G, sizeof(struct gameState));

        // TESTS

        treasureCountPre = countTreasures(&G);
        r = cardEffect(adventurer, choice1, choice2, choice3, &G, handPos, &bonus);
        treasureCountPost = countTreasures(&G);

        if(treasureCountPost != treasureCountPre + 2 && !emptyDeck) {
            if(failedToDrawErrorCount < 10) {
                printf("FAILED TEST: playing card adventurer failed to draw 2 treasure cards in fresh game.\n");
                printf("    Test #%d\n", tests_done);
                failedToDrawErrorCount++;
            }
            errorCount++;
        }

        if(G.handCount[p] != pre.handCount[p] + 2) {
            if(drewTooManyErrorCount < 10) {
                printf("FAILED TEST: playing card adventurer drew more than 2 cards after discarding non-treasures.\n");
                printf("    pre: %d post: %d\n", pre.handCount[p], G.handCount[p]);
                printf("    emptyDeck: %d\n", emptyDeck);
                printf("    Test #%d\n", tests_done);
                drewTooManyErrorCount++;
            }
            errorCount++;
        }

        if(r != 0) {
            printf("FAILED TEST: playing card adventurer resulted in nonzero return code, should always return 0.\n");
            printf("    Test #%d\n", tests_done);
            errorCount++;
        }

        preTotalCards = pre.handCount[p] + pre.deckCount[p] + pre.discardCount[p];
        postTotalCards = G.handCount[p] + G.deckCount[p] + G.discardCount[p];

        if(preTotalCards != postTotalCards) {
            if(totalCardsErrCount < 10) {
                printf("FAILED TEST: playing card adventurer changed the total number of cards in player %d's deck, hand, and discard.\n", p);
                printf("    had %d cards before playing adventurer. post: %d\n", preTotalCards, postTotalCards);
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
