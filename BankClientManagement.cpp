#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

const string ClientsFileName = "Clients.txt";

struct sClient {
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

void AddDataLineToFile(string FileName, string stDataLine) {
    fstream Myfile;
    Myfile.open(FileName, ios::out | ios::app); //Read Mode
    if (Myfile.is_open()) {
        Myfile << stDataLine << endl;
        Myfile.close();
    }
}

sClient ReadNewClient() {
    sClient Client;
    cout << "Enter Account Number? ";
    getline(cin >> ws, Client.AccountNumber);

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter  Name? ";
    getline(cin, Client.Name);

    cout << "Enter  Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter  AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

vector<string> SplitString(string S1, string Delim) {
    vector<string> vString;
    short pos = 0;
    string sWord;

    while ((pos = S1.find(Delim)) != std::string::npos) {
        sWord = S1.substr(0, pos);
        if (sWord != "") {
            vString.push_back(sWord);
        }
        S1.erase(0, pos + Delim.length());
    }

    if (S1 != "") {
        vString.push_back(S1);
    }

    return vString;
}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#") {
    sClient Client;
    vector<string> vClientData;

    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);

    return Client;
}

string ConvertRecordToLine(sClient Client, string Seperator = "#//#") {
    string stClientRecord = "";

    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);

    return stClientRecord;
}

vector <sClient> LoadCleintsDataFromFile(string FileName) {
    vector <sClient> vClients;
    fstream MyFile;

    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        string Line;
        sClient Client;

        while (getline(MyFile, Line)) {
            if (Line.size() == 0) continue;
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }

        MyFile.close();
    }

    return vClients;
}

void PrintClientCard(sClient Client) {
    cout << "\nThe following are the client details:\n";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
}

bool FindClientByAccountNumber(string AccountNumber, vector<sClient> vClients, sClient& Client) {
    for (sClient C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            Client = C;
            return true;
        }
    }
    return false;
}

void AddNewClient() {
    vector<sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    sClient Client = ReadNewClient();

    // check if account number already exists
    sClient Temp;
    if (FindClientByAccountNumber(Client.AccountNumber, vClients, Temp)) {
        cout << "\nClient with Account Number (" << Client.AccountNumber << ") already exists!\n";
        return; // stop and don't add
    }

    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
    cout << "\nClient Added Successfully.\n";
}
void AddClient() {
    char AddMore = 'Y';
    do {
        system("Cls");
        cout << "Adding New Client: \n\n";
        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? (Y/N)\n";
        cin >> AddMore;
    } while (toupper(AddMore) == 'Y');
}



bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients) {
    for (sClient& C : vClients) {
        if (C.AccountNumber == AccountNumber) {
            C.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector<sClient> vClients) {
    fstream MyFile;
    MyFile.open(FileName, ios::out);

    string DataLine;

    if (MyFile.is_open()) {
        for (sClient C : vClients) {
            if (C.MarkForDelete == false) {
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }

    return vClients;
}

sClient ChangeClientRecord(string AccountNumber) {
    sClient Client;
    Client.AccountNumber = AccountNumber;

    cout << "\n\n Enter PinCode?";
    getline(cin >> ws, Client.PinCode);

    cout << "\n\n Enter Name?";
    getline(cin, Client.Name);

    cout << "\n\n Enter Phone?";
    getline(cin, Client.Phone);

    cout << "\n\n Enter Account Balance?";
    cin >> Client.AccountBalance;

    return Client;
}

void PrintClientRecord(sClient Client) {
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void PrintAllClientsData(vector<sClient> vClients)
{
    cout << "\n\t\t\t\t\tClient List ("
        << vClients.size() << ") Client(s).";

    cout << "\n_______________________________________________________"
        << "_________________________________________\n\n";

    cout << "| " << left << setw(15) << "Account Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";

    cout << "\n_______________________________________________________"
        << "_________________________________________\n";

    for (const sClient& Client : vClients)
    {
        PrintClientRecord(Client);
        cout << endl;
    }

    cout << "\n_______________________________________________________"
        << "_________________________________________\n";
}

bool UpdateClientRecord(string AccountNumber, vector<sClient>& vClients) {
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want to update this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {
            for (sClient& c : vClients) {
                if (c.AccountNumber == AccountNumber) {
                    c = ChangeClientRecord(AccountNumber);
                    break;
                }
            }
            SaveCleintsDataToFile(ClientsFileName, vClients);
            cout << "\n\nClient Updated Successfully.";
            return true;
        }
    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

    return false;
}

void PrintFClientData(vector<sClient> vClients)
{
    string Number;
    cout << "Please enter Account Number? ";
    getline(cin >> ws, Number);

    sClient Client;
    if (FindClientByAccountNumber(Number, vClients, Client))
    {
        PrintClientRecord(Client);
        cout << endl;
        return; // وقف بعد ما نلاقيه
    }

    cout << "\nClient With Account Number (" << Number << ") Not Found!\n";
}

string ReadClientAccountNumber() {
    string AccountNumber = "";
    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;
}

bool DeleteClientByAccountNumber(string AccountNumber, vector<sClient>& vClients) {
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }

    return false;
}

void Choose() {
    while (true) {
        vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

        cout << "=========================================";
        cout << "\n             Main menu screen          ";
        cout << "\n=========================================";
        cout << "\n[1] Show Client List.";
        cout << "\n[2] Add New Client.";
        cout << "\n[3] Delete Client.";
        cout << "\n[4] Update Client Info.";
        cout << "\n[5] Find Client.";
        cout << "\n[6] Exit.";
        cout << "\n=========================================";
        cout << "\nChoose what do you want to do ? [1 to 6]? ";
        int Number;
        cin >> Number;

        if (Number == 1) {
            PrintAllClientsData(vClients);
        }
        else if (Number == 2) {
            AddClient();
        }
        else if (Number == 3) {
            string AccountNumber = ReadClientAccountNumber();
            DeleteClientByAccountNumber(AccountNumber, vClients);
        }
        else if (Number == 4) {
            string AccountNumber = ReadClientAccountNumber();
            UpdateClientRecord(AccountNumber, vClients);
        }
        else if (Number == 5) {
            PrintFClientData(vClients);
        }
        else if (Number == 6) {
            cout << "\n-------------------------------";
            cout << "\n       Program Ends :-)        ";
            cout << "\n-------------------------------";
            break;
        }
        else {
            cout << "\nInvalid choice, try again.\n";
        }

        cout << "\n\nPress Enter to continue...";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cin.get();
        system("cls");
    }
}

int main() {
    Choose();
    system("pause>0");
    return 0;
}
