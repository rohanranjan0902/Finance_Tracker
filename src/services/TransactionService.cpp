#include "TransactionService.h"
#include "../models/Transaction.h"
#include "../models/Account.h"
#include <iostream>
#include <thread>
#include <future>
#include <algorithm>

TransactionService::TransactionService() : next_transaction_id(1) {}

TransactionService::~TransactionService() {}

bool TransactionService::processDeposit(std::shared_ptr<Account> account, double amount, 
                                       const std::string& description, const std::string& location) {
    if (!account) {
        std::cerr << "Error: Invalid account for deposit" << std::endl;
        return false;
    }

    // Create transaction
    auto transaction = std::make_shared<Transaction>(
        getNextTransactionId(),
        account->getAccountId(),
        amount,
        TransactionType::DEPOSIT,
        TransactionCategory::OTHER,
        description
    );
    
    transaction->setLocation(location);
    transaction->setStatus(TransactionStatus::PENDING);
    
    // Add to pending transactions
    {
        std::lock_guard<std::mutex> lock(service_mutex);
        pending_transactions.push_back(transaction);
    }
    
    // Process the deposit
    bool success = account->deposit(amount, description);
    
    // Update transaction status
    transaction->setStatus(success ? TransactionStatus::COMPLETED : TransactionStatus::FAILED);
    
    if (success) {
        std::lock_guard<std::mutex> lock(service_mutex);
        completed_transactions.push_back(transaction);
        // Remove from pending
        pending_transactions.erase(
            std::remove(pending_transactions.begin(), pending_transactions.end(), transaction),
            pending_transactions.end()
        );
    }
    
    return success;
}

bool TransactionService::processWithdrawal(std::shared_ptr<Account> account, double amount, 
                                         const std::string& description, const std::string& location) {
    if (!account) {
        std::cerr << "Error: Invalid account for withdrawal" << std::endl;
        return false;
    }

    // Create transaction
    auto transaction = std::make_shared<Transaction>(
        getNextTransactionId(),
        account->getAccountId(),
        amount,
        TransactionType::WITHDRAWAL,
        TransactionCategory::OTHER,
        description
    );
    
    transaction->setLocation(location);
    transaction->setStatus(TransactionStatus::PENDING);
    
    // Add to pending transactions
    {
        std::lock_guard<std::mutex> lock(service_mutex);
        pending_transactions.push_back(transaction);
    }
    
    // Process the withdrawal
    bool success = account->withdraw(amount, description);
    
    // Update transaction status
    transaction->setStatus(success ? TransactionStatus::COMPLETED : TransactionStatus::FAILED);
    
    if (success) {
        std::lock_guard<std::mutex> lock(service_mutex);
        completed_transactions.push_back(transaction);
        // Remove from pending
        pending_transactions.erase(
            std::remove(pending_transactions.begin(), pending_transactions.end(), transaction),
            pending_transactions.end()
        );
    }
    
    return success;
}

bool TransactionService::processTransfer(std::shared_ptr<Account> from_account, 
                                       std::shared_ptr<Account> to_account, 
                                       double amount, const std::string& description) {
    if (!from_account || !to_account) {
        std::cerr << "Error: Invalid accounts for transfer" << std::endl;
        return false;
    }

    // Create transaction
    auto transaction = std::make_shared<Transaction>(
        getNextTransactionId(),
        from_account->getAccountId(),
        amount,
        TransactionType::TRANSFER_OUT,
        TransactionCategory::OTHER,
        description
    );
    
    transaction->setToAccountId(to_account->getAccountId());
    transaction->setStatus(TransactionStatus::PENDING);
    
    // Add to pending transactions
    {
        std::lock_guard<std::mutex> lock(service_mutex);
        pending_transactions.push_back(transaction);
    }
    
    // Process the transfer
    bool success = from_account->transfer(to_account, amount, description);
    
    // Update transaction status
    transaction->setStatus(success ? TransactionStatus::COMPLETED : TransactionStatus::FAILED);
    
    if (success) {
        std::lock_guard<std::mutex> lock(service_mutex);
        completed_transactions.push_back(transaction);
        // Remove from pending
        pending_transactions.erase(
            std::remove(pending_transactions.begin(), pending_transactions.end(), transaction),
            pending_transactions.end()
        );
    }
    
    return success;
}

std::vector<std::shared_ptr<Transaction>> TransactionService::getTransactionHistory(int account_id) {
    std::lock_guard<std::mutex> lock(service_mutex);
    std::vector<std::shared_ptr<Transaction>> account_transactions;
    
    for (const auto& transaction : completed_transactions) {
        if (transaction->getAccountId() == account_id || 
            transaction->getToAccountId() == account_id) {
            account_transactions.push_back(transaction);
        }
    }
    
    return account_transactions;
}

std::vector<std::shared_ptr<Transaction>> TransactionService::getPendingTransactions() {
    std::lock_guard<std::mutex> lock(service_mutex);
    return pending_transactions;
}

std::vector<std::shared_ptr<Transaction>> TransactionService::getSuspiciousTransactions() {
    std::lock_guard<std::mutex> lock(service_mutex);
    std::vector<std::shared_ptr<Transaction>> suspicious;
    
    for (const auto& transaction : completed_transactions) {
        if (transaction->isSuspicious()) {
            suspicious.push_back(transaction);
        }
    }
    
    return suspicious;
}

void TransactionService::processTransactionsBatch(const std::vector<TransactionRequest>& requests) {
    std::vector<std::future<bool>> futures;
    
    for (const auto& request : requests) {
        auto future = std::async(std::launch::async, [this, request]() {
            switch (request.type) {
                case TransactionType::DEPOSIT:
                    return processDeposit(request.account, request.amount, 
                                        request.description, request.location);
                case TransactionType::WITHDRAWAL:
                    return processWithdrawal(request.account, request.amount, 
                                           request.description, request.location);
                case TransactionType::TRANSFER_OUT:
                    return processTransfer(request.account, request.to_account, 
                                         request.amount, request.description);
                default:
                    return false;
            }
        });
        
        futures.push_back(std::move(future));
    }
    
    // Wait for all transactions to complete
    for (auto& future : futures) {
        future.get();
    }
    
    std::cout << "Batch processing completed for " << requests.size() << " transactions" << std::endl;
}

double TransactionService::calculateDailyVolume(int account_id) {
    std::lock_guard<std::mutex> lock(service_mutex);
    double volume = 0.0;
    auto now = std::chrono::system_clock::now();
    auto today = std::chrono::time_point_cast<std::chrono::days>(now);
    
    for (const auto& transaction : completed_transactions) {
        if (transaction->getAccountId() == account_id) {
            auto tx_time = std::chrono::time_point_cast<std::chrono::days>(transaction->getTimestamp());
            if (tx_time == today) {
                volume += transaction->getAmount();
            }
        }
    }
    
    return volume;
}

void TransactionService::displayTransactionSummary() {
    std::lock_guard<std::mutex> lock(service_mutex);
    
    std::cout << "\n=== Transaction Service Summary ===" << std::endl;
    std::cout << "Total Completed Transactions: " << completed_transactions.size() << std::endl;
    std::cout << "Pending Transactions: " << pending_transactions.size() << std::endl;
    
    int suspicious_count = 0;
    for (const auto& transaction : completed_transactions) {
        if (transaction->isSuspicious()) {
            suspicious_count++;
        }
    }
    std::cout << "Suspicious Transactions: " << suspicious_count << std::endl;
    
    double total_volume = 0.0;
    for (const auto& transaction : completed_transactions) {
        total_volume += transaction->getAmount();
    }
    std::cout << "Total Transaction Volume: $" << std::fixed << std::setprecision(2) 
              << total_volume << std::endl;
    
    std::cout << "===================================" << std::endl;
}

int TransactionService::getNextTransactionId() {
    std::lock_guard<std::mutex> lock(service_mutex);
    return next_transaction_id++;
}
