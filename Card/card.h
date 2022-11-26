#ifndef PayCard
#define PayCard

#include <string.h>
#include <stdio.h>

#define uint8_t unsigned char

typedef struct ST_cardData_t
{
    uint8_t cardHolderName[25];
    uint8_t primaryAccountNumber[20];
    uint8_t cardExpirationDate[6];
}ST_cardData_t;

typedef enum EN_cardError_t
{
    CARD_OK, WRONG_NAME, WRONG_EXP_DATE, WRONG_PAN
}EN_cardError_t;

EN_cardError_t getCardHolderName(ST_cardData_t* cardData);
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData);
EN_cardError_t getCardPAN(ST_cardData_t* cardData);

char validateCardDate(uint8_t* inputCardDate);
char validateCardPan(uint8_t* inputCardDate);

void getCardHolderNameTest(ST_cardData_t* cardData);
void getCardExpiryDateTest(ST_cardData_t* cardData);
void getCardPANTest(ST_cardData_t* cardData);

#endif /* PayCard */