/*
    weekday.c    : Assignment #1
    Author       : Vinh Nguyen
    Modifications:
        - Jan 05 2026 - Initial version
*/

#include <stdio.h>

/* ZELLER_DOW enumeration */
enum ZELLER_DOW {
    ZELLER_DOW_SAT = 0,
    ZELLER_DOW_SUN,
    ZELLER_DOW_MON,
    ZELLER_DOW_TUE,
    ZELLER_DOW_WED,
    ZELLER_DOW_THU,
    ZELLER_DOW_FRI,
};

/* ZellerCongruence function */

int ZellerCongruence(int day, int month, int year)
{
    if (month < 3) {
        month += 12;
        year--;
    }
    
    int G = -1;         
    int V = year / 100; 
    int U = year % 100; 

    int chunk = ( (month + 1) * 13) / 5;
    G = (day + chunk + U + (U / 4) + (V / 4) - (2 * V) ) % 7;

    return G;
}

/* WhatZellerDOW function*/
void WhatZellerDOW(const int day)
{
    switch (day)
    {
        case ZELLER_DOW_SAT:
            printf("This is a Saturday.\n");
            break;
        case ZELLER_DOW_SUN:
            printf("This is a Sunday.\n");
            break;
        case ZELLER_DOW_MON:
            printf("This is a Monday.\n");
            break;
        case ZELLER_DOW_TUE:
            printf("This is a Tuesday.\n");
            break;
        case ZELLER_DOW_WED:
            printf("This is a Wednesday.\n");
            break;
        case ZELLER_DOW_THU:
            printf("This is a Thursday.\n");
            break;
        case ZELLER_DOW_FRI:
            printf("This is a Friday.\n");
            break;
    }
}

/* main function */

int main(void)
{
    int q = 0;
    int m = 0;
    int y = 0;

    printf("Please enter a calendar date:\n");
    printf("Day, q = ");
    scanf("%d", &q);
    printf("Month, m = ");
    scanf("%d", &m);
    printf("Year, y = ");
    scanf("%d", &y);

    int zellerResult = ZellerCongruence(q, m, y);
    printf("For the calendar date %d/%d/%d, the day of the week is %d.\n", m, q ,y, zellerResult);
    WhatZellerDOW(zellerResult);

    return 0;
}

/* EOF */
