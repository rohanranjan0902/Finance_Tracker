#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <iomanip>
#include <limits>
#include <map>
#include "models/User.h"
#include "models/Account.h"
#include "models/Transaction.h"
#include "models/Budget.h"
#include "services/TransactionService.h"
#include "exceptions.h"

// Global data structures (in production, these would be loaded from database)
std::map<std::string, std::shared_ptr<User>> users_by_email;
std::map<int, std::shared_ptr<Account>> accounts_by_id;
int next_user_id = 1;
int next_account_id = 1;

// Current session state
std::shared_ptr<User> logged_in_user = nullptr;
TransactionService transaction_service;

// Utility functions
void clearInputBuffer() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int readIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Invalid input. Please enter a number.\n";
        clearInputBuffer();
    }
}

double readDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= 0) {
            clearInputBuffer();
            return value;
        }
        std::cout << "Invalid input. Please enter a positive number.\n";
        clearInputBuffer();
    }
}

std::string readStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

std::string hashPassword(const std::string& password) {
    // Simple hash for demonstration - in production use bcrypt or similar
    return "hashed_" + password;
}

bool verifyPassword(const std::string& input, const std::string& stored_hash) {
    return hashPassword(input) == stored_hash;
}

// Display functions
void displayAccounts(const std::shared_ptr<User>& user) {
    auto accounts = user->getAccounts();
    if (accounts.empty()) {
        std::cout << "\nNo accounts found.\n";
        return;
    }
    
    std::cout << "\n=== Your Accounts ===\n";
    for (const auto& account : accounts) {
        std::cout << "ID: " << account->getAccountId()
                  << " | Type: " << std::setw(12) << std::left << account->getTypeString()
                  << " | Balance: $" << std::fixed << std::setprecision(2) << account->getBalance()
                  << "\n";
    }
    std::cout << "====================\n";
}

void displayTransactionHistory(const std::shared_ptr<Account>& account) {
    auto transactions = account->getTransactionHistory();
    if (transactions.empty()) {
        std::cout << "\nNo transactions found.\n";
        return;
    }
    
    std::cout << "\n=== Transaction History ===\n";
    int count = 0;
    for (auto it = transactions.rbegin(); it != transactions.rend() && count < 10; ++it, ++count) {
        auto tx = *it;
        std::cout << tx->getTimestampString() << " | "
                  << std::setw(12) << std::left << tx->getTypeString() << " | $"
                  << std::fixed << std::setprecision(2) << tx->getAmount() << " | "
                  << tx->getDescription() << "\n";
    }
    std::cout << "===========================\n";
}

// Action handlers
void handleCreateUser() {
    std::cout << "\n=== Create New User ===\n";
    
    std::string name = readStringInput("Enter your name: ");
    std::string email = readStringInput("Enter your email: ");
    
    if (users_by_email.find(email) != users_by_email.end()) {
        std::cout << "Error: A user with this email already exists.\n";
        return;
    }
    
    std::string password = readStringInput("Enter password: ");
    std::string confirm = readStringInput("Confirm password: ");
    
    if (password != confirm) {
        std::cout << "Error: Passwords do not match.\n";
        return;
    }
    
    try {
        auto user = std::make_shared<User>(next_user_id++, name, email, hashPassword(password));
        users_by_email[email] = user;
        std::cout << "✅ User created successfully! You can now log in.\n";
    } catch (const std::exception& e) {
        std::cout << "Error creating user: " << e.what() << "\n";
    }
}

void handleLogin() {
    std::cout << "\n=== Login ===\n";
    
    std::string email = readStringInput("Email: ");
    std::string password = readStringInput("Password: ");
    
    auto it = users_by_email.find(email);
    if (it == users_by_email.end()) {
        std::cout << "Error: User not found.\n";
        return;
    }
    
    if (!verifyPassword(password, it->second->getPasswordHash())) {
        std::cout << "Error: Incorrect password.\n";
        return;
    }
    
    logged_in_user = it->second;
    std::cout << "✅ Login successful! Welcome, " << logged_in_user->getName() << "!\n";
}

void handleCreateAccount() {
    std::cout << "\n=== Create New Account ===\n";
    std::cout << "1. Savings\n2. Checking\n3. Credit\n4. Investment\n";
    
    int type_choice = readIntInput("Select account type (1-4): ");
    AccountType type;
    
    switch (type_choice) {
        case 1: type = AccountType::SAVINGS; break;
        case 2: type = AccountType::CHECKING; break;
        case 3: type = AccountType::CREDIT; break;
        case 4: type = AccountType::INVESTMENT; break;
        default:
            std::cout << "Invalid selection.\n";
            return;
    }
    
    double initial_balance = readDoubleInput("Enter initial balance: $");
    
    try {
        auto account = std::make_shared<Account>(next_account_id++, logged_in_user->getUserId(), type, initial_balance);
        logged_in_user->addAccount(account);
        accounts_by_id[account->getAccountId()] = account;
        std::cout << "✅ Account created successfully! Account ID: " << account->getAccountId() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error creating account: " << e.what() << "\n";
    }
}

void handleDeposit() {
    displayAccounts(logged_in_user);
    int account_id = readIntInput("\nEnter account ID: ");
    
    auto account = logged_in_user->getAccount(account_id);
    if (!account) {
        std::cout << "Error: Account not found.\n";
        return;
    }
    
    double amount = readDoubleInput("Enter deposit amount: $");
    std::string description = readStringInput("Description (optional): ");
    
    try {
        transaction_service.processDeposit(account, amount, description);
        std::cout << "✅ Deposit successful! New balance: $" << std::fixed << std::setprecision(2) << account->getBalance() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void handleWithdrawal() {
    displayAccounts(logged_in_user);
    int account_id = readIntInput("\nEnter account ID: ");
    
    auto account = logged_in_user->getAccount(account_id);
    if (!account) {
        std::cout << "Error: Account not found.\n";
        return;
    }
    
    double amount = readDoubleInput("Enter withdrawal amount: $");
    std::string description = readStringInput("Description (optional): ");
    
    try {
        transaction_service.processWithdrawal(account, amount, description);
        std::cout << "✅ Withdrawal successful! New balance: $" << std::fixed << std::setprecision(2) << account->getBalance() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void handleTransfer() {
    displayAccounts(logged_in_user);
    int from_id = readIntInput("\nEnter source account ID: ");
    
    auto from_account = logged_in_user->getAccount(from_id);
    if (!from_account) {
        std::cout << "Error: Source account not found.\n";
        return;
    }
    
    int to_id = readIntInput("Enter destination account ID: ");
    auto to_account = accounts_by_id.find(to_id);
    
    if (to_account == accounts_by_id.end()) {
        std::cout << "Error: Destination account not found.\n";
        return;
    }
    
    double amount = readDoubleInput("Enter transfer amount: $");
    std::string description = readStringInput("Description (optional): ");
    
    try {
        transaction_service.processTransfer(from_account, to_account->second, amount, description);
        std::cout << "✅ Transfer successful!\n";
        std::cout << "From account balance: $" << std::fixed << std::setprecision(2) << from_account->getBalance() << "\n";
        std::cout << "To account balance: $" << std::fixed << std::setprecision(2) << to_account->second->getBalance() << "\n";
    } catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << "\n";
    }
}

void handleViewTransactions() {
    displayAccounts(logged_in_user);
    int account_id = readIntInput("\nEnter account ID: ");
    
    auto account = logged_in_user->getAccount(account_id);
    if (!account) {
        std::cout << "Error: Account not found.\n";
        return;
    }
    
    displayTransactionHistory(account);
}

// Menu displays
void displayLoggedOutMenu() {
    std::cout << "\n╔════════════════════════════════════╗\n";
    std::cout << "║         FinTrack - Main Menu       ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "1. Create User\n";
    std::cout << "2. Login\n";
    std::cout << "0. Exit\n";
    std::cout << "──────────────────────────────────────\n";
}

void displayLoggedInMenu() {
    std::cout << "\n╔════════════════════════════════════╗\n";
    std::cout << "║      Welcome, " << std::setw(20) << std::left << logged_in_user->getName() << " ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "1. View Accounts\n";
    std::cout << "2. Create Account\n";
    std::cout << "3. Deposit\n";
    std::cout << "4. Withdraw\n";
    std::cout << "5. Transfer\n";
    std::cout << "6. View Transaction History\n";
    std::cout << "7. Logout\n";
    std::cout << "0. Exit\n";
    std::cout << "──────────────────────────────────────\n";
}

// Main application loop
int main() {
    std::cout << "╔════════════════════════════════════╗\n";
    std::cout << "║  🏦 FinTrack - Personal Finance   ║\n";
    std::cout << "║      Management System             ║\n";
    std::cout << "╚════════════════════════════════════╝\n";
    std::cout << "\nWelcome to FinTrack!\n";
    
    // Main application loop
    while (true) {
        try {
            if (!logged_in_user) {
                displayLoggedOutMenu();
                int choice = readIntInput("Select option: ");
                
                switch (choice) {
                    case 1:
                        handleCreateUser();
                        break;
                    case 2:
                        handleLogin();
                        break;
                    case 0:
                        std::cout << "\nThank you for using FinTrack. Goodbye!\n";
                        return 0;
                    default:
                        std::cout << "Invalid option. Please try again.\n";
                }
            } else {
                displayLoggedInMenu();
                int choice = readIntInput("Select option: ");
                
                switch (choice) {
                    case 1:
                        displayAccounts(logged_in_user);
                        break;
                    case 2:
                        handleCreateAccount();
                        break;
                    case 3:
                        handleDeposit();
                        break;
                    case 4:
                        handleWithdrawal();
                        break;
                    case 5:
                        handleTransfer();
                        break;
                    case 6:
                        handleViewTransactions();
                        break;
                    case 7:
                        std::cout << "Logging out...\n";
                        logged_in_user = nullptr;
                        break;
                    case 0:
                        std::cout << "\nThank you for using FinTrack. Goodbye!\n";
                        return 0;
                    default:
                        std::cout << "Invalid option. Please try again.\n";
                }
            }
        } catch (const std::exception& e) {
            std::cout << "An error occurred: " << e.what() << "\n";
            std::cout << "Returning to main menu...\n";
        }
    }
    
    return 0;
}

