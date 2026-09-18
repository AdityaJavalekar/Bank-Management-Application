#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>

using namespace std;

// Class representing a Bank Account (Object-Oriented Design)
class BankAccount {
private:
    int accountNumber;
    string accountHolder;
    double balance;

public:
    // Constructors
    BankAccount() : accountNumber(0), accountHolder(""), balance(0.0) {}
    BankAccount(int accNum, string name, double initialDeposit) {
        accountNumber = accNum;
        accountHolder = name;
        balance = initialDeposit;
    }

    // Getters
    int getAccountNumber() const { return accountNumber; }
    string getAccountHolder() const { return accountHolder; }
    double getBalance() const { return balance; }

    // Core Business Operations
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "\nSuccess: $" << fixed << setprecision(2) << amount << " deposited successfully.";
        } else {
            cout << "\nError: Deposit amount must be positive.";
        }
    }

    bool withdraw(double amount) {
        if (amount <= 0) {
            cout << "\nError: Withdrawal amount must be greater than zero.";
            return false;
        }
        if (amount > balance) {
            cout << "\nError: Insufficient funds! Current Balance: $" << fixed << setprecision(2) << balance;
            return false;
        }
        balance -= amount;
        cout << "\nSuccess: $" << fixed << setprecision(2) << amount << " withdrawn successfully.";
        return true;
    }

    void displayDetails() const {
        cout << "\n-----------------------------------------";
        cout << "\n Account Number : " << accountNumber;
        cout << "\n Account Holder : " << accountHolder;
        cout << "\n Current Balance: $" << fixed << setprecision(2) << balance;
        cout << "\n-----------------------------------------";
    }

    // Convert object data to pipe-delimited string for file storage
    string toFileString() const {
        return to_string(accountNumber) + "|" + accountHolder + "|" + to_string(balance);
    }
};

// Function Declarations
void createAccount();
void depositMoney();
void withdrawMoney();
void checkBalance();
void displayAllAccounts();
void displayMenu();

int main() {
    int choice;

    do {
        displayMenu();
        cout << "Enter your choice (1-6): ";
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number (1-6): ";
            cin.clear();
            cin.ignore(10000, '\n');
        }

        switch (choice) {
            case 1: createAccount(); break;
            case 2: depositMoney(); break;
            case 3: withdrawMoney(); break;
            case 4: checkBalance(); break;
            case 5: displayAllAccounts(); break;
            case 6: cout << "\nThank you for using our banking system. Goodbye!\n"; break;
            default: cout << "\nInvalid choice. Please select from 1 to 6.\n";
        }
    } while (choice != 6);

    return 0;
}

void displayMenu() {
    cout << "\n=========================================\n";
    cout << "        BANK MANAGEMENT SYSTEM           \n";
    cout << "=========================================\n";
    cout << " 1. Create New Account\n";
    cout << " 2. Deposit Funds\n";
    cout << " 3. Withdraw Funds\n";
    cout << " 4. Check Balance / Inquiry\n";
    cout << " 5. Display All Accounts\n";
    cout << " 6. Exit\n";
    cout << "-----------------------------------------\n";
}

void createAccount() {
    int accNum;
    string name;
    double initialDeposit;

    cout << "\n--- Create Account ---\n";
    cout << "Enter Account Number: ";
    cin >> accNum;
    cin.ignore();

    // Verify duplicate account number
    ifstream inFile("bank_records.txt");
    if (inFile) {
        string line;
        while (getline(inFile, line)) {
            if (line.empty()) continue;
            stringstream ss(line);
            string accStr;
            getline(ss, accStr, '|');
            if (stoi(accStr) == accNum) {
                cout << "\nError: Account number " << accNum << " already exists!\n";
                inFile.close();
                return;
            }
        }
        inFile.close();
    }

    cout << "Enter Account Holder Name: ";
    getline(cin, name);

    cout << "Enter Initial Deposit: $";
    cin >> initialDeposit;

    if (initialDeposit < 0) {
        cout << "\nError: Initial deposit cannot be negative.\n";
        return;
    }

    BankAccount account(accNum, name, initialDeposit);

    ofstream outFile("bank_records.txt", ios::app);
    if (!outFile) {
        cout << "\nError opening file for persistent storage.\n";
        return;
    }

    outFile << account.toFileString() << "\n";
    outFile.close();

    cout << "\nAccount successfully created for " << name << "!\n";
}

void depositMoney() {
    ifstream inFile("bank_records.txt");
    if (!inFile) {
        cout << "\nNo account records found.\n";
        return;
    }

    int targetAcc;
    cout << "\nEnter Account Number: ";
    cin >> targetAcc;

    ofstream tempFile("temp.txt");
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string accStr, name, balStr;

        getline(ss, accStr, '|');
        getline(ss, name, '|');
        getline(ss, balStr, '|');

        if (stoi(accStr) == targetAcc) {
            found = true;
            BankAccount account(stoi(accStr), name, stod(balStr));
            
            double amount;
            cout << "Enter deposit amount: $";
            cin >> amount;

            account.deposit(amount);
            tempFile << account.toFileString() << "\n";
        } else {
            tempFile << line << "\n";
        }
    }

    inFile.close();
    tempFile.close();

    remove("bank_records.txt");
    rename("temp.txt", "bank_records.txt");

    if (!found) {
        cout << "\nAccount Number " << targetAcc << " not found.\n";
    }
}

void withdrawMoney() {
    ifstream inFile("bank_records.txt");
    if (!inFile) {
        cout << "\nNo account records found.\n";
        return;
    }

    int targetAcc;
    cout << "\nEnter Account Number: ";
    cin >> targetAcc;

    ofstream tempFile("temp.txt");
    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string accStr, name, balStr;

        getline(ss, accStr, '|');
        getline(ss, name, '|');
        getline(ss, balStr, '|');

        if (stoi(accStr) == targetAcc) {
            found = true;
            BankAccount account(stoi(accStr), name, stod(balStr));
            
            double amount;
            cout << "Enter withdrawal amount: $";
            cin >> amount;

            account.withdraw(amount);
            tempFile << account.toFileString() << "\n";
        } else {
            tempFile << line << "\n";
        }
    }

    inFile.close();
    tempFile.close();

    remove("bank_records.txt");
    rename("temp.txt", "bank_records.txt");

    if (!found) {
        cout << "\nAccount Number " << targetAcc << " not found.\n";
    }
}

void checkBalance() {
    ifstream inFile("bank_records.txt");
    if (!inFile) {
        cout << "\nNo account records found.\n";
        return;
    }

    int targetAcc;
    cout << "\nEnter Account Number: ";
    cin >> targetAcc;

    string line;
    bool found = false;

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string accStr, name, balStr;

        getline(ss, accStr, '|');
        getline(ss, name, '|');
        getline(ss, balStr, '|');

        if (stoi(accStr) == targetAcc) {
            found = true;
            BankAccount account(stoi(accStr), name, stod(balStr));
            account.displayDetails();
            break;
        }
    }

    if (!found) {
        cout << "\nAccount Number " << targetAcc << " not found.\n";
    }
    inFile.close();
}

void displayAllAccounts() {
    ifstream inFile("bank_records.txt");
    if (!inFile) {
        cout << "\nNo account records found.\n";
        return;
    }

    string line;
    cout << "\n--------------------------------------------------\n";
    cout << left << setw(12) << "Acc No" 
         << setw(25) << "Account Holder" 
         << setw(12) << "Balance ($)" << "\n";
    cout << "--------------------------------------------------\n";

    bool found = false;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string accStr, name, balStr;

        getline(ss, accStr, '|');
        getline(ss, name, '|');
        getline(ss, balStr, '|');

        cout << left << setw(12) << accStr 
             << setw(25) << name 
             << setw(12) << fixed << setprecision(2) << stod(balStr) << "\n";
        found = true;
    }

    if (!found) {
        cout << "No bank records stored.\n";
    }
    cout << "--------------------------------------------------\n";
    inFile.close();
}