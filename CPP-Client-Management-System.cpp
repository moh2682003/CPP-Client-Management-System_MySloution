#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>

using namespace std;

const string FileName = "Client File.txt";

enum enChoise
{
	Show_Client_List = 1,
	Add_New_Client = 2,
	Delet_Client = 3,
	UpData_Client = 4,
	Find_Account = 5,
	Exit = 6,
};

struct stClient
{
	string AccountNumber;
	string PinCode;
	string AccountName;
	string PhoneNumber;
	double AccountBlance;
	bool MarkClient = false;
};


stClient GetClientInfo()
{
	stClient Client;
	cout << "Enter Account Number: ";
	getline(cin >> ws, Client.AccountNumber);

	cout << "Enter PIN Code: ";
	getline(cin, Client.PinCode);

	cout << "Enter Account Name: ";
	getline(cin, Client.AccountName);

	cout << "Enter Phone Number: ";
	getline(cin, Client.PhoneNumber);

	cout << "Enter Account Blance: ";
	cin >> Client.AccountBlance;

	return Client;


}

void PrintClientCard(stClient Client)
{
	cout << "============================Client Card=======================================\n";
	cout << "Account Number: " << Client.AccountNumber << endl;
	cout << "Pin Code: " << Client.PinCode << endl;
	cout << "Account Name: " << Client.AccountName << endl;
	cout << "Phone Number: " << Client.PhoneNumber << endl;
	cout << "Account Blance: " << Client.AccountBlance << endl;
}

string PrintDataInOneLine(stClient Client, string Sperator = "##**##")
{
	string OneLineData = "";

	OneLineData += Client.AccountNumber + Sperator;
	OneLineData += Client.PinCode + Sperator;
	OneLineData += Client.AccountName + Sperator;
	OneLineData += Client.PhoneNumber + Sperator;
	OneLineData += to_string(Client.AccountBlance);

	return OneLineData;
}

vector <string> SplitFunction(string S1 ,string Delim )
{
	vector <string> Vstring;
	int pos;
	string Sword;

	while ((pos = S1.find(Delim)) != std::string::npos)
	{
		Sword = S1.substr(0, pos);
		if (!Sword.empty())
		{
			Vstring.push_back(Sword);
		}
		S1.erase(0, pos + Delim.length());
	}
	if (!S1.empty())
	{
		Vstring.push_back(S1);
	}
	return Vstring;

}

stClient ConvertOneLineToRecord(string OneLineData , string Sperator = "##**##")
{
	vector <string> Vstring = SplitFunction(OneLineData, Sperator);
	stClient Client;

	Client.AccountNumber = Vstring[0];
	Client.PinCode = Vstring[1];
	Client.AccountName = Vstring[2];
	Client.PhoneNumber = Vstring[3];
	Client.AccountBlance = stod(Vstring[4]);
	return Client;
}

vector <stClient> LoadDataFromFile(string FileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	vector <stClient> Vclient;

	if (MyFile.is_open())
	{
		string line;
		stClient Client;

		while (getline(MyFile, line))
		{
			Client = ConvertOneLineToRecord(line);
			Vclient.push_back(Client);
		}
	}
	MyFile.close();
	return Vclient;
}

void PrintClientInfoInTable(stClient Client)
{
	cout <<"|" << left << setw(15) << Client.AccountNumber;
	cout <<"|" << left << setw(10) << Client.PinCode;
	cout <<"|" << left << setw(40) << Client.AccountName;
	cout <<"|" << left << setw(15) << Client.PhoneNumber;
	cout <<"|" << left << setw(10) << Client.AccountBlance;
}

void ShowClietList(vector <stClient> Vclient)
{
	cout << "\n\t==============================================Client Table==========================\n";
	cout << "\t=====================================Number Of Client(" << Vclient.size() << ")Client(s)===================\n";
	cout << "\t-------------------------------------------------------------------------------------\n";
	cout << "|" << left << setw(15) << "Account Number";
	cout << "|" << left << setw(10) << "Pin Code";
	cout << "|" << left << setw(40) << "Client Name";
	cout << "|" << left << setw(15) << "Phone Number";
	cout << "|" << left << setw(10) << "Blance";
	cout << endl;

	for (stClient &C : Vclient)
	{
		PrintClientInfoInTable(C);
		cout << endl;
	}
}

void GetClientInFile(string FileName, string OneLineData)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << OneLineData << endl;
	}
	MyFile.close();

}

void FillTheFile()
{
	stClient Client = GetClientInfo();
	string ClientData = PrintDataInOneLine(Client);
	GetClientInFile(FileName, ClientData);
}

void AddNewClient()
{
	char Answer = 'Y';
	
	do
	{
		system("cls");

		FillTheFile();
		cout << "Your Client Successful Add\n";
		cout << "Do You Want To Add Another Client? Y/N? ";
		cin >> Answer;
	} while (toupper(Answer) == 'Y');
}

bool FindAccountByAccountNumber(string AccountNumber, stClient &Client, vector <stClient> &Vclient)
{
	for (stClient& C : Vclient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			Client = C;
			return true;
		}
	}
	return false;
}

string GetAccountNumber()
{
	string AccountNumber;
	cout << "Enter Account Number: ";
	cin >> AccountNumber;
	return AccountNumber;
}

void FindAccount()
{
	system("cls");
	vector <stClient> Vclient = LoadDataFromFile(FileName);
	string AccountNumber = GetAccountNumber();
	stClient Client;
	Client.AccountNumber = AccountNumber;
	if (FindAccountByAccountNumber(AccountNumber, Client, Vclient))
	{
		PrintClientCard(Client);
	}
	else
	{
		cout << "Your Client By This" << AccountNumber << "Account Number Not Found !\n";

	}
}

bool MarkAccountToDelet(string AccountNumber, vector <stClient> &Vclient)
{
	for (stClient& C : Vclient)
	{
		if (C.AccountNumber == AccountNumber)
		{
			return C.MarkClient = true;
		}
	}
	return false;
}

vector <stClient> SaveDataAfterUpData(string FileName, vector <stClient> Vclient)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	string DataLine;
	if (MyFile.is_open())
	{
		string DataLine;
		for (stClient& C : Vclient)
		{
			if (C.MarkClient == false)
			{
				DataLine = PrintDataInOneLine(C);
				MyFile << DataLine << endl;
			}
		}
	}
	MyFile.close();
	return Vclient;
}

bool DeletAccount( vector<stClient> &Vclient)
{
	stClient Client;
	char Answer;
	string AccountNumber = GetAccountNumber();

	if (FindAccountByAccountNumber(AccountNumber, Client, Vclient))
	{
		PrintClientCard(Client);
		cout << "Do you sure you want to delet this client?  Y/N?";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			MarkAccountToDelet(AccountNumber, Vclient);
			SaveDataAfterUpData(FileName, Vclient);
			cout << "Your Client Successful Delet";
			Vclient = LoadDataFromFile(FileName);
			return true;
		}
	}
	else
	{
		cout << "Your Client By " << AccountNumber << "Not Found !";
		return false;
	}
}

stClient UpDataClientInfo(string AccountNumber)
{
	stClient Client;
	Client.AccountNumber = AccountNumber;

	
	cout << "Enter Pin Code: ";
	getline(cin >> ws, Client.PinCode);

	cout << "Enter Client Name: ";
	getline(cin, Client.AccountName);

	cout << "Enter PhoneNumber: ";
	getline(cin, Client.PhoneNumber);

	cout << "Blance: ";
	cin >> Client.AccountBlance;

	return Client;
}

void UpDataClient(vector <stClient> &Vclient)
{
	stClient Client;
	char Answer;
	string AccountNumber = GetAccountNumber();


	if (FindAccountByAccountNumber(AccountNumber, Client, Vclient))
	{
		system("cls");
		PrintClientCard(Client);
		cout << "You Want To UpData This Info?  Y/N? ";
		cin >> Answer;
		if (toupper(Answer) == 'Y')
		{
			for (stClient& C : Vclient)
			{
				
				if (C.AccountNumber == AccountNumber)
				{
					C = UpDataClientInfo(AccountNumber);
					break;
				}
				
			}
			SaveDataAfterUpData(FileName, Vclient);
			
		}
	}
	else
	{
		cout << "Your Client By This" << AccountNumber << "Account Number Not Found !";
		
	}
}

void MainMenu()
{
	system("cls");
	cout << "======================Main Menu===============================\n";
	cout << "================================================================\n";
	cout << "[1] Show Client List\n";
	cout << "[2] Add New Client\n";
	cout << "[3] Delet Client\n";
	cout << "[4] Updata Client Info\n";
	cout << "[5] Find Account\n";
	cout << "[6] Exit\n";
	cout << "================================================\n";
}

int ReadLimtedNumber(int From , int To)
{
	int Number;
	cout << "Enter Your Choise: ";
	cin >> Number;
	while (From <= Number <= To)
		return Number;
}


int main()
{
	
	
	char Answer = 'Y';
	vector <stClient> Vclient = LoadDataFromFile(FileName);
	

	do
	{
		MainMenu();

		int Number = ReadLimtedNumber(1, 6);
		enChoise Choise;
		Choise = enChoise(Number);
		
		switch (Choise)
		{
		case::enChoise::Show_Client_List:
			ShowClietList(Vclient);
			SaveDataAfterUpData(FileName, Vclient);
			break;
		case::enChoise::Add_New_Client:
			AddNewClient();
			SaveDataAfterUpData(FileName, Vclient);
			break;
		case::enChoise::Delet_Client:
			DeletAccount( Vclient);
			SaveDataAfterUpData(FileName, Vclient);
			break;
		case::enChoise::UpData_Client:
			UpDataClient( Vclient);
			SaveDataAfterUpData(FileName, Vclient);
			break;
		case::enChoise::Find_Account:
			FindAccount();
			SaveDataAfterUpData(FileName, Vclient);
			break;
			case::enChoise::Exit:
				break;
		}
		cout << "Do You Want Another Servise? Y/N? ";
		cin >> Answer;
	} while (toupper(Answer) == 'Y');
}
