#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include "myLib.h"

using namespace std;

enum eMainOption { Show = 1, Add = 2, Delete = 3, Update = 4, Find = 5, Transaction = 6, MainExit = 7 };
enum eTransactionOption { Deposit = 1, Withdraw = 2, TotalBalance = 3, TransactionExit = 4 };

struct sClient
{
	string accountNumber;
	string pinCode;
	string name;
	string phone;
	double balance = 0.0;
	bool deleteFlag = false;
};

// global variables
string clientFileName = "clients.txt";
string delimiter = "|#|";

// declaretion
void showMainMenuOption();
void showTransactionMenuOption();

eMainOption readMainMenuOption()
{
	short answer = 0;

	do {

		cout << "\tChoose option number [1 - 7] : ";
		cin >> answer;
		cin.ignore(100, '\n');

	} while (answer < 1 || answer > 7);

	return (eMainOption)answer;
}

void goBackToMainMenu()
{
	cout << "\n\nto back to main menu press any key...";
	system("pause>0");

	showMainMenuOption();
}

sClient convertLineToClient(string line)
{
	sClient c;
	vector<string> vClientRecord;

	vClientRecord = mystd::splitString(line, delimiter);

	c.accountNumber = vClientRecord[0];
	c.pinCode = vClientRecord[1];
	c.name = vClientRecord[2];
	c.phone = vClientRecord[3];
	c.balance = stod(vClientRecord[4]);

	return c;
}

vector<sClient> loadClientsFromFile(string fileName)
{
	vector<sClient> vClients;

	fstream clientFile;
	clientFile.open(fileName, ios::in);

	if (clientFile.is_open())
	{
		sClient client;
		string line = "";

		while (getline(clientFile, line))
		{
			client = convertLineToClient(line);
			vClients.push_back(client);
		}

		clientFile.close();
	}

	return vClients;
}

void printClientRecordList(sClient client)
{
	cout << "  " << setw(10) << left << client.accountNumber;
	cout << "| " << setw(11) << left << client.pinCode;
	cout << "| " << setw(30) << left << client.name;
	cout << "| " << setw(18) << left << client.phone;
	cout << "| " << setw(18) << left << client.balance;
	cout << "|" << endl;
}

void printClientsList(const vector<sClient>& vClients)
{
	for (sClient c : vClients)
	{
		printClientRecordList(c);
	}

	cout << "__________________________________________________________________________________________________\n";

}

void showClientListScreen()
{
	cout << "\n                                  C L I E N T    R E C O R D S";
	cout << "\n__________________________________________________________________________________________________\n";
	cout << "   Number   |  pincode   |            name               |       phone       |      balance      |\n";
	cout << "__________________________________________________________________________________________________\n";

	vector<sClient> vClients = loadClientsFromFile(clientFileName);

	printClientsList(vClients);

	cout << "\n\t Client list(s) number = " << vClients.size() << endl;

}

string readAccountNumber()
{
	string accountNumber;

	cout << "enter account number : ";
	getline(cin >> ws, accountNumber);

	return accountNumber;
}

bool isAccountNumberExist(const vector<sClient>& vClients, string accountNumber)
{
	for (const sClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			return true;
		}
	}

	return false;
}

string readNewAccountNumber()
{
	string accountNumber;
	vector<sClient> vClients = loadClientsFromFile(clientFileName);

	accountNumber = readAccountNumber();
	while (isAccountNumberExist(vClients, accountNumber))
	{
		cout << "  Exist account number [" << accountNumber << "] plz enter another one !!.\n\n";
		accountNumber = readAccountNumber();
	}

	return accountNumber;
}

sClient readClientInfo(string accountNumber)
{
	sClient client;

	client.accountNumber = accountNumber;

	cout << "enter PinCode : ";
	getline(cin >> ws, client.pinCode);

	cout << "enter name : ";
	getline(cin, client.name);

	cout << "enter phone : ";
	getline(cin, client.phone);

	cout << "enter account balance : ";
	cin >> client.balance;

	return client;
}

sClient readNewClient()
{
	string accountNumber;

	accountNumber = readNewAccountNumber();

	sClient newClient = readClientInfo(accountNumber);

	return newClient;

}

bool addDataLineToFile(string fileName, string line)
{
	fstream file;

	file.open(fileName, ios::out | ios::app);
	if (file.is_open())
	{
		file << line << endl;

		file.close();
		return true;
	}

	return false;
}

string convertClientToLine(sClient client, string seperator)
{
	string line = "";

	line += client.accountNumber + seperator;
	line += client.pinCode + seperator;
	line += client.name + seperator;
	line += client.phone + seperator;
	line += to_string(client.balance);

	return line;
}

void addNewClient()
{
	sClient newClient;
	newClient = readNewClient();
	addDataLineToFile(clientFileName, convertClientToLine(newClient, delimiter));
}

void addNewClients()
{
	string accountNumber;
	sClient newClient;
	char answer;

	do
	{
		addNewClient();

		cout << "  [added successfuly]\n\n do u want to add more client [y-YES | n-NO] : ";
		cin >> answer;
		cin.ignore(100, '\n');

	} while (toupper(answer) == 'Y');

}

void showAddClientScreen()
{
	cout << "\n           A D D    C L I E N T \n\n";

	addNewClients();

}

bool searchClientByAccountNumber(vector<sClient>& vAllClient, string toSearch, sClient& client)
{
	for (sClient& c : vAllClient)
	{
		if (c.accountNumber == toSearch)
		{
			client = c;
			return true;
		}
	}

	return false;
}

void printClientCard(sClient client)
{
	cout << "\n\n----------------------------------\n";
	cout << "number  : " << client.accountNumber << endl;
	cout << "pinCode : " << client.pinCode << endl;
	cout << "name    : " << client.name << endl;
	cout << "phone   : " << client.phone << endl;
	cout << "balance : " << client.balance;
	cout << "\n----------------------------------\n";
}

bool updateFile(vector<sClient> vClients, string fileName)
{
	fstream clientFile;

	clientFile.open(fileName, ios::out);
	if (clientFile.is_open())
	{
		string line = "";

		for (sClient& c : vClients)
		{
			if (c.deleteFlag == false)
			{
				line = convertClientToLine(c, delimiter);
				clientFile << line << endl;
			}
		}

		clientFile.close();
		return true;
	}

	return false;
}

void markDeleteFlagClient(vector<sClient>& vClients, string accountNumber)
{
	for (sClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			c.deleteFlag = true;
		}
	}
}

bool deleteClientByAccountNumber(vector<sClient>& vClients, string accountNumber)
{
	sClient findedClient;
	char ans = 'n';

	if (searchClientByAccountNumber(vClients, accountNumber, findedClient))
	{
		printClientCard(findedClient);

		cout << "do u want to delete this client [y-YES | n-NO] : ";
		cin >> ans;
		cin.ignore(100, '\n');

		if (toupper(ans) == 'Y')
		{
			markDeleteFlagClient(vClients, accountNumber);
			updateFile(vClients, clientFileName);

			cout << "\n  [Client deleted successfuly]\n";

			return true;

		}

	}
	else
	{
		cout << "\nClient with Account Number [" << accountNumber << "] is Not Found!";
		return false;
	}
}

void showDeleteClientScreen()
{
	cout << "\n           D E L E T E    C L I E N T \n\n";

	string accountNumber = readAccountNumber();
	vector<sClient> vClients = loadClientsFromFile(clientFileName);

	deleteClientByAccountNumber(vClients, accountNumber);
}

bool updateClientRecord(vector<sClient>& vClients, string accountNumber)
{
	for (sClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			c = readClientInfo(accountNumber);

			return true;
		}
	}

	return false;
}

bool updateClientByAccountNumber(vector<sClient>& vClients, string accountNumber)
{
	sClient findedClient;
	char ans = 'n';

	if (searchClientByAccountNumber(vClients, accountNumber, findedClient))
	{
		printClientCard(findedClient);

		cout << "do u want to update this client [y-YES | n-NO] : ";
		cin >> ans;
		cin.ignore(100, '\n');

		if (toupper(ans) == 'Y')
		{
			updateClientRecord(vClients, accountNumber);
			updateFile(vClients, clientFileName);

			cout << "\n  [Client updated successfuly] \n";

			return true;

		}
	}
	else
	{
		cout << "\nClient with Account Number [" << accountNumber << "] is Not Found!";
		return false;
	}

}

void showUpdateClientScreen()
{
	cout << "\n           U P D A T E    C L I E N T \n\n";

	string accountNumber = readAccountNumber();
	vector<sClient> vClients = loadClientsFromFile(clientFileName);

	updateClientByAccountNumber(vClients, accountNumber);
}

void showFindClientScreen()
{

	cout << "\n           F I N D    C L I E N T \n\n";

	string accountNumber = readAccountNumber();
	vector<sClient> vClients = loadClientsFromFile(clientFileName);

	sClient findedClient;
	if (searchClientByAccountNumber(vClients, accountNumber, findedClient))
	{
		printClientCard(findedClient);

	}
	else
	{
		cout << "account number [" << accountNumber << "] is not found.";
	}
}

void showExitScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n-----------------------------------\n";
}

eTransactionOption readTransactionOption()
{
	short answer = 0;

	do {

		cout << "\tChoose option number [1 - 4] : ";
		cin >> answer;
		cin.ignore(100, '\n');

	} while (answer < 1 || answer > 4);

	return (eTransactionOption)answer;
}

void goBackToTransactionMenu()
{
	cout << "\n\nto back to transaction menu press any key...";
	system("pause>0");

	showTransactionMenuOption();
}

double readAmount(string message)
{
	double num = 0.0;

	cout << message;
	cin >> num;

	return num;
}

bool depositClientByAccountNumber(vector<sClient>& vClients, string accountNumber)
{
	double depositAmount = readAmount("enter deposit amonut : ");

	char ans = 'n';
	cout << "\n are you sure to deposit [" << depositAmount << "] amount [y-YES | n-NO] : ";
	cin >> ans;
	cin.ignore(100, '\n');

	if (toupper(ans) == 'Y')
	{
		double newBalance = 0.0;

		for (sClient& c : vClients)
		{
			if (c.accountNumber == accountNumber)
			{
				c.balance += depositAmount;
				newBalance = c.balance;
				break;
			}
		}

		updateFile(vClients, clientFileName);
		cout << "\n [Deposit successfuly done] \n";
		cout << " new balance = " << newBalance << endl;

		return true;
	}
	else
	{
		return false;
	}

}

void showDepositScreen()
{
	cout << "\n           D E P O S I T \n\n";

	string accountNumber = readAccountNumber();
	vector<sClient> vClients = loadClientsFromFile(clientFileName);
	sClient findedClient;

	while (!searchClientByAccountNumber(vClients, accountNumber, findedClient))
	{
		cout << "account number [" << accountNumber << "] is not exist, try another one !\n\n";
		accountNumber = readAccountNumber();
	}

	printClientCard(findedClient);

	depositClientByAccountNumber(vClients, accountNumber);
}

bool withdrawClientByAccountNumber(vector<sClient>& vClients, string accountNumber)
{
	double withdrawAmount = readAmount("enter withdraw amount : ");
	double newBalance = 0.0;

	for (sClient& c : vClients)
	{
		if (c.accountNumber == accountNumber)
		{
			while (withdrawAmount > c.balance)
			{
				cout << "u are poor, enter another amount...\n";
				withdrawAmount = readAmount("enter withdraw amount : ");
			}

			char ans = 'n';
			cout << "\n are you sure to withdraw [" << withdrawAmount << "] amount [y-YES | n-NO] : ";
			cin >> ans;
			cin.ignore(100, '\n');

			if (toupper(ans) == 'Y')
			{
				c.balance -= withdrawAmount;
				newBalance = c.balance;

				updateFile(vClients, clientFileName);
				cout << "\n [Deposit successfuly done] \n";
				cout << " new balance = " << newBalance << endl;

				return true;
			}

		}
	}

	return false;
}


void showWithdrawScreen()
{
	cout << "\n           W I T H D R A W \n\n";

	string accountNumber = readAccountNumber();
	vector<sClient> vClients = loadClientsFromFile(clientFileName);
	sClient findedClient;

	while (!searchClientByAccountNumber(vClients, accountNumber, findedClient))
	{
		cout << "account number [" << accountNumber << "] is not exist, try another one !\n\n";
		accountNumber = readAccountNumber();
	}

	printClientCard(findedClient);

	withdrawClientByAccountNumber(vClients, accountNumber);

}

void printShortClientList(sClient client)
{
	cout << "  " << setw(10) << left << client.accountNumber;
	cout << "| " << setw(30) << left << client.name;
	cout << "| " << setw(18) << left << client.balance;
	cout << "|" << endl;
}

void printTotalBalanceList(const vector<sClient>& vClients, double& totalBalance)
{
	for (const sClient& c : vClients)
	{
		printShortClientList(c);
		totalBalance += c.balance;
	}

}

void showTotalBalanceScreen()
{
	cout << "\n                   T O T A L   B A L A N C E \n";
	cout << "_________________________________________________________________\n";
	cout << "   Number   |            name               |      balance      |\n";
	cout << "_________________________________________________________________\n";

	double totalBalance = 0.0;
	vector<sClient> vClients = loadClientsFromFile(clientFileName);

	printTotalBalanceList(vClients, totalBalance);

	cout << "_________________________________________________________________\n";
	cout << "\n\n TOTAL BALANCE = " << totalBalance << endl;
}

void performTransactionMenuOption(eTransactionOption option)
{
	switch (option)
	{
	case eTransactionOption::Deposit:
		system("cls");
		showDepositScreen();
		goBackToTransactionMenu();
		break;

	case eTransactionOption::Withdraw:
		system("cls");
		showWithdrawScreen();
		goBackToTransactionMenu();
		break;

	case eTransactionOption::TotalBalance:
		system("cls");
		showTotalBalanceScreen();
		goBackToTransactionMenu();
		break;

	case eTransactionOption::TransactionExit:
		showMainMenuOption();
		break;

	}
}

void showTransactionMenuOption()
{
	system("cls");

	cout << "\t\t--------------------------------------\n";
	cout << "\t\t   T R A N S A C T I O N    M E N U   \n";
	cout << "\t\t--------------------------------------\n\n";

	cout << "\t\t [1] Deposit\n";
	cout << "\t\t [2] Withdraw \n";
	cout << "\t\t [3] Total Balance \n";
	cout << "\t\t [4] Main Menu\n";

	cout << "\t\t--------------------------------------\n\n";

	performTransactionMenuOption(readTransactionOption());
}

void performMainMenuOption(eMainOption option)
{

	switch (option)
	{
	case eMainOption::Show:
		system("cls");
		showClientListScreen();
		goBackToMainMenu();
		break;

	case eMainOption::Add:
		system("cls");
		showAddClientScreen();
		goBackToMainMenu();
		break;

	case eMainOption::Delete:
		system("cls");
		showDeleteClientScreen();
		goBackToMainMenu();
		break;

	case eMainOption::Update:
		system("cls");
		showUpdateClientScreen();
		goBackToMainMenu();
		break;

	case eMainOption::Find:
		system("cls");
		showFindClientScreen();
		goBackToMainMenu();
		break;

	case eMainOption::Transaction:
		showTransactionMenuOption();
		break;

	case eMainOption::MainExit:
		system("cls");
		showExitScreen();
		break;
	}
}

void showMainMenuOption()
{
	system("cls");

	cout << "\t\t------------------------------\n";
	cout << "\t\t      M A I N    M E N U      \n";
	cout << "\t\t------------------------------\n\n";

	cout << "\t\t [1] Show Client List\n";
	cout << "\t\t [2] Add New Client \n";
	cout << "\t\t [3] Delete Client \n";
	cout << "\t\t [4] Update Client Info\n";
	cout << "\t\t [5] Find Client\n";
	cout << "\t\t [6] Transaction Menu\n";
	cout << "\t\t [7] Exit\n\n";

	cout << "\t\t------------------------------\n\n";

	performMainMenuOption(readMainMenuOption());
}

int main() {

	showMainMenuOption();

	return 0;
}