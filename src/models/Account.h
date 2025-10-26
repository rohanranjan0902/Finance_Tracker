#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>
#include <memory>
#include <mutex>

class Transaction;

enum class AccountType {
    SAVINGS,
    CHECKING,
    CREDIT,
    INVESTMENT
};

class Account {
private:
    int account_id;
    int user_id;
    AccountType type;
    double balance;
    std::vector<std::shared_ptr<Transaction>> transaction_history;
    mutable std::mutex account_mutex;  // For thread safety

public:
    // Constructors
    Account();
    Account(int account_id, int user_id, AccountType type, double initial_balance = 0.0);
    
    // Destructor
    ~Account();
    
    // Getters
    int getAccountId() const;
    int getUserId() const;
    AccountType getType() const;
    double getBalance() const;
    std::string getTypeString() const;
    std::vector<std::shared_ptr<Transaction>> getTransactionHistory() const;
    
    // Setters
    void setBalance(double balance);
    
    // Transaction operations (thread-safe)
    bool deposit(double amount, const std::string& description = "");
    bool withdraw(double amount, const std::string& description = "");
    bool transfer(std::shared_ptr<Account> to_account, double amount, const std::string& description = "");
    
    // Transaction management
    void addTransaction(std::shared_ptr<Transaction> transaction);
    
    // Utility functions
    void displayAccountInfo() const;
    double calculateMonthlyAverage() const;
    bool hasInsufficientFunds(double amount) const;
    
    // Static utility functions
    static std::string accountTypeToString(AccountType type);
    static AccountType stringToAccountType(const std::string& type_str);
};

#endif // ACCOUNT_H
