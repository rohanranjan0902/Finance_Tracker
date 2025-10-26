#ifndef FRAUD_DETECTION_SERVICE_H
#define FRAUD_DETECTION_SERVICE_H

#include <vector>
#include <memory>
#include <map>
#include <thread>
#include <chrono>
#include <mutex>
#include "../models/Transaction.h"

class Account;
class TransactionService;

struct FraudRule {
    std::string rule_name;
    double threshold_value;
    bool enabled;
    
    FraudRule(const std::string& name, double threshold, bool is_enabled = true)
        : rule_name(name), threshold_value(threshold), enabled(is_enabled) {}
};

struct AccountProfile {
    int account_id;
    double average_transaction_amount;
    double max_transaction_amount;
    std::vector<std::string> common_locations;
    std::chrono::hours typical_transaction_hours[24];
    int daily_transaction_count;
    
    AccountProfile(int id) : account_id(id), average_transaction_amount(0.0), 
                           max_transaction_amount(0.0), daily_transaction_count(0) {}
};

class FraudDetectionService {
private:
    std::vector<FraudRule> fraud_rules;
    std::map<int, AccountProfile> account_profiles; // account_id -> profile
    std::vector<std::shared_ptr<Transaction>> flagged_transactions;
    std::mutex service_mutex;
    std::thread background_thread;
    bool running;
    
    // Fraud detection algorithms
    bool checkHighValueTransaction(std::shared_ptr<Transaction> transaction);
    bool checkUnusualLocation(std::shared_ptr<Transaction> transaction);
    bool checkRapidTransactions(std::shared_ptr<Transaction> transaction);
    bool checkUnusualTime(std::shared_ptr<Transaction> transaction);
    bool checkVelocityPattern(std::shared_ptr<Transaction> transaction);
    
    // Profile management
    void updateAccountProfile(std::shared_ptr<Transaction> transaction);
    AccountProfile* getAccountProfile(int account_id);
    
    // Background processing
    void backgroundFraudDetection();

public:
    // Constructor and Destructor
    FraudDetectionService();
    ~FraudDetectionService();
    
    // Service control
    void startService();
    void stopService();
    
    // Rule management
    void addFraudRule(const FraudRule& rule);
    void removeFraudRule(const std::string& rule_name);
    void updateFraudRule(const std::string& rule_name, double new_threshold);
    std::vector<FraudRule> getFraudRules() const;
    
    // Fraud detection
    bool analyzeTransaction(std::shared_ptr<Transaction> transaction);
    void analyzeTransactionBatch(const std::vector<std::shared_ptr<Transaction>>& transactions);
    
    // Query operations
    std::vector<std::shared_ptr<Transaction>> getFlaggedTransactions() const;
    std::vector<std::shared_ptr<Transaction>> getFlaggedTransactionsByAccount(int account_id) const;
    
    // Analytics and reporting
    void generateFraudReport() const;
    double getFraudRate() const; // Percentage of transactions flagged
    void displayFraudStatistics() const;
    
    // Account profiling
    void buildAccountProfile(int account_id, const std::vector<std::shared_ptr<Transaction>>& history);
    void updateAllProfiles(TransactionService* transaction_service);
    
    // Manual review
    void markTransactionAsLegitimate(int transaction_id);
    void markTransactionAsFraud(int transaction_id);
    
    // Alert system
    void sendFraudAlert(std::shared_ptr<Transaction> transaction) const;
    
private:
    // Helper functions
    double calculateTransactionAnomaly(std::shared_ptr<Transaction> transaction, const AccountProfile& profile);
    bool isWithinBusinessHours(std::chrono::system_clock::time_point timestamp);
    double calculateLocationRisk(const std::string& location, const AccountProfile& profile);
};

#endif // FRAUD_DETECTION_SERVICE_H
