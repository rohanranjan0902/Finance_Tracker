#ifndef TRANSACTION_SERVICE_H
#define TRANSACTION_SERVICE_H

#include <vector>
#include <memory>
#include <mutex>
#include <string>
#include <future>
#include "../models/Transaction.h"

class Account;

struct TransactionRequest {
    std::shared_ptr<Account> account;
    std::shared_ptr<Account> to_account;  // For transfers
    double amount;
    TransactionType type;
    std::string description;
    std::string location;
    
    TransactionRequest(std::shared_ptr<Account> acc, double amt, TransactionType t, 
                      const std::string& desc = "", const std::string& loc = "")
        : account(acc), to_account(nullptr), amount(amt), type(t), description(desc), location(loc) {}
        
    TransactionRequest(std::shared_ptr<Account> from_acc, std::shared_ptr<Account> to_acc, 
                      double amt, const std::string& desc = "")
        : account(from_acc), to_account(to_acc), amount(amt), type(TransactionType::TRANSFER_OUT), 
          description(desc), location("") {}
};

class TransactionService {
private:
    std::vector<std::shared_ptr<Transaction>> pending_transactions;
    std::vector<std::shared_ptr<Transaction>> completed_transactions;
    std::mutex service_mutex;
    int next_transaction_id;

public:
    // Constructor and Destructor
    TransactionService();
    ~TransactionService();
    
    // Transaction processing
    bool processDeposit(std::shared_ptr<Account> account, double amount, 
                       const std::string& description = "", const std::string& location = "");
    bool processWithdrawal(std::shared_ptr<Account> account, double amount, 
                          const std::string& description = "", const std::string& location = "");
    bool processTransfer(std::shared_ptr<Account> from_account, std::shared_ptr<Account> to_account, 
                        double amount, const std::string& description = "");
    
    // Batch processing
    void processTransactionsBatch(const std::vector<TransactionRequest>& requests);
    
    // Query operations
    std::vector<std::shared_ptr<Transaction>> getTransactionHistory(int account_id);
    std::vector<std::shared_ptr<Transaction>> getPendingTransactions();
    std::vector<std::shared_ptr<Transaction>> getSuspiciousTransactions();
    
    // Analytics
    double calculateDailyVolume(int account_id);
    void displayTransactionSummary();
    
    // Utility
    int getNextTransactionId();
};

#endif // TRANSACTION_SERVICE_H
