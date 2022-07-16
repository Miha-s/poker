#include <stdio.h>
#include <stdlib.h>


/*enum suits {club, diamond, heart, spade};*/

struct card {
    enum {club, diamond, heart, spade} suit;
    int rank;
};
/* club     ♣ 
   diamond  ♦ 
   heart    ♥
   spade    ♠
*/

/* 1  - if first player win
   0  - if second player win
   -1 - if draw         */
int cmp_cards(struct card *fplayer, struct card *splayer, int size);

/* we just make array of cars here */
struct card *randcard(struct card *pl, const unsigned int max, 
                                const unsigned int min, int amount) {
    for(int i = 0; i < amount; i++) {
        pl[i].rank = min + rand()%(max - min + 1);
        pl[i].suit = rand()%4;
    }
    return pl;
}

/* print cards and their suits */
void print_cards(const struct card *cards, int amount) {
    printf("***************\n");

    for(int i = 0; i < amount; i++) {
        char c = 0;
        switch(cards[i].rank) {
            case 11: c = 'J'; break;
            case 12: c = 'Q'; break;
            case 13: c = 'K'; break;
            case 14: c = 'A'; break;
            default: printf("%d", cards[i].rank);
        }
        if(c) putchar(c);
        switch(cards[i].suit) {
            case club:    c = 'c'; break;
            case diamond: c = 'd'; break;
            case heart:   c = 'h'; break;
            case spade:   c = 's'; break;
        }
        putchar(c);
        putchar(' ');
    }
    printf("\n***************\n");
}


/**********************************************************/
int main() {
    int seed;
    const unsigned int max_size = 14;
    const unsigned int min_size = 2;
    int amcards = 5; /* amount of cards for each player */
    struct card player[amcards], computer[amcards];
    int chances;
    const char *prize = "\nYou are doing really well :)\nTake a piece of cake here <...>\n";

    printf("Please enter any seed: ");
    if(scanf("%i", &seed) != 1) {
        printf("Please enter correct number!\n");
        exit(0);
    }
    srand(seed);

    char game = 'y';
    while(game == 'y'){  /* while player want to play the game */


    /* make cards for computer and player */
    randcard(player, max_size, min_size, amcards);
    randcard(computer, max_size, min_size, amcards);
    
    printf("Your cards are:\n");
    print_cards(player, amcards);

    chances = 3;

    /* giving player 3 chances to change cards */
    while(chances) {
        printf("Do you wanna reroll your cards? (%d probs left) [y/n] ", chances);
        getchar();
        if(getchar() == 'y') {
            randcard(player, max_size, min_size, amcards);
            printf("Your cards are:\n");
            print_cards(player, amcards);
            chances--;
        }
        else
            break;
    }

    printf("Computer cards are:\n");
    print_cards(computer, amcards);

    putchar('\n');

    switch(cmp_cards(player, computer, amcards)) {
        case 1:
            printf("My congratulations! You have won, here is your prize\n");
            printf("%s", prize);
            break;
        case 0:
            printf("Ooups, you have lose! But don't worry, computer just cheated\n");
            break;
        case -1:
            printf("Wow, here is a draw, that's fastinating!\n");
            break;
        default:
            printf("Something's gone wrong...\n");
    }

    printf("Do you wanna play game again?[y/n] ");
    getchar();
    game = getchar();
    putchar('\n');
    }

    return 0;
}

int Pairs(struct card *pl, int size) {
    int flag = 0;
    for(int i = 0; i < size-1; i++)
        if(pl[i].rank == pl[i+1].rank)
            flag++;

    if(flag) return 3-flag;

    return 3;
}

int ThreeOfAKind(struct card *pl, int size) {
    int flag;

    flag = (pl[2].rank == pl[0].rank || pl[2].rank == pl[4].rank);
    
    if(flag) return flag;

    return 1 + Pairs(pl, size);
}

int Straight(struct card *pl, int size) {
    int flag = 1;
    
    for(int i = 0; i < size - 1; i++)
        if(pl[i].rank != pl[i+1].rank - 1) {
            flag = 0;
            break;
        }

    if(flag) return flag;

    return 1 + ThreeOfAKind(pl, size);
}

int Flush(struct card *pl, int size) {
    int flag = 1;
    for(int i = 0; i < size - 1; i++)
        if(pl[i].suit != pl[i+1].suit) {
            flag = 0;
            break;
        }

    if(flag) return flag;

    return 1 + Straight(pl, size);
}

int FullHouse(struct card *pl, int size) {
    int flag = 0;
    if(pl[2].rank == pl[0].rank)
        flag = pl[3].rank == pl[4].rank;
    else if(pl[2].rank == pl[4].rank)
        flag = pl[0].rank == pl[1].rank;

    if(flag) return flag;

    return 1 + Flush(pl, size);
}


int FourOfAKind(struct card *pl, int size) {
    int flag = 0;
    if(pl[1].rank == pl[3].rank && (pl[1].rank == pl[0].rank || pl[1].rank == pl[4].rank))
        flag = 1;

    if(flag) return flag;

    return 1 + FullHouse(pl, size);    
}

int StraightFlush(struct card *pl, int size) {
    int flag = 1;
    for(int i = 0; i < size-1; i++)
        if(pl[i].suit != pl[i+1].suit || pl[i].rank != pl[i+1].rank - 1) {
            flag = 0;
            break;
        }

    if(flag) return flag;
    return 1 + FourOfAKind(pl, size);
}

int RoyalFlush(struct card *pl, int size) {
    struct card tmp;
    /* sorting cards */
    int royal_flush[] = {10, 11, 12, 13, 14};
    for(int i = 0; i < size; i++)
        for(int a = 0; a < size - 1; a++)
            if(pl[a].rank > pl[a+1].rank) {
                tmp = pl[a];
                pl[a] = pl[a+1];
                pl[a+1] = tmp;
            }
    int flag = 1;
    for(int i = 0; i < size-1; i++)
        if(pl[i].suit != pl[i+1].suit || pl[i].rank != royal_flush[i]) {
            flag = 0;
            break;
        }

    if(flag) flag = pl[4].rank == royal_flush[4];
    if(flag) return flag;

    return 1 + StraightFlush(pl, size);
}

int cmp_cards(struct card *fplayer, struct card *splayer, int size) {
    int fp, sp;
    // we recursively find what is the strongest combination each 
    // plaer have
    fp = RoyalFlush(fplayer, size);
    sp = RoyalFlush(splayer, size);


    if(fp != sp) return fp < sp;

    int i, a;
    // and then if two players have equal strongest combination
    // we need to check their other cards to choose winner
    // *and we alredy have sorted cards here*
    switch(fp) {
    case 1: return -1; 
    case 2: if(fplayer[0].rank != splayer[0].rank)
                return fplayer[0].rank > splayer[0].rank;
            return -1; 
    case 3: if(fplayer[2].rank != splayer[2].rank)
                return fplayer[2].rank > splayer[2].rank; 
            if(fplayer[0].rank == fplayer[1].rank)
                i = 4;
            else 
                i = 0;
            if(splayer[0].rank == splayer[1].rank)
                a = 4;              
            else
                a = 0;
            if(fplayer[i].rank != splayer[a].rank)
                return fplayer[i].rank > splayer[a].rank;
            return -1;
    case 4: if(fplayer[2].rank != splayer[2].rank) 
                return fplayer[2].rank > splayer[2].rank;
            if(fplayer[2].rank == fplayer[4].rank)
                i = 0;
            else 
                i = 4;
            if(splayer[2].rank == splayer[4].rank)
                a = 0;              
            else
                a = 4;
            if(fplayer[i].rank != splayer[a].rank)
                return fplayer[i].rank > splayer[a].rank;
            return -1;
    case 5: for(i = 4; i >= 0; i--)
                if(fplayer[i].rank != splayer[i].rank)
                    return fplayer[i].rank > splayer[i].rank;
            return -1;
    case 6: if(fplayer[2].rank != splayer[2].rank)
                return fplayer[2].rank > splayer[2].rank; 
    case 7: if(fplayer[2].rank != splayer[2].rank)
                return fplayer[2].rank > splayer[2].rank; 
            for(i = 4; i >= 0; i--)
                if(fplayer[i].rank != fplayer[2].rank)
                    break;
            for(a = 4; a >= 0; a--)
                if(splayer[a].rank != splayer[2].rank)
                    break;
            if(fplayer[i].rank != splayer[a].rank)
                return fplayer[i].rank > splayer[a].rank;
            
            for(; i >= 0; i--)
                if(fplayer[i].rank != fplayer[2].rank)
                    break;
            for(; a >= 0; a--)
                if(splayer[a].rank != splayer[2].rank)
                    break;

            if(fplayer[i].rank != splayer[a].rank)
                return fplayer[i].rank > splayer[a].rank;
            return -1;
    case 8: if(fplayer[3].rank != splayer[3].rank)
                return fplayer[3].rank > splayer[3].rank;
            if(fplayer[1].rank != splayer[1].rank)
                return fplayer[1].rank > splayer[1].rank;
            if(fplayer[1].rank != fplayer[0].rank)
                i = 0;
            else if (fplayer[3].rank != fplayer[4].rank)
                i = 4;
            else 
                i = 2;

            if(splayer[1].rank != splayer[0].rank)
                a = 0;
            else if (splayer[3].rank != splayer[4].rank)
                a = 4;
            else 
                a = 2;

            if(fplayer[i].rank != splayer[a].rank)
                return fplayer[i].rank > splayer[a].rank;
            return -1;
    case 9: for(i = 0; i < size-1; i++)
                if(fplayer[i].rank == fplayer[i+1].rank)
                    break;
                    
            for(a = 0; a < size-1; a++)
                if(splayer[a].rank == splayer[a+1].rank)
                    break;

            if(fplayer[i].rank != splayer[a].rank)
                return fplayer[i].rank > splayer[a].rank;

            struct card f[3];
            struct card s[3];
            
            int m = 0;
            for(int j = 4; j >= 0; j--)
                if(j != i && j != i+1)
                    f[m++] = fplayer[j];
            m = 0;
            for(int j = 0; j < size; j++)
                if(j != a && j != a+1)
                    f[m++] = splayer[j];

            for(i = 2; i >= 0; i--)
                if(f[i].rank != s[i].rank)
                    return f[i].rank > s[i].rank;
            return -1;
    case 10: for(i = 4; i >= 0; i--)
                if(fplayer[i].rank != splayer[i].rank)
                    return fplayer[i].rank > splayer[i].rank;
             return -1;
    default: printf("Something's gone wrong in switch...\n");
             return 2;
    }       
}

