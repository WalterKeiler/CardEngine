#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
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

i32 main(i32 argc, char *argv[])
{
    char suits[] = {'H', 'D', 'C', 'S'};
    char ranks[] = {'2','3','4','5','6','7','8','9','0','J','Q','K','A'};
    i32 values[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14};
    struct CardPool pool = BuildCardPool(52,4,13,suits,ranks,values);

    printf("CardPool size: %d\n", pool.size);
    printf("CardPool Card 1: %c\n", suits[3]);
    for (i32 i = 0; i < pool.size; ++i)
    {
        printf("%c", pool.cards[i].suit);
        printf("%c\n", pool.cards[i].rank);
    }
    free(pool.cards);
}