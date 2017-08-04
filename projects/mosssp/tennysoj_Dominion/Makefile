CFLAGS = -Wall -fpic -coverage -lm

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


player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

unittestresults.out: dominion.o interface.c rngs.o unittest1.c unittest2.c unittest3.c unittest4.c cardtest1.c cardtest2.c cardtest3.c cardtest4.c testhelper.c
	echo "***** LAUNCHING UNIT TESTS *****" > unittestresults.out
	
	echo "unittest1.c:" >> unittestresults.out
	gcc -o unittest1 dominion.o rngs.o interface.c unittest1.c testhelper.c $(CFLAGS)
	./unittest1 >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

	echo "unittest2.c:" >> unittestresults.out
	gcc -o unittest2 dominion.o rngs.o interface.c unittest2.c testhelper.c $(CFLAGS)
	./unittest2 >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

	echo "unittest3.c:" >> unittestresults.out
	gcc -o unittest3 dominion.o rngs.o interface.c unittest3.c testhelper.c $(CFLAGS)
	./unittest3 >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

	echo "unittest4.c:" >> unittestresults.out
	gcc -o unittest4 dominion.o rngs.o interface.c unittest4.c testhelper.c $(CFLAGS)
	./unittest4 >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

	echo "cardtest1.c:" >> unittestresults.out
	gcc -o cardtest1 dominion.o rngs.o interface.c cardtest1.c testhelper.c $(CFLAGS)
	./cardtest1 >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

	echo "cardtest2.c:" >> unittestresults.out
	gcc -o cardtest2 dominion.o rngs.o interface.c cardtest2.c testhelper.c $(CFLAGS)
	./cardtest2 >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

	echo "cardtest3.c:" >> unittestresults.out
	gcc -o cardtest3 dominion.o rngs.o interface.c cardtest3.c testhelper.c $(CFLAGS)
	./cardtest3 >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

	echo "cardtest4.c:" >> unittestresults.out
	gcc -o cardtest4 dominion.o rngs.o interface.c cardtest4.c testhelper.c $(CFLAGS)
	./cardtest4 >> unittestresults.out
	gcov -b -c dominion.c >> unittestresults.out
	cat dominion.c.gcov >> unittestresults.out

randomtestcard1.out: dominion.o interface.c rngs.o randomtestcard1.c 
	echo "***** LAUNCHING RANDOM TESTS: SMITHY *****" > randomtestcard1.out
	gcc -o randomtestcard1 dominion.o rngs.o interface.c randomtestcard1.c $(CFLAGS)
	./randomtestcard1 >> randomtestcard1.out
	gcov -b -c dominion.c >> randomtestcard1.out
	cat dominion.c.gcov >> randomtestcard1.out

randomtestcard2.out: dominion.o interface.c rngs.o randomtestcard2.c 
	echo "***** LAUNCHING RANDOM TESTS: VILLAGE *****" > randomtestcard2.out
	gcc -o randomtestcard2 dominion.o rngs.o interface.c randomtestcard2.c $(CFLAGS)
	./randomtestcard2 >> randomtestcard2.out
	gcov -b -c dominion.c >> randomtestcard2.out
	cat dominion.c.gcov >> randomtestcard2.out

randomtestadventurer.out: dominion.o interface.c rngs.o randomtestadventurer.c 
	echo "***** LAUNCHING RANDOM TESTS: ADVENTURER *****" > randomtestadventurer.out
	gcc -o randomtestadventurer dominion.o rngs.o interface.c randomtestadventurer.c $(CFLAGS)
	./randomtestadventurer >> randomtestadventurer.out
	gcov -b -c dominion.c >> randomtestadventurer.out
	cat dominion.c.gcov >> randomtestadventurer.out

all: playdom player testDrawCard testBuyCard badTestDrawCard

clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
