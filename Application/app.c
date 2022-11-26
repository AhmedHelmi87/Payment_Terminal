#include "app.h"

extern struct ST_accountsDB_t accountsDB[255];
struct ST_cardData_t userCardData;
struct ST_terminalData_t currentTerminalData;
struct ST_transaction_t currentTransData;

void main(void)
{
	initAccountDB();
	initTransactionDB();

	//getCardHolderNameTest(&userCardData);
	//getCardExpiryDateTest(&userCardData);
	//getCardPANTest(&userCardData);
	//getTransactionDateTest(&currentTerminalData);
	//isCardExpiredTest(&userCardData, &currentTerminalData);
	//getTransactionAmountTest(&currentTerminalData);
	//isBelowMaxAmountTest(&currentTerminalData);
	//setMaxAmountTest(&currentTerminalData);
	//isValidAccountTest();
	//isBlockedAccountTest();
	//isAmountAvailableTest();
	//saveTransactionTest();
	//listSavedTransactionsTest();
	//recieveTransactionDataTest();

	char userSelection = '0';

	while (1)
	{
		system("cls");
		printf("\033[32mWelcom to \"Payment application\":\033[0m\n\n");
		printf("Please select one of the following options:\n");
		printf(" 1. New transaction cycle\n");
		printf(" 2. List all saved transactions\n");
		printf(" 3. Exist\n\n");
		printf("\033[33mEnter your selection: ");
		userSelection = _getche();
		printf("\033[0m\n\n\n");

		if (userSelection == '1')
		{
			system("cls");
			appStart();
		}
		else if (userSelection == '2')
		{
			system("cls");
			listSavedTransactions();
		}
		else if (userSelection == '3')
		{
			break;
		}
		else
		{
			printf("\033[31m**** Wrong Selection ****\033[0m\n");
		}
		
		printf("\n\033[33mPress Any Key to Continue\033[0m\n");
		_getch();
	}
}

EN_cardError_t getCardHolderNameRetry(ST_cardData_t *cardData)
{
	int inputRetries = 0;
	EN_cardError_t cardError;

	do {
		cardError = getCardHolderName(cardData);
		if (cardError == WRONG_NAME)
		{
			printf("\033[33mWrong Card Holder Name\033[0m\n");
			inputRetries++;
			if (inputRetries == 3)
			{
				printf("\033[31mToo Many Attempt Retries\033[0m\n");
				break;
			}
		}
	} while (cardError == WRONG_NAME);

	return cardError;
}

EN_cardError_t getCardExpiryDateRetry(ST_cardData_t* cardData)
{
	int inputRetries = 0;
	EN_cardError_t cardError;

	do {
		cardError = getCardExpiryDate(cardData);
		if (cardError == WRONG_EXP_DATE)
		{
			printf("\033[33mWrong Card Expiry Date\033[0m\n");
			inputRetries++;
			if (inputRetries == 3)
			{
				printf("\033[31mToo Many Attempt Retries\033[0m\n");
				break;
			}
		}
	} while (cardError == WRONG_EXP_DATE);

	return cardError;
}

EN_cardError_t getCardPANRetry(ST_cardData_t* cardData)
{
	int inputRetries = 0;
	EN_cardError_t cardError;

	do {
		cardError = getCardPAN(cardData);
		if (cardError == WRONG_PAN)
		{
			printf("\033[33mWrong Card PAN\033[0m\n");
			inputRetries++;
			if (inputRetries == 3)
			{
				printf("\033[31mToo Many Attempt Retries\033[0m\n");
				break;
			}
		}
	} while (cardError == WRONG_PAN);

	return cardError;
}

EN_terminalError_t getTransactionDateRetry(ST_cardData_t * tempCard, ST_terminalData_t* termData)
{
	int inputRetries = 0;
	EN_terminalError_t terminalError;

	do {
		terminalError = getTransactionDate(termData);
		if (terminalError == WRONG_DATE)
		{
			printf("\033[33mWrong Transaction Date Format\033[0m\n");
			inputRetries++;
		}
		else
		{
			terminalError = isCardExpired(tempCard, termData);
			if (terminalError == EXPIRED_CARD)
			{
				printf("\033[31mTransaction Incomplete - Card Expired\033[0m\n");
				break;
			}
		}

		if (inputRetries == 3)
		{
			printf("\033[31mToo Many Attempt Retries\033[0m\n");
			break;
		}
	} while (terminalError != TERMINAL_OK);

	return terminalError;
}

EN_terminalError_t getTransactionAmountRetry(ST_terminalData_t* termData)
{
	int inputRetries = 0;
	EN_terminalError_t terminalError;

	do {
		terminalError = getTransactionAmount(termData);
		if (terminalError == INVALID_AMOUNT)
		{
			printf("\033[33mInvalid Amount or Format\033[0m\n");
			inputRetries++;
		}
		else
		{
			terminalError = isBelowMaxAmount(termData);
			if (terminalError == EXCEED_MAX_AMOUNT)
			{
				printf("\033[31mTransaction Incomplete - Terminal Limit Exceeded \033[0m\n");
				break;
			}
		}

		if (inputRetries == 3)
		{
			printf("\033[31mToo Many Attempt Retries\033[0m\n");
			break;
		}
	} while (terminalError != TERMINAL_OK);

	return terminalError;
}

void appStart(void)
{
	EN_transState_t transactionError;

	printf("\033[32m********** New Transaction **********\033[0m\n\n");

	// Set Terminal Maximum Transaction Amount
	if (setMaxAmount(&currentTerminalData, 10000) == INVALID_MAX_AMOUNT)
	{
		printf("\033[31mError setting the Terminal Max Amount to 10000\033[0m\n");
		return;
	}
	
	// Read & Check Card Data
	if (getCardHolderNameRetry(&userCardData) != CARD_OK) return;
	if (getCardExpiryDateRetry(&userCardData) != CARD_OK) return;
	if (getCardPANRetry(&userCardData) != CARD_OK) return;
	
	// Read & Check Transaction Data
	if (getTransactionDateRetry(&userCardData, &currentTerminalData) != TERMINAL_OK)
	{
		printf("\n\033[32m********** Transaction Ended **********\033[0m\n\n");
		return;
	}

	if (getTransactionAmountRetry(&currentTerminalData) != TERMINAL_OK)
	{
		printf("\n\033[32m********** Transaction Ended **********\033[0m\n\n");
		return;
	}
	
	// Validate Transaction Data with the Server
	currentTransData.cardHolderData = userCardData;
	currentTransData.terminalData = currentTerminalData;
	
	transactionError = recieveTransactionData(&currentTransData);

	printTransState(transactionError);

	printf("\n\033[32m********** Transaction Ended **********\033[0m\n\n");

	return;
}