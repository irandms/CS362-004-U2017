CFLAGS = -Wall -fpic -coverage -lm -fprofile-arcs

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

unittest1: unittest1.c dominion.o
	gcc -o unittest1 -g unittest1.c dominion.o rngs.o $(CFLAGS)
	
unittest2: unittest2.c dominion.o
	gcc -o unittest2 -g unittest2.c dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c dominion.o
	gcc -o unittest3 -g unittest3.c dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c dominion.o
	gcc -o unittest4 -g unittest4.c dominion.o rngs.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o
	gcc -o cardtest1 -g cardtest1.c dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o
	gcc -o cardtest2 -g cardtest2.c dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o
	gcc -o cardtest3 -g cardtest3.c dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o
	gcc -o cardtest4 -g cardtest4.c dominion.o rngs.o $(CFLAGS)

randomtestcard1: randomtestcard1.c dominion.o
	gcc -o randomtestcard1 -g randomtestcard1.c dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c dominion.o
	gcc -o randomtestcard2 -g randomtestcard2.c dominion.o rngs.o $(CFLAGS)

randomtestadventurer: randomtestadventurer.c dominion.o
	gcc -o randomtestadventurer -g randomtestadventurer.c dominion.o rngs.o $(CFLAGS)

randomtestcard1.out: randomtestcard1
	echo "randomcardtest1.c:" > randomtestcard1.out
	./randomtestcard1 >> randomtestcard1.out
	gcov -abcd dominion.c >> randomtestcard1.out

randomtestcard2.out: randomtestcard2
	echo "randomcardtest2.c:" > randomtestcard2.out
	./randomtestcard2 >> randomtestcard2.out
	gcov -abcd dominion.c >> randomtestcard2.out

randomtestadventurer.out: randomtestadventurer
	echo "randomcardtest2.c:" > randomtestadventurer.out
	./randomtestadventurer >> randomtestadventurer.out
	gcov -abcd dominion.c >> randomtestadventurer.out

randomtestresults.out: randomtestcard1 randomtestcard2 randomtestadventurer
	echo "Results for random card tests 1-3 generated in randomtestresults.out"
	echo "randomcardtest1.c:" > randomtestresults.out
	./randomtestcard1 >> randomtestresults.out
	gcov -abcd dominion.c >> randomtestresults.out
	echo "randomcardtest2.c:" >> randomtestresults.out
	./randomtestcard2 >> randomtestresults.out
	gcov -abcd dominion.c >> randomtestresults.out
	echo "randomcardadventurer.c:" >> randomtestresults.out
	./randomtestadventurer >> randomtestresults.out
	gcov -abcd dominion.c >> randomtestresults.out

unittestresults.out: unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 dominion.o
	echo "Results for unit tests 1-4 and card tests 1-4 generated in unittestresults.out"
	echo "unittest1.c:" > unittestresults.out
	./unittest1 >> unittestresults.out
	gcov -abcd dominion.c >> unittestresults.out
	echo "unittest2.c:" >> unittestresults.out
	./unittest2 >> unittestresults.out 
	gcov -abcd dominion.c >> unittestresults.out
	echo "unittest3.c:" >> unittestresults.out
	./unittest3 >> unittestresults.out 
	gcov -abcd dominion.c >> unittestresults.out
	echo "unittest4.c:" >> unittestresults.out
	./unittest4 >> unittestresults.out
	gcov -abcd dominion.c >> unittestresults.out
	echo "cardtest1.c:" >> unittestresults.out
	./cardtest1 >> unittestresults.out
	gcov -abcd dominion.c >> unittestresults.out
	echo "cardtest2.c:" >> unittestresults.out
	./cardtest2 >> unittestresults.out
	gcov -abcd dominion.c >> unittestresults.out
	echo "cardtest3.c:" >> unittestresults.out
	./cardtest3 >> unittestresults.out
	gcov -abcd dominion.c >> unittestresults.out
	echo "cardtest4.c:" >> unittestresults.out
	./cardtest4 >> unittestresults.out
	gcov -abcd dominion.c >> unittestresults.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe badTestDrawCard unittest1 unittest2 unittest3 unittest4 cardtest1 cardtest2 cardtest3 cardtest4 randomtestcard1 randomtestcard2 randomtestadventurer
