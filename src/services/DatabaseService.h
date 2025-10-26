#ifndef DATABASE_SERVICE_H
#define DATABASE_SERVICE_H

#include <string>
#include <vector>
#include <memory>
#include "../exceptions.h"

class User;
class Account;
class Transaction;
class Budget;
enum class AccountType;
enum class TransactionType;
enum class TransactionCategory;
enum class TransactionStatus;

class DatabaseService {
private:
    void* db; // SQLite database handle (void* to avoid including sqlite3.h)
    std::string db_path;
    
    void executeSQL(const std::string& sql);
    void createTables();

public:
    explicit DatabaseService(const std::string& database_path = "fintrack.db");
    ~DatabaseService();
    
    // User operations
    void saveUser(const User& user);
    std::shared_ptr<User> loadUserByEmail(const std::string& email);
    std::shared_ptr<User> loadUserById(int user_id);
    std::vector<std::shared_ptr<User>> loadAllUsers();
    bool userExists(const std::string& email);
    
    // Account operations
    void saveAccount(const Account& account);
    void updateAccountBalance(int account_id, double new_balance);
    std::shared_ptr<Account> loadAccountById(int account_id);
    std::vector<std::shared_ptr<Account>> loadAccountsForUser(int user_id);
    
    // Transaction operations
    void saveTransaction(const Transaction& transaction);
    std::vector<std::shared_ptr<Transaction>> loadTransactionsForAccount(int account_id);
    std::vector<std::shared_ptr<Transaction>> loadAllTransactions();
    
    // Budget operations
    void saveBudget(const Budget& budget);
    void updateBudgetSpent(int budget_id, double current_spent);
    std::vector<std::shared_ptr<Budget>> loadBudgetsForUser(int user_id);
    
    // Utility
    int getNextUserId();
    int getNextAccountId();
    int getNextTransactionId();
    int getNextBudgetId();
};

#endif // DATABASE_SERVICE_H

