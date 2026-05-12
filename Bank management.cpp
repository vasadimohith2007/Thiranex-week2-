#include <iostream>
#include <fstream>
using namespace std;

class Account {
private:
    int accNo;
    char name[50];
    float balance;

public:
    void createAccount() {
        cout << "Enter Account Number: ";
        cin >> accNo;
        cout << "Enter Account Holder Name: ";
        cin.ignore();
        cin.getline(name, 50);
        cout << "Enter Initial Balance: ";
        cin >> balance;
    }

    void showAccount() const {
        cout << "\nAccount Number: " << accNo;
        cout << "\nName: " << name;
        cout << "\nBalance: " << balance << endl;
    }

    void deposit(float amt) {
        balance += amt;
    }

    void withdraw(float amt) {
        if (amt <= balance)
            balance -= amt;
        else
            cout << "Insufficient balance!\n";
    }

    int getAccNo() const {
        return accNo;
    }

    float getBalance() const {
        return balance;
    }
};

// Function to write account to file
void writeAccount() {
    Account acc;
    ofstream outFile("bank.dat", ios::binary | ios::app);

    acc.createAccount();
    outFile.write(reinterpret_cast<char*>(&acc), sizeof(Account));

    outFile.close();
    cout << "Account Created Successfully!\n";
}

// Function to display all accounts
void displayAll() {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        acc.showAccount();
        cout << "---------------------\n";
    }

    inFile.close();
}

// Function to search account
void searchAccount(int accNo) {
    Account acc;
    ifstream inFile("bank.dat", ios::binary);
    bool found = false;

    while (inFile.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == accNo) {
            acc.showAccount();
            found = true;
        }
    }

    if (!found)
        cout << "Account not found!\n";

    inFile.close();
}

// Deposit / Withdraw update
void updateAccount(int accNo, float amount, bool isDeposit) {
    Account acc;
    fstream file("bank.dat", ios::binary | ios::in | ios::out);
    bool found = false;

    while (!file.eof() && file.read(reinterpret_cast<char*>(&acc), sizeof(Account))) {
        if (acc.getAccNo() == accNo) {
            if (isDeposit)
                acc.deposit(amount);
            else
                acc.withdraw(amount);

            int pos = (-1) * sizeof(Account);
            file.seekp(pos, ios::cur);
            file.write(reinterpret_cast<char*>(&acc), sizeof(Account));

            cout << "Transaction successful!\n";
            found = true;
            break;
        }
    }

    if (!found)
        cout << "Account not found!\n";

    file.close();
}

// Main Menu
int main() {
    int choice, accNo;
    float amount;

    do {
        cout << "\n===== BANK MENU =====\n";
        cout << "1. Create Account\n";
        cout << "2. Display All Accounts\n";
        cout << "3. Search Account\n";
        cout << "4. Deposit\n";
        cout << "5. Withdraw\n";
        cout << "6. Exit\n";
        cout << "Enter Choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                writeAccount();
                break;

            case 2:
                displayAll();
                break;

            case 3:
                cout << "Enter Account Number: ";
                cin >> accNo;
                searchAccount(accNo);
                break;

            case 4:
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter Amount: ";
                cin >> amount;
                updateAccount(accNo, amount, true);
                break;

            case 5:
                cout << "Enter Account Number: ";
                cin >> accNo;
                cout << "Enter Amount: ";
                cin >> amount;
                updateAccount(accNo, amount, false);
                break;
        }

    } while (choice != 6);

    return 0;
}
