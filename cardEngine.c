#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
//#define ARRLENGTH(x)  (sizeof(x) / sizeof((x)[0]))
#define ARRLENGTH(x)  (*(&x + 1) - x)
#define i32 int32_t

struct Card
{
    char suit;
    char rank;
    i32 value;
};

struct CardPool
{
    i32 size;
    struct Card *cards;
};

struct Deck
{
    i32 size;
    i32 *cards;
};

struct CardPool BuildCardPool(i32 deckSize, i32 numOfSuits, i32 numOfValues,char *suits, char *ranks, i32 *values)
{
    struct CardPool pool;
    pool.size = deckSize;
    pool.cards = malloc(sizeof(struct Card) * deckSize);
    i32 cardNum = 0;
    for (i32 i = 0; i < numOfValues; i++)
    {
        for (i32 j = 0; j < numOfSuits; j++)
        {
            struct Card newCard = {suits[j], ranks[i], values[i]};
            pool.cards[cardNum] = newCard;
            cardNum++;
        }
    }

    return pool;
}

void ShuffleCards(i32 *cards, i32 numOfCards)
{
    if (numOfCards > 1)
    {
        struct timeval tv;
        gettimeofday(&tv, NULL);
        int usec = tv.tv_usec;
        srand48(usec);

        for (i32 i = numOfCards - 1; i > 0; i--)
        {
            i32 newPos = (unsigned int) (drand48()*(i+1));
            i32 temp = cards[newPos];
            cards[newPos] = cards[i];
            cards[i] = temp;
        }
    }
}

struct Deck CreateDeck(i32* pool, i32 cardNum, i32 deckSize, i32 offset)
{
    if (deckSize > cardNum) printf("Deck size is too big\n");

    struct Deck deck;
    deck.size = deckSize;
    deck.cards = malloc(sizeof(i32) * cardNum);

    for (i32 i = 0; i < cardNum; i++)
    {
        if (i >= offset && i < offset + deckSize)
            deck.cards[i - offset] = pool[i];
        else
        {
            deck.cards[i] = -1;
        }
    }

    return deck;
}

struct Deck* SetupDecks(struct CardPool pool, i32 deckNum, i32 deckSize)
{
    struct Deck* decks = malloc(sizeof(struct Deck) * deckNum);
    i32 cards[pool.size];
    for (i32 i = 0; i < pool.size; i++)
    {
        cards[i] = i;
    }

    ShuffleCards(cards, pool.size);

    for (i32 i = 0; i < deckNum; i++)
    {
        decks[i] = CreateDeck(cards, pool.size, deckSize, i * deckSize);
    }

    return decks;
}

void CleanUpDecks(struct Deck* decks, i32 deckNum)
{
    for (i32 i = 0; i < deckNum; i++)
    {
        free(decks[i].cards);
    }
    free(decks);
}

i32 CompareCardValue(i32 firstCardValue, i32 secondCardValue)
{
    if (firstCardValue == secondCardValue) return -1;

    return firstCardValue > secondCardValue;
}

void RunWar(struct CardPool cardPool)
{
    i32 deck1Size = cardPool.size / 2;
    i32 deck2Size = cardPool.size / 2;

    i32 maxTurns = 10000;

    struct Deck* decks = SetupDecks(cardPool, 2, cardPool.size / 2);

    for (i32 i = 0; i < deck1Size; i++)
    {
        printf("%i, ", cardPool.cards[decks[0].cards[i]].value);
    }
    printf("\n");
    for (i32 i = 0; i < deck1Size; i++)
    {
        printf("%i, ", cardPool.cards[decks[1].cards[i]].value);
    }
    printf("\n");

    i32 tie = 0;
    i32 win1 = 0;
    i32 win2 = 0;

    while (deck1Size > 0 && deck2Size > 0 && maxTurns > 0)
    {
        printf("Deck size 1: %i\n", deck1Size);
        printf("Deck size 2: %i\n", deck2Size);

        i32 deck1Card = cardPool.cards[decks[0].cards[0]].value;
        i32 deck2Card = cardPool.cards[decks[1].cards[0]].value;

        printf("Deck 1 Card: %c", cardPool.cards[decks[0].cards[0]].rank);
        printf("%c\n", cardPool.cards[decks[0].cards[0]].suit);
        printf("Deck 2 Card: %c", cardPool.cards[decks[1].cards[0]].rank);
        printf("%c\n\n", cardPool.cards[decks[1].cards[0]].suit);

        switch (CompareCardValue(deck1Card, deck2Card))
        {
            case -1:
                decks[0].cards[deck1Size] = decks[0].cards[0];
                decks[1].cards[deck2Size] = decks[1].cards[0];
                tie++;
                break;
            case 0: // second deck card is larger
                decks[1].cards[deck2Size + 0] = decks[1].cards[0];
                decks[1].cards[deck2Size + 1] = decks[0].cards[0];
                deck1Size--;
                deck2Size++;
                win2++;
                break;
            case 1: // first deck card is larger
                decks[0].cards[deck1Size + 0] = decks[0].cards[0];
                decks[0].cards[deck1Size + 1] = decks[1].cards[0];
                deck1Size++;
                deck2Size--;
                win1++;
                break;
        }

        for (i32 i = 0; i < cardPool.size; i++)
        {
            decks[0].cards[i] = decks[0].cards[i+1];
            decks[1].cards[i] = decks[1].cards[i+1];
        }
        /*
        for (i32 i = 0; i < deck1Size; i++)
        {
            printf("%i, ", cardPool.cards[decks[0].cards[i]].value);
        }
        printf("\n");
        for (i32 i = 0; i < deck2Size; i++)
        {
            printf("%i, ", cardPool.cards[decks[1].cards[i]].value);
        }
        printf("\n\n");
        */
        maxTurns--;
    }


    printf("Ties: %i\n", tie);
    printf("1 Wins: %i\n", win1);
    printf("2 Wins: %i\n", win2);

    if (maxTurns <= 0)
    {
        printf("Ran out of turns\n");
        for (i32 i = 0; i < deck1Size; i++)
        {
            printf("%i, ", cardPool.cards[decks[0].cards[i]].value);
        }
        printf("\n");
        for (i32 i = 0; i < deck2Size; i++)
        {
            printf("%i, ", cardPool.cards[decks[1].cards[i]].value);
        }
        printf("\n\n");
    }
    else if (deck1Size < deck2Size) {
        printf("Deck 2 Wins!");
    }
    else if (deck2Size < deck1Size) {
        printf("Deck 1 Wins!");
    }


    CleanUpDecks(decks, 2);
}

i32 main(i32 argc, char *argv[])
{
    char suits[] = {'H', 'D', 'C', 'S'};
    char ranks[] = {'2','3','4','5','6','7','8','9','0','J','Q','K','A'};
    i32 values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    i32 numOfValues = 13;
    i32 numOfSuits = 4;

    struct CardPool pool = BuildCardPool(numOfSuits * numOfValues,numOfSuits,numOfValues,suits,ranks,values);

    RunWar(pool);

    //printf("CardPool size: %d\n", pool.size);
    //printf("CardPool Card 1: %c\n", suits[3]);
    //for (i32 i = 0; i < pool.size; ++i)
    //{
    //    printf("%c", pool.cards[i].suit);
    //    printf("%c\n", pool.cards[i].rank);
    //}
    free(pool.cards);
}