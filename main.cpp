// EECS 348 EC Lab 3
// Name: Ella Nguyen
// ID: 3098942
// Goal: Banking System

#include <iostream>
#include <iomanip>
#include <string>

// Abstract base class representing a generic account
class Account {
protected:
    std::string accountNumber;
    std::string accountHolder;
    double balance;

public:
    // Constructor to initialize account details
    Account(const std::string& number, const std::string& holder, double initialBalance)
        : accountNumber(number), accountHolder(holder), balance(initialBalance) {}

    // Virtual destructor to allow proper cleanup in derived classes
    virtual ~Account() {}

    // Virtual function to get the type of account (to be overridden by derived classes)
    virtual std::string accountType() const {
        return "Generic";
    }

    // Virtual function to display extra details specific to each account type
    virtual void displayExtraDetails() const {}

    // Virtual function to deposit money into the account
    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
        } else {
            std::cout << "Invalid deposit amount." << std::endl;
        }
    }

    // Virtual function to withdraw money from the account
    virtual void withdraw(double amount) {
        if (amount > 0 && balance >= amount) {
            balance -= amount;
        } else {
            std::cout << "Invalid withdrawal amount or insufficient balance." << std::endl;
        }
    }

    // Getter function to retrieve the current balance
    double getBalance() const {
        return balance;
    }

    // Function to display account details
    void displayDetails() const {
        std::cout << "Account Details for " << accountType() << " Account (ID: " << accountNumber << "):" << std::endl;
        std::cout << "   Holder: " << accountHolder << std::endl;
        std::cout << "   Balance: $" << std::fixed << std::setprecision(2) << balance << std::endl;
        displayExtraDetails();
        std::cout << std::endl;
    }

    // Overloaded + operator for transferring money between accounts
    Account operator+(const Account& rhs) const {
        Account result("Transfer", "Transfer", 0);
        result.balance = this->balance + rhs.balance;

        // Deposit amount from rhs into result
        result.deposit(rhs.balance);

        // Withdraw amount from this
        result.withdraw(rhs.balance);

        return result;
    }

    // Friend function to overload the << operator for easy printing of account details
    friend std::ostream& operator<<(std::ostream& os, const Account& account);
};

// Overloaded << operator to display account details using cout
std::ostream& operator<<(std::ostream& os, const Account& account) {
    os << "Account Details for " << account.accountType() << " Account (ID: " << account.accountNumber << "):" << std::endl;
    os << "   Holder: " << account.accountHolder << std::endl;
    os << "   Balance: $" << std::fixed << std::setprecision(2) << account.balance << std::endl;
    account.displayExtraDetails();
    os << std::endl;
    return os;
}

// Derived class representing a savings account
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    // Constructor to initialize savings account details
    SavingsAccount(const std::string& number, const std::string& holder, double initialBalance, double rate)
        : Account(number, holder, initialBalance), interestRate(rate) {}

    // Override function to get the type of account
    std::string accountType() const override {
        return "Savings";
    }

    // Override function to display extra details specific to savings account
    void displayExtraDetails() const override {
        std::cout << "   Interest Rate: " << std::fixed << std::setprecision(2) << interestRate * 100 << "%" << std::endl;
    }

    // Override function to deposit money into the savings account
    void deposit(double amount) override {
        if (amount > 0) {
            balance += amount;
        } else {
            std::cout << "Invalid deposit amount." << std::endl;
        }
    }

    // Override function to withdraw money from the savings account
    void withdraw(double amount) override {
        if (amount > 0 && balance - amount >= 100) {
            balance -= amount;
        } else {
            std::cout << "Invalid withdrawal amount or minimum balance requirement not met." << std::endl;
        }
    }
};

// Derived class representing a current account
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    // Constructor to initialize current account details
    CurrentAccount(const std::string& number, const std::string& holder, double initialBalance, double limit)
        : Account(number, holder, initialBalance), overdraftLimit(limit) {}

    // Override function to get the type of account
    std::string accountType() const override {
        return "Current";
    }

    // Override function to display extra details specific to current account
    void displayExtraDetails() const override {
        std::cout << "   Overdraft Limit: $" << std::fixed << std::setprecision(2) << overdraftLimit << std::endl;
    }

    // Override function to deposit money into the current account
    void deposit(double amount) override {
        if (amount > 0) {
            balance += amount;
        } else {
            std::cout << "Invalid deposit amount." << std::endl;
        }
    }

    // Override function to withdraw money from the current account
    void withdraw(double amount) override {
        if (amount > 0 && balance + overdraftLimit >= amount) {
            balance -= amount;
        } else {
            std::cout << "Invalid withdrawal amount or overdraft limit exceeded." << std::endl;
        }
    }

    // Overloaded assignment operator for transferring from Account to CurrentAccount
    CurrentAccount& operator=(const Account& other) {
        if (this != &other) {
            if (const CurrentAccount* derived = dynamic_cast<const CurrentAccount*>(&other)) {
                Account::operator=(other);
                overdraftLimit = derived->overdraftLimit;
            } else {
                // Handle error or provide a default behavior
            }
        }
        return *this;
    }
};

// Main function to demonstrate the use of the Account, SavingsAccount, and CurrentAccount classes
int main() {
    // Create instances of SavingsAccount and CurrentAccount
    SavingsAccount savings("S123", "John Doe", 1000, 0.02);
    CurrentAccount current("C456", "Jane Doe", 2000, 500);

    // Display initial account details
    savings.displayDetails();
    current.displayDetails();

    // Perform deposit and withdrawal operations
    savings.deposit(500);
    current.withdraw(1000);

    // Display account details after deposit and withdrawal
    std::cout << "Account Details after deposit and withdrawal:" << std::endl;
    savings.displayDetails();
    current.displayDetails();

    // Transfer money from savings to current account
    current = current + savings;

    // Display account details after transfer
    std::cout << "Account Details after transfer:" << std::endl;
    savings.displayDetails();
    current.displayDetails();

    return 0;
}