#include "terminal.h"

EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
	uint8_t transactionDate[100] = "";
	uint8_t transactionDateLength = 0;

	printf("Enter your transaction date: ");
	gets(transactionDate);
	transactionDate[15] = (uint8_t)NULL;

	transactionDateLength = (uint8_t)strlen(transactionDate);

	if (transactionDateLength != 10)
	{
		return WRONG_DATE;
	}
	else if (transactionDate[2] != '/')
	{
		return WRONG_DATE;
	}
	else if (transactionDate[5] != '/')
	{
		return WRONG_DATE;
	}
	else if (validateTransactionDate(&transactionDate))
	{
		return WRONG_DATE;
	}
	else
	{
		strcpy(termData->transactionDate, transactionDate);
		return TERMINAL_OK;
	}
}

EN_terminalError_t isCardExpired(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	uint8_t tempTransactionDate[15];
	strcpy(tempTransactionDate, termData->transactionDate);
	uint8_t* transactionDayPtr = strtok(tempTransactionDate, "/");
	uint8_t* transactionMonthPtr = strtok(NULL, "/");
	uint8_t* transactionYearPtr = strtok(NULL, "/");

	int transactionDay = atoi(transactionDayPtr);
	int transactionMonth = atoi(transactionMonthPtr);
	int transactionYear = atoi(transactionYearPtr);

	uint8_t tempCardDate[10];
	strcpy(tempCardDate, cardData->cardExpirationDate);
	uint8_t* cardExpirMonthPtr = strtok(tempCardDate, "/");
	uint8_t* cardExpirYearPtr = strtok(NULL, "/");

	int cardExpirMonth = atoi(cardExpirMonthPtr);
	int cardExpirYear = atoi(cardExpirYearPtr);
	cardExpirYear += 2000;

	if (cardExpirYear < transactionYear)
	{
		return EXPIRED_CARD;
	}
	else if ((cardExpirYear == transactionYear) && (cardExpirMonth < transactionMonth))
	{
		return EXPIRED_CARD;
	}
	else
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
	float tempTransactionAmount = 0.0;
	printf("Enter your transaction amount: ");
	int readResult = scanf("%f", &tempTransactionAmount);
	
	if (readResult != 1)
	{
		return INVALID_AMOUNT;
	} 
	else if (tempTransactionAmount <= 0)
	{
		return INVALID_AMOUNT;
	}
	else
	{
		termData->transAmount = tempTransactionAmount;
		return TERMINAL_OK;
	}
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
	if (termData->transAmount > termData->maxTransAmount)
	{
		return EXCEED_MAX_AMOUNT;
	}
	else
	{
		return TERMINAL_OK;
	}
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData, float maxAmount)
{
	if (maxAmount <= 0)
	{
		return INVALID_MAX_AMOUNT;
	}
	else
	{
		termData->maxTransAmount = maxAmount;
		return TERMINAL_OK;
	}
}

EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{

}


void getTransactionDateTest(ST_terminalData_t* termData)
{
	EN_terminalError_t terminalError;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: getTransactionDate\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: \"\"\n");
	terminalError = getTransactionDate(termData);
	printf("Expected Result: %d\n", WRONG_DATE);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 2:\n");
	printf("Input Data: 2/7/2023\n");
	terminalError = getTransactionDate(termData);
	printf("Expected Result: %d\n", WRONG_DATE);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 3:\n");
	printf("Input Data: 02-07-2023\n");
	terminalError = getTransactionDate(termData);
	printf("Expected Result: %d\n", WRONG_DATE);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 4:\n");
	printf("Input Data: 29/10/2022\n");
	terminalError = getTransactionDate(termData);
	printf("Expected Result: %d\n", TERMINAL_OK);
	printf("Actual Result: %d\n\n", terminalError);
}

void isCardExpiredTest(ST_cardData_t* cardData, ST_terminalData_t* termData)
{
	EN_terminalError_t terminalError;
	strcpy(termData->transactionDate, "11/08/2022");

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: isCardExpired\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: Card Expire Date: 10/21 - Transaction Date: 11/08/2022\n");
	strcpy(cardData->cardExpirationDate, "10/21");
	terminalError = isCardExpired(cardData, termData);
	printf("Expected Result: %d\n", EXPIRED_CARD);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 2:\n");
	printf("Input Data: Card Expire Date: 05/22 - Transaction Date: 11/08/2022\n");
	strcpy(cardData->cardExpirationDate, "05/22");
	terminalError = isCardExpired(cardData, termData);
	printf("Expected Result: %d\n", EXPIRED_CARD);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 3:\n");
	printf("Input Data: Card Expire Date: 12/22 - Transaction Date: 11/08/2022\n");
	strcpy(cardData->cardExpirationDate, "12/22");
	terminalError = isCardExpired(cardData, termData);
	printf("Expected Result: %d\n", TERMINAL_OK);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 4:\n");
	printf("Input Data: Card Expire Date: 05/23 - Transaction Date: 11/08/2022\n");
	strcpy(cardData->cardExpirationDate, "05/23");
	terminalError = isCardExpired(cardData, termData);
	printf("Expected Result: %d\n", TERMINAL_OK);
	printf("Actual Result: %d\n\n", terminalError);
}

void getTransactionAmountTest(ST_terminalData_t* termData)
{
	EN_terminalError_t terminalError;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: getTransactionAmount\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: Transaction amount = 0.0\n");
	terminalError = getTransactionAmount(termData);
	printf("Expected Result: %d\n", INVALID_AMOUNT);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 2:\n");
	printf("Input Data: Transaction amount = -100\n");
	terminalError = getTransactionAmount(termData);
	printf("Expected Result: %d\n", INVALID_AMOUNT);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 3:\n");
	printf("Input Data: Transaction amount = 1000\n");
	terminalError = getTransactionAmount(termData);
	printf("Expected Result: %d\n", TERMINAL_OK);
	printf("Actual Result: %d\n\n", terminalError);
}

void isBelowMaxAmountTest(ST_terminalData_t* termData)
{
	termData->maxTransAmount = 5000;
	EN_terminalError_t terminalError;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: isBelowMaxAmount\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: Transaction amount = 7000 - Max Transaction = 5000\n");
	termData->transAmount = 7000;
	terminalError = isBelowMaxAmount(termData);
	printf("Expected Result: %d\n", EXCEED_MAX_AMOUNT);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 2:\n");
	printf("Input Data: Transaction amount = 2500 - Max Transaction = 5000\n");
	termData->transAmount = 2500;
	terminalError = isBelowMaxAmount(termData);
	printf("Expected Result: %d\n", TERMINAL_OK);
	printf("Actual Result: %d\n\n", terminalError);
}

void setMaxAmountTest(ST_terminalData_t* termData)
{
	float maxAmount;
	EN_terminalError_t terminalError;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: setMaxAmount\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: Maximum transaction amount = 0.0\n");
	maxAmount = 0.0;
	terminalError = setMaxAmount(termData, maxAmount);
	printf("Expected Result: %d\n", INVALID_MAX_AMOUNT);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 2:\n");
	printf("Input Data: Maximum transaction amount = -100\n");
	maxAmount = -100;
	terminalError = setMaxAmount(termData, maxAmount);
	printf("Expected Result: %d\n", INVALID_MAX_AMOUNT);
	printf("Actual Result: %d\n\n", terminalError);

	printf("Test Case 3:\n");
	printf("Input Data: Maximum transaction amount = 5000\n");
	maxAmount = 5000;
	terminalError = setMaxAmount(termData, maxAmount);
	printf("Expected Result: %d\n", TERMINAL_OK);
	printf("Actual Result: %d\n\n", terminalError);
}

void isValidCardPANTest(ST_cardData_t* cardData)
{

}


char validateTransactionDate(uint8_t* inputTransactionDate)
{
	uint8_t tempTransactionDate[15];
	strcpy(tempTransactionDate, inputTransactionDate);
	uint8_t* transactionDay = strtok(tempTransactionDate, "/");
	uint8_t* transactionMonth = strtok(NULL, "/");
	uint8_t* transactionYear = strtok(NULL, "/");

	if ((atoi(transactionDay) > 31) || (atoi(transactionDay) < 1))
	{
		return 1; // Wrong Day
	}

	if ((atoi(transactionMonth) > 12) || (atoi(transactionMonth) < 1))
	{
		return 1; // Wrong Month
	}

	if (atoi(transactionYear) < 2022)
	{
		return 1; // Wrong Year
	}

	return 0;
}