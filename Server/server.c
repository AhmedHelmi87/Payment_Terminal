#include "server.h"

void initAccountDB(void)
{
	for (int i = 0; i < 255; i++)
	{
		accountsDB[i].balance = 0.0;
		accountsDB[i].state = BLOCKED;
		strcpy(accountsDB[i].primaryAccountNumber, "8989374615436851");	
	}

	accountsDB[0].balance = 1000.0;
	accountsDB[0].state = RUNNING;
	strcpy(accountsDB[0].primaryAccountNumber, "12345165165679002");

	accountsDB[1].balance = 5000.0;
	accountsDB[1].state = RUNNING;
	strcpy(accountsDB[1].primaryAccountNumber, "54648456464654002");

	accountsDB[2].balance = 2000.0;
	accountsDB[2].state = RUNNING;
	strcpy(accountsDB[2].primaryAccountNumber, "9875431654901057");

	accountsDB[3].balance = 40000.0;
	accountsDB[3].state = RUNNING;
	strcpy(accountsDB[3].primaryAccountNumber, "9721316464321032");

	accountsDB[4].balance = 30000.0;
	accountsDB[4].state = RUNNING;
	strcpy(accountsDB[4].primaryAccountNumber, "6541315646046008");

	accountsDB[5].balance = 100000.0;
	accountsDB[5].state = BLOCKED;
	strcpy(accountsDB[5].primaryAccountNumber, "69873165624460077");

	accountsDB[6].balance = 6000.0;
	accountsDB[6].state = BLOCKED;
	strcpy(accountsDB[6].primaryAccountNumber, "32543164346007041");

	accountsDB[7].balance = 8000.0;
	accountsDB[7].state = BLOCKED;
	strcpy(accountsDB[7].primaryAccountNumber, "3254654564460294");

	accountsDB[8].balance = 30000.0;
	accountsDB[8].state = BLOCKED;
	strcpy(accountsDB[8].primaryAccountNumber, "6541235764460007");

	accountsDB[9].balance = 8000.0;
	accountsDB[9].state = BLOCKED;
	strcpy(accountsDB[9].primaryAccountNumber, "6978735776446003");
}

void initTransactionDB(void)
{
	memset(&transActionDB, (255 * sizeof(transActionDB)), 0);
}


EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
	EN_serverError_t serverError;
	struct ST_accountsDB_t tempAccount;

	serverError = isValidAccount(&transData->cardHolderData, &tempAccount);
	if (serverError == ACCOUNT_NOT_FOUND)
	{
		transData->transState = FRAUD_CARD;
		saveTransaction(transData);
		return FRAUD_CARD;
	}

	serverError = isAmountAvailable(&transData->terminalData, &tempAccount);
	if (serverError == LOW_BALANCE)
	{
		transData->transState = DECLINED_INSUFFECIENT_FUND;
		saveTransaction(transData);
		return DECLINED_INSUFFECIENT_FUND;
	}

	serverError = isBlockedAccount(&tempAccount);
	if (serverError == BLOCKED_ACCOUNT)
	{
		transData->transState = DECLINED_STOLEN_CARD;
		saveTransaction(transData);
		return DECLINED_STOLEN_CARD;
	}

	serverError = updateAccountBalance(transData);
	if (serverError == SAVING_FAILED)
	{
		transData->transState = INTERNAL_SERVER_ERROR;
		saveTransaction(transData);
		return INTERNAL_SERVER_ERROR;
	}
	else
	{
		transData->transState = APPROVED;
		saveTransaction(transData);
		return APPROVED;
	}
}

EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
	int accountIndex = -1;

	for (int i = 0; i < 255; i++)
	{
		if (strcmp(accountsDB[i].primaryAccountNumber, cardData->primaryAccountNumber) == 0)
		{
			accountIndex = i;
			break;
		}
	}

	if (accountIndex >= 0)
	{
		accountRefrence->balance = accountsDB[accountIndex].balance;
		strcpy(accountRefrence->primaryAccountNumber, accountsDB[accountIndex].primaryAccountNumber);
		accountRefrence->state = accountsDB[accountIndex].state;
		return SERVER_OK;
	}
	else
	{
		return ACCOUNT_NOT_FOUND;
	}
}

EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
	if (accountRefrence->state == RUNNING)
	{
		return SERVER_OK;
	}
	else
	{
		return BLOCKED_ACCOUNT;
	}
}

EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
	if (termData->transAmount > accountRefrence->balance)
	{
		return LOW_BALANCE;
	}
	else
	{
		return SERVER_OK;
	}
}

EN_serverError_t saveTransaction(ST_transaction_t* transData)
{
	uint32_t lastTransactionNumber;

	if (transactionIndex >= 0)
	{
		lastTransactionNumber = transActionDB[transactionIndex].transactionSequenceNumber;
	}
	else lastTransactionNumber = 1111111110;

	transactionIndex++;
	
	transActionDB[transactionIndex].cardHolderData = transData->cardHolderData;
	transActionDB[transactionIndex].terminalData = transData->terminalData;
	transActionDB[transactionIndex].transState = transData->transState;
	transActionDB[transactionIndex].transactionSequenceNumber = lastTransactionNumber + 1;

	transData->transactionSequenceNumber = transActionDB[transactionIndex].transactionSequenceNumber;

	return SERVER_OK;
}

void listSavedTransactions(void)
{
	if (transactionIndex < 0)
	{
		printf("\033[31m******* No saved transactions !!! *******\033[0m\n\n");
	}
	else
	{
		printf("\033[32m******* Transactions List *******\033[0m\n\n");

		for (int i = 0; i <= transactionIndex; i++)
		{
			printf("*********************************\n");
			printf("Transaction Sequence Number: %d\n", transActionDB[i].transactionSequenceNumber);
			printf("Transaction Date: %s\n", transActionDB[i].terminalData.transactionDate);
			printf("Transaction Amount: %.2f\n", transActionDB[i].terminalData.transAmount);
			printTransState(transActionDB[i].transState);
			printf("Terminal Max Amount: %.2f\n", transActionDB[i].terminalData.maxTransAmount);
			printf("Cardholder Name: %s\n", transActionDB[i].cardHolderData.cardHolderName);
			printf("PAN: %s\n", transActionDB[i].cardHolderData.primaryAccountNumber);
			printf("Card Expiration Date: %s\n", transActionDB[i].cardHolderData.cardExpirationDate);
			transActionDB[i].cardHolderData.cardHolderName;
			printf("*********************************\n\n");
		}

		printf("******* Transactions List End *******\n\n");
	}

	return;
}


void recieveTransactionDataTest(void)
{
	EN_serverError_t serverError;
	struct ST_cardData_t tempCard;
	struct ST_terminalData_t tempTerminal;
	struct ST_transaction_t tempTransData;

	strcpy(tempCard.cardExpirationDate, "05/24");
	strcpy(tempCard.cardHolderName, "Ahmed Helmi Abdel Wahab");

	strcpy(tempTerminal.transactionDate, "09/11/2022");
	tempTerminal.maxTransAmount = 10000;
	tempTerminal.transAmount = 3500;

	tempTransData.terminalData = tempTerminal;
	//tempTransData.transState = APPROVED;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: recieveTransactionData\n\n");

	strcpy(tempCard.primaryAccountNumber, "4444555566667777");
	tempTransData.cardHolderData = tempCard;

	printf("Test Case 1:\n");
	printf("Input Data: Invalid account\n");
	serverError = recieveTransactionData(&tempTransData);
	printf("Expected Result: %d\n", FRAUD_CARD);
	printf("Actual Result: %d\n\n", serverError);

	strcpy(tempCard.primaryAccountNumber, "9875431654901057");
	tempTransData.cardHolderData = tempCard;

	printf("Test Case 2:\n");
	printf("Input Data: Low account balance\n");
	serverError = recieveTransactionData(&tempTransData);
	printf("Expected Result: %d\n", DECLINED_INSUFFECIENT_FUND);
	printf("Actual Result: %d\n\n", serverError);

	strcpy(tempCard.primaryAccountNumber, "69873165624460077");
	tempTransData.cardHolderData = tempCard;

	printf("Test Case 3:\n");
	printf("Input Data: Stolen card\n");
	serverError = recieveTransactionData(&tempTransData);
	printf("Expected Result: %d\n", DECLINED_STOLEN_CARD);
	printf("Actual Result: %d\n\n", serverError);

	strcpy(tempCard.primaryAccountNumber, "9721316464321032");
	tempTransData.cardHolderData = tempCard;

	printf("Test Case 3:\n");
	printf("Input Data: Valid data\n");
	serverError = recieveTransactionData(&tempTransData);
	printf("Expected Result: %d\n", APPROVED);
	printf("Actual Result: %d\n\n", serverError);

	listSavedTransactions();
}

void isValidAccountTest(void)
{
	EN_serverError_t serverError;
	struct ST_cardData_t tempCardData;
	struct ST_accountsDB_t tempAccount;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: isValidAccount\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: \"8888777766665555\"\n");
	strcpy(tempCardData.primaryAccountNumber, "8888777766665555");
	serverError = isValidAccount(&tempCardData, &tempAccount);
	printf("Expected Result: %d\n", ACCOUNT_NOT_FOUND);
	printf("Actual Result: %d\n\n", serverError);

	printf("Test Case 2:\n");
	printf("Input Data: \"54648456464654002\"\n");
	strcpy(tempCardData.primaryAccountNumber, "54648456464654002");
	serverError = isValidAccount(&tempCardData, &tempAccount);
	printf("Expected Result: %d\n", SERVER_OK);
	printf("Actual Result: %d\n\n", serverError);

	printf("Test Case 3:\n");
	printf("Input Data: \"12345165165679002\"\n");
	strcpy(tempCardData.primaryAccountNumber, "12345165165679002");
	serverError = isValidAccount(&tempCardData, &tempAccount);
	printf("Expected Result: %d\n", SERVER_OK);
	printf("Actual Result: %d\n\n", serverError);
}

void isBlockedAccountTest(void)
{
	EN_serverError_t serverError;
	struct ST_accountsDB_t tempAccount;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: isBlockedAccount\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: RUNNING\n");
	tempAccount.state = RUNNING;
	serverError = isBlockedAccount(&tempAccount);
	printf("Expected Result: %d\n", SERVER_OK);
	printf("Actual Result: %d\n\n", serverError);

	printf("Test Case 2:\n");
	printf("Input Data: BLOCKED\n");
	tempAccount.state = BLOCKED;
	serverError = isBlockedAccount(&tempAccount);
	printf("Expected Result: %d\n", BLOCKED_ACCOUNT);
	printf("Actual Result: %d\n\n", serverError);
}

void isAmountAvailableTest(void)
{
	EN_serverError_t serverError;
	struct ST_terminalData_t tempterminal;
	struct ST_accountsDB_t tempAccount;

	tempterminal.transAmount = 5000;

	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: isAmountAvailable\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: Balance(1000) & Tran Amount(5000)\n");
	tempAccount.balance = 1000;
	serverError = isAmountAvailable(&tempterminal, &tempAccount);
	printf("Expected Result: %d\n", LOW_BALANCE);
	printf("Actual Result: %d\n\n", serverError);

	printf("Test Case 2:\n");
	printf("Input Data: Balance(10000) & Tran Amount(5000)\n");
	tempAccount.balance = 10000;
	serverError = isAmountAvailable(&tempterminal, &tempAccount);
	printf("Expected Result: %d\n", SERVER_OK);
	printf("Actual Result: %d\n\n", serverError);

	printf("Test Case 3:\n");
	printf("Input Data: Balance(5000) & Tran Amount(5000)\n");
	tempAccount.balance = 5000;
	serverError = isAmountAvailable(&tempterminal, &tempAccount);
	printf("Expected Result: %d\n", SERVER_OK);
	printf("Actual Result: %d\n\n", serverError);
}

void saveTransactionTest(void)
{
	EN_serverError_t serverError;
	struct ST_cardData_t tempCard;
	struct ST_terminalData_t tempTerminal;
	struct ST_transaction_t tempTransData;

	strcpy(tempCard.cardExpirationDate, "05/24");
	strcpy(tempCard.cardHolderName, "Ahmed Helmi Abdel Wahab");
	strcpy(tempCard.primaryAccountNumber, "9721316464321032");

	strcpy(tempTerminal.transactionDate, "09/11/2022");
	tempTerminal.maxTransAmount = 10000;
	tempTerminal.transAmount = 3500;

	tempTransData.cardHolderData = tempCard;
	tempTransData.terminalData = tempTerminal;
	tempTransData.transState = APPROVED;
	
	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: saveTransaction\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: Valid data with transState= APPROVED\n");
	serverError = saveTransaction(&tempTransData);
	printf("Expected Result: %d\n", SERVER_OK);
	printf("Expected transaction sequance number: %d\n", 1111111111);
	printf("Actual Result: %d\n", serverError);
	printf("Actual transaction sequance number: %d\n", tempTransData.transactionSequenceNumber);

	tempTransData.transState = DECLINED_INSUFFECIENT_FUND;

	printf("Test Case 2:\n");
	printf("Input Data: Valid data with transState= APPROVED\n");
	serverError = saveTransaction(&tempTransData);
	printf("Expected Result: %d\n", SERVER_OK);
	printf("Expected transaction sequance number: %d\n", 1111111112);
	printf("Actual Result: %d\n", serverError);
	printf("Actual transaction sequance number: %d\n\n", tempTransData.transactionSequenceNumber);
}

void saveTransactionTestMute(void)
{
	EN_serverError_t serverError;
	struct ST_cardData_t tempCard;
	struct ST_terminalData_t tempTerminal;
	struct ST_transaction_t tempTransData;

	strcpy(tempCard.cardExpirationDate, "05/24");
	strcpy(tempCard.cardHolderName, "Ahmed Helmi Abdel Wahab");
	strcpy(tempCard.primaryAccountNumber, "9721316464321032");

	strcpy(tempTerminal.transactionDate, "09/11/2022");
	tempTerminal.maxTransAmount = 10000;
	tempTerminal.transAmount = 3500;

	tempTransData.cardHolderData = tempCard;
	tempTransData.terminalData = tempTerminal;
	tempTransData.transState = APPROVED;

	serverError = saveTransaction(&tempTransData);

	tempTransData.transState = DECLINED_INSUFFECIENT_FUND;

	serverError = saveTransaction(&tempTransData);
}

void listSavedTransactionsTest(void)
{
	printf("Tester name: Ahmed Helmi\n");
	printf("Function Name: listSavedTransactions\n\n");

	printf("Test Case 1:\n");
	printf("Input Data: Empty transaction list\n");
	printf("Expected Result: No saved transactions !!!\n");
	printf("Actual Result: \n");
	listSavedTransactions();
	
	saveTransactionTestMute();

	printf("Test Case 2:\n");
	printf("Input Data: Transaction list have 2 records\n");
	printf("Expected Result: List 2 records data\n");
	printf("Actual Result: \n\n");
	listSavedTransactions();
}

void printTransState(int transState)
{
	if (transState == 0) printf("\033[32mTransaction State : APPROVED \033[0m\n");
	else if (transState == 1) printf("\033[31mTransaction State : DECLINED INSUFFECIENT FUND \033[0m\n");
	else if (transState == 2) printf("\033[31mTransaction State : DECLINED STOLEN CARD \033[0m\n");
	else if (transState == 3) printf("\033[31mTransaction State : FRAUD CARD \033[0m\n");
	else if (transState == 4) printf("\033[31mTransaction State : INTERNAL SERVER ERROR \033[0m\n");
	else printf("\033[31mTransaction State : *** Error *** \033[0m\n");
}

EN_serverError_t updateAccountBalance(ST_transaction_t* transData)
{
	int accountIndex = -1;

	for (int i = 0; i < 255; i++)
	{
		if (strcmp(accountsDB[i].primaryAccountNumber, transData->cardHolderData.primaryAccountNumber) == 0)
		{
			accountIndex = i;
			break;
		}
	}

	if (accountIndex >= 0) 
	{
		if (accountsDB[accountIndex].balance > transData->terminalData.transAmount)
		{
			accountsDB[accountIndex].balance = accountsDB[accountIndex].balance - transData->terminalData.transAmount;

			printf("\n\033[95mAccount Left Balance: %.2f\033[0m\n\n", accountsDB[accountIndex].balance);
			return SERVER_OK;
		}
	}
	
	return SAVING_FAILED;
}
