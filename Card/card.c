#include "card.h"
#include <stdlib.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
	uint8_t cardName[100] = "";
	uint8_t cardNameLength = 0;

	printf("Enter your card holder name: ");
	gets(cardName);
	cardName[49] = (uint8_t) NULL;

	cardNameLength = (uint8_t)strlen(cardName);

	if ((cardNameLength < 20) || (cardNameLength > 24))
	{
		return WRONG_NAME;
	}
	else
	{
		strcpy(cardData->cardHolderName, cardName);
		return CARD_OK;
	}
}

EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
	uint8_t cardDate[100] = "";
	uint8_t cardDateLength = 0;

	printf("Enter your card expiry date: ");
	gets(cardDate);
	cardDate[9] = (uint8_t) NULL;

	cardDateLength = (uint8_t)strlen(cardDate);

	if (cardDateLength != 5)
	{
		return WRONG_EXP_DATE;
	}
	else if (cardDate[2] != '/')
	{
		return WRONG_EXP_DATE;
	}
	else if (validateCardDate(&cardDate))
	{
		return WRONG_EXP_DATE;
	}
	else
	{
		strcpy(cardData->cardExpirationDate, cardDate);
		return CARD_OK;
	}
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
	uint8_t cardPan[100] = "";
	uint8_t cardPanLength = 0;

	printf("Enter your card PAN: ");
	gets(cardPan);
	cardPan[29] = (uint8_t)NULL;

	cardPanLength = (uint8_t)strlen(cardPan);

	if ((cardPanLength < 16) || (cardPanLength >19))
	{
		return WRONG_PAN;
	}
	else if (validateCardPan(&cardPan))
	{
		return WRONG_PAN;
	}
	else
	{
		strcpy(cardData->primaryAccountNumber, cardPan);
		return CARD_OK;
	}
}


void getCardHolderNameTest(ST_cardData_t* cardData)
{
	EN_cardError_t cardError;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: getCardHolderName\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: \"\"\n");
	cardError = getCardHolderName(cardData);
	printf("Expected Result: %d\n", WRONG_NAME);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 2:\n");
	printf("Input Data: Ahmed Helmi\n");
	cardError = getCardHolderName(cardData);
	printf("Expected Result: %d\n", WRONG_NAME);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 3:\n");
	printf("Input Data: Ahmed Helmi Abdel Wahab Hassan\n");
	cardError = getCardHolderName(cardData);
	printf("Expected Result: %d\n", WRONG_NAME);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 4:\n");
	printf("Input Data: Ahmed Helmi Abdel Wahab\n");
	cardError = getCardHolderName(cardData);
	printf("Expected Result: %d\n", CARD_OK);
	printf("Actual Result: %d\n\n", cardError);
}

void getCardExpiryDateTest(ST_cardData_t* cardData)
{
	EN_cardError_t cardError;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: getCardExpiryDate\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: \"\"\n");
	cardError = getCardExpiryDate(cardData);
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 2:\n");
	printf("Input Data: 2/23\n");
	cardError = getCardExpiryDate(cardData);
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 3:\n");
	printf("Input Data: 02/2023\n");
	cardError = getCardExpiryDate(cardData);
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 4:\n");
	printf("Input Data: 02234\n");
	cardError = getCardExpiryDate(cardData);
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 5:\n");
	printf("Input Data: 15/23\n");
	cardError = getCardExpiryDate(cardData);
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 6:\n");
	printf("Input Data: 12/10\n");
	cardError = getCardExpiryDate(cardData);
	printf("Expected Result: %d\n", WRONG_EXP_DATE);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 7:\n");
	printf("Input Data: 07/23\n");
	cardError = getCardExpiryDate(cardData);
	printf("Expected Result: %d\n", CARD_OK);
	printf("Actual Result: %d\n\n", cardError);
}

void getCardPANTest(ST_cardData_t* cardData)
{
	EN_cardError_t cardError;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: getCardPAN\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: \"\"\n");
	cardError = getCardPAN(cardData);
	printf("Expected Result: %d\n", WRONG_PAN);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 2:\n");
	printf("Input Data: 0123456789\n");
	cardError = getCardPAN(cardData);
	printf("Expected Result: %d\n", WRONG_PAN);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 3:\n");
	printf("Input Data: 01234567890123456789\n");
	cardError = getCardPAN(cardData);
	printf("Expected Result: %d\n", WRONG_PAN);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 4:\n");
	printf("Input Data: 0123456789aaaaaaa\n");
	cardError = getCardPAN(cardData);
	printf("Expected Result: %d\n", WRONG_PAN);
	printf("Actual Result: %d\n\n", cardError);

	printf("Test Case 5:\n");
	printf("Input Data: 01234567890123456\n");
	cardError = getCardPAN(cardData);
	printf("Expected Result: %d\n", CARD_OK);
	printf("Actual Result: %d\n\n", cardError);
}


char validateCardDate(uint8_t * inputCardDate)
{
	uint8_t tempCardDate[10];
	strcpy(tempCardDate, inputCardDate);
	uint8_t* cardMonth = strtok(tempCardDate, "/");
	uint8_t* cardYear = strtok(NULL, "/");

	if ((atoi(cardMonth) > 12) || (atoi(cardMonth) < 1))
	{
		return 1; // Wrong Month
	}

	if ((atoi(cardYear) < 22) || (atoi(cardYear) > 99))
	{
		return 1; // Wrong Month
	}

	return 0;
}

char validateCardPan(uint8_t* inputCardPan)
{
	uint8_t tempCardPan[25];
	strcpy(tempCardPan, inputCardPan);
	int cardPanLenth = strlen(inputCardPan);
	int i;

	for(i = 0; i < cardPanLenth; i++)
	{
		if (!isdigit(tempCardPan[i])) break;
	}

	if (i < cardPanLenth) return 1;
	else return 0;
}