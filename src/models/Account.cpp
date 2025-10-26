#include "Account.h"
#include "Transaction.h"
#include "../exceptions.h"
#include <algorithm>
#include <numeric>

Account::Account() : account_id(0), user_id(0), type(AccountType::CHECKING), balance(0.0) {}

Account::Account(int account_id, int user_id, AccountType type, double initial_balance)
    : account_id(account_id), user_id(user_id), type(type), balance(initial_balance) {}

Account::~Account() {
    transaction_history.clear();
}
int Account::getAccountId() const {
    return account_id;
}

int Account::getUserId() const {
    return user_id;
}

AccountType Account::getType() const {
    return type;
}

double Account::getBalance() const {
    std::lock_guard<std::mutex> lock(account_mutex);
    return balance;
}

std::string Account::getTypeString() const {
    return accountTypeToString(type);
}

std::vector<std::shared_ptr<Transaction>> Account::getTransactionHistory() const {
    std::lock_guard<std::mutex> lock(account_mutex);
    return transaction_history;
}

// Setters
void Account::setBalance(double balance) {
    std::lock_guard<std::mutex> lock(account_mutex);
    this->balance = balance;
}

bool Account::deposit(double amount, const std::string& description) {
    if (amount <= 0) {
        throw InvalidTransactionException("Deposit amount must be positive");
    }

    std::lock_guard<std::mutex> lock(account_mutex);
    balance += amount;
    
    auto transaction = std::make_shared<Transaction>(
        static_cast<int>(transaction_history.size() + 1),
        account_id,
        amount,
        TransactionType::DEPOSIT,
        TransactionCategory::OTHER,
        description.empty() ? "Deposit" : description
    );
    
    transaction->setStatus(TransactionStatus::COMPLETED);
    transaction_history.push_back(transaction);
    
    return true;
}

bool Account::withdraw(double amount, const std::string& description) {
    if (amount <= 0) {
        throw InvalidTransactionException("Withdrawal amount must be positive");
    }

    std::lock_guard<std::mutex> lock(account_mutex);
    
    if (type != AccountType::CREDIT && balance < amount) {
        throw InsufficientFundsException("Insufficient funds for withdrawal");
    }

    balance -= amount;
    
    auto transaction = std::make_shared<Transaction>(
        static_cast<int>(transaction_history.size() + 1),
        account_id,
        amount,
        TransactionType::WITHDRAWAL,
        TransactionCategory::OTHER,
        description.empty() ? "Withdrawal" : description
    );
    
    transaction->setStatus(TransactionStatus::COMPLETED);
    transaction_history.push_back(transaction);
    
    return true;
}

bool Account::transfer(std::shared_ptr<Account> to_account, double amount, const std::string& description) {
    if (!to_account) {
        throw InvalidAccountException("Invalid destination account");
    }
    
    if (amount <= 0) {
        throw InvalidTransactionException("Transfer amount must be positive");
    }
    
    if (account_id == to_account->getAccountId()) {
        throw InvalidTransactionException("Cannot transfer to the same account");
    }

    // Lock both accounts in consistent order to prevent deadlock
    std::mutex* first_mutex = (account_id < to_account->getAccountId()) ? &account_mutex : &(to_account->account_mutex);
    std::mutex* second_mutex = (account_id < to_account->getAccountId()) ? &(to_account->account_mutex) : &account_mutex;
    
    std::lock_guard<std::mutex> lock1(*first_mutex);
    std::lock_guard<std::mutex> lock2(*second_mutex);
    
    if (type != AccountType::CREDIT && balance < amount) {
        throw InsufficientFundsException("Insufficient funds for transfer");
    }

    balance -= amount;
    to_account->balance += amount;
    
    std::string transfer_desc = description.empty() ? "Transfer" : description;
    auto out_transaction = std::make_shared<Transaction>(
        static_cast<int>(transaction_history.size() + 1),
        account_id,
        amount,
        TransactionType::TRANSFER_OUT,
        TransactionCategory::OTHER,
        transfer_desc + " to Account " + std::to_string(to_account->getAccountId())
    );
    out_transaction->setToAccountId(to_account->getAccountId());
    out_transaction->setStatus(TransactionStatus::COMPLETED);
    transaction_history.push_back(out_transaction);
    
    // Incoming transaction
    auto in_transaction = std::make_shared<Transaction>(
        static_cast<int>(to_account->transaction_history.size() + 1),
        to_account->getAccountId(),
        amount,
        TransactionType::TRANSFER_IN,
        TransactionCategory::OTHER,
        transfer_desc + " from Account " + std::to_string(account_id)
    );
    in_transaction->setToAccountId(account_id);
    in_transaction->setStatus(TransactionStatus::COMPLETED);
    to_account->transaction_history.push_back(in_transaction);
    
    return true;
}

void Account::addTransaction(std::shared_ptr<Transaction> transaction) {
    if (transaction && transaction->getAccountId() == account_id) {
        std::lock_guard<std::mutex> lock(account_mutex);
        transaction_history.push_back(transaction);
    }
}

void Account::displayAccountInfo() const {
    // This method is intentionally left for backwards compatibility
    // but should not be used in production. UI layer should handle display.
}

double Account::calculateMonthlyAverage() const {
    std::lock_guard<std::mutex> lock(account_mutex);
    
    if (transaction_history.empty()) return 0.0;
    
    double total = std::accumulate(transaction_history.begin(), transaction_history.end(), 0.0,
        [](double sum, const std::shared_ptr<Transaction>& tx) {
            return sum + tx->getAmount();
        });
    
    return total / transaction_history.size();
}

bool Account::hasInsufficientFunds(double amount) const {
    std::lock_guard<std::mutex> lock(account_mutex);
    return (type != AccountType::CREDIT) && (balance < amount);
}

std::string Account::accountTypeToString(AccountType type) {
    switch (type) {
        case AccountType::SAVINGS: return "Savings";
        case AccountType::CHECKING: return "Checking";
        case AccountType::CREDIT: return "Credit";
        case AccountType::INVESTMENT: return "Investment";
        default: return "Unknown";
    }
}

AccountType Account::stringToAccountType(const std::string& type_str) {
    if (type_str == "Savings") return AccountType::SAVINGS;
    if (type_str == "Checking") return AccountType::CHECKING;
    if (type_str == "Credit") return AccountType::CREDIT;
    if (type_str == "Investment") return AccountType::INVESTMENT;
    return AccountType::CHECKING;
}
