#include "FraudDetectionService.h"
#include "TransactionService.h"
#include <iostream>
#include <algorithm>
#include <random>

FraudDetectionService::FraudDetectionService() : running(false) {
    // Initialize default fraud rules
    addFraudRule(FraudRule("High Value Transaction", 5000.0));
    addFraudRule(FraudRule("Rapid Transactions", 10.0));
    addFraudRule(FraudRule("Unusual Location", 1.0));
}

FraudDetectionService::~FraudDetectionService() {
    stopService();
}

void FraudDetectionService::startService() {
    if (!running) {
        running = true;
        background_thread = std::thread(&FraudDetectionService::backgroundFraudDetection, this);
        std::cout << "🔍 Fraud Detection Service started" << std::endl;
    }
}

void FraudDetectionService::stopService() {
    if (running) {
        running = false;
        if (background_thread.joinable()) {
            background_thread.join();
        }
        std::cout << "🔍 Fraud Detection Service stopped" << std::endl;
    }
}

void FraudDetectionService::addFraudRule(const FraudRule& rule) {
    std::lock_guard<std::mutex> lock(service_mutex);
    fraud_rules.push_back(rule);
    std::cout << "Added fraud rule: " << rule.rule_name 
              << " (threshold: " << rule.threshold_value << ")" << std::endl;
}

void FraudDetectionService::removeFraudRule(const std::string& rule_name) {
    std::lock_guard<std::mutex> lock(service_mutex);
    auto it = std::find_if(fraud_rules.begin(), fraud_rules.end(),
        [&rule_name](const FraudRule& rule) {
            return rule.rule_name == rule_name;
        });
    
    if (it != fraud_rules.end()) {
        fraud_rules.erase(it);
        std::cout << "Removed fraud rule: " << rule_name << std::endl;
    }
}

void FraudDetectionService::updateFraudRule(const std::string& rule_name, double new_threshold) {
    std::lock_guard<std::mutex> lock(service_mutex);
    auto it = std::find_if(fraud_rules.begin(), fraud_rules.end(),
        [&rule_name](FraudRule& rule) {
            return rule.rule_name == rule_name;
        });
    
    if (it != fraud_rules.end()) {
        it->threshold_value = new_threshold;
        std::cout << "Updated fraud rule: " << rule_name 
                  << " (new threshold: " << new_threshold << ")" << std::endl;
    }
}

std::vector<FraudRule> FraudDetectionService::getFraudRules() const {
    std::lock_guard<std::mutex> lock(service_mutex);
    return fraud_rules;
}

bool FraudDetectionService::analyzeTransaction(std::shared_ptr<Transaction> transaction) {
    if (!transaction) return false;
    
    bool is_suspicious = false;
    std::vector<std::string> triggered_rules;
    
    // Check all fraud detection rules
    if (checkHighValueTransaction(transaction)) {
        triggered_rules.push_back("High Value");
        is_suspicious = true;
    }
    
    if (checkUnusualLocation(transaction)) {
        triggered_rules.push_back("Unusual Location");
        is_suspicious = true;
    }
    
    if (checkRapidTransactions(transaction)) {
        triggered_rules.push_back("Rapid Transactions");
        is_suspicious = true;
    }
    
    if (checkUnusualTime(transaction)) {
        triggered_rules.push_back("Unusual Time");
        is_suspicious = true;
    }
    
    // Mark transaction as suspicious if any rules triggered
    if (is_suspicious) {
        transaction->setSuspiciousFlag(true);
        
        std::lock_guard<std::mutex> lock(service_mutex);
        flagged_transactions.push_back(transaction);
        
        // Send alert
        sendFraudAlert(transaction);
        
        std::cout << "🚨 FRAUD ALERT: Transaction " << transaction->getTransactionId()
                  << " flagged for: ";
        for (size_t i = 0; i < triggered_rules.size(); ++i) {
            std::cout << triggered_rules[i];
            if (i < triggered_rules.size() - 1) std::cout << ", ";
        }
        std::cout << std::endl;
    }
    
    // Update account profile
    updateAccountProfile(transaction);
    
    return is_suspicious;
}

void FraudDetectionService::analyzeTransactionBatch(const std::vector<std::shared_ptr<Transaction>>& transactions) {
    for (const auto& transaction : transactions) {
        analyzeTransaction(transaction);
    }
    std::cout << "Analyzed batch of " << transactions.size() << " transactions" << std::endl;
}

std::vector<std::shared_ptr<Transaction>> FraudDetectionService::getFlaggedTransactions() const {
    std::lock_guard<std::mutex> lock(service_mutex);
    return flagged_transactions;
}

std::vector<std::shared_ptr<Transaction>> FraudDetectionService::getFlaggedTransactionsByAccount(int account_id) const {
    std::lock_guard<std::mutex> lock(service_mutex);
    std::vector<std::shared_ptr<Transaction>> account_flagged;
    
    for (const auto& transaction : flagged_transactions) {
        if (transaction->getAccountId() == account_id) {
            account_flagged.push_back(transaction);
        }
    }
    
    return account_flagged;
}

void FraudDetectionService::generateFraudReport() const {
    std::lock_guard<std::mutex> lock(service_mutex);
    
    std::cout << "\n=== FRAUD DETECTION REPORT ===" << std::endl;
    std::cout << "Total Flagged Transactions: " << flagged_transactions.size() << std::endl;
    
    if (!flagged_transactions.empty()) {
        std::cout << "\nSuspicious Transactions:" << std::endl;
        for (const auto& transaction : flagged_transactions) {
            std::cout << "  - TX ID: " << transaction->getTransactionId()
                      << ", Amount: $" << transaction->getAmount()
                      << ", Account: " << transaction->getAccountId()
                      << ", Location: " << transaction->getLocation()
                      << ", Time: " << transaction->getTimestampString() << std::endl;
        }
    }
    
    std::cout << "\nActive Fraud Rules:" << std::endl;
    for (const auto& rule : fraud_rules) {
        std::cout << "  - " << rule.rule_name 
                  << " (Threshold: " << rule.threshold_value 
                  << ", Enabled: " << (rule.enabled ? "Yes" : "No") << ")" << std::endl;
    }
    
    std::cout << "==============================" << std::endl;
}

double FraudDetectionService::getFraudRate() const {
    // This would need integration with TransactionService to get total transaction count
    // For now, return a simplified calculation
    std::lock_guard<std::mutex> lock(service_mutex);
    if (flagged_transactions.empty()) return 0.0;
    
    // Assume 100 total transactions for demo purposes
    return (static_cast<double>(flagged_transactions.size()) / 100.0) * 100.0;
}

void FraudDetectionService::displayFraudStatistics() const {
    std::lock_guard<std::mutex> lock(service_mutex);
    
    std::cout << "\n=== FRAUD STATISTICS ===" << std::endl;
    std::cout << "Total Suspicious Transactions: " << flagged_transactions.size() << std::endl;
    std::cout << "Fraud Rate: " << std::fixed << std::setprecision(2) << getFraudRate() << "%" << std::endl;
    std::cout << "Active Rules: " << fraud_rules.size() << std::endl;
    
    // Count by rule type (simplified)
    int high_value_count = 0, location_count = 0, rapid_count = 0;
    for (const auto& tx : flagged_transactions) {
        if (tx->getAmount() > 2000) high_value_count++;
        if (tx->getLocation() != "New York") location_count++;
        // Add more sophisticated counting logic here
    }
    
    std::cout << "High Value Alerts: " << high_value_count << std::endl;
    std::cout << "Location Alerts: " << location_count << std::endl;
    std::cout << "Rapid Transaction Alerts: " << rapid_count << std::endl;
    std::cout << "========================" << std::endl;
}

void FraudDetectionService::buildAccountProfile(int account_id, const std::vector<std::shared_ptr<Transaction>>& history) {
    AccountProfile profile(account_id);
    
    if (history.empty()) {
        account_profiles[account_id] = profile;
        return;
    }
    
    // Calculate average transaction amount
    double total_amount = 0.0;
    double max_amount = 0.0;
    
    for (const auto& tx : history) {
        total_amount += tx->getAmount();
        if (tx->getAmount() > max_amount) {
            max_amount = tx->getAmount();
        }
        
        // Track common locations
        std::string location = tx->getLocation();
        if (!location.empty()) {
            auto it = std::find(profile.common_locations.begin(), profile.common_locations.end(), location);
            if (it == profile.common_locations.end()) {
                profile.common_locations.push_back(location);
            }
        }
    }
    
    profile.average_transaction_amount = total_amount / history.size();
    profile.max_transaction_amount = max_amount;
    profile.daily_transaction_count = static_cast<int>(history.size());
    
    account_profiles[account_id] = profile;
    
    std::cout << "Built profile for account " << account_id 
              << " (avg: $" << profile.average_transaction_amount 
              << ", max: $" << profile.max_transaction_amount << ")" << std::endl;
}

void FraudDetectionService::updateAllProfiles(TransactionService* transaction_service) {
    // This would integrate with TransactionService to update all profiles
    std::cout << "Profile update requested (integration with TransactionService needed)" << std::endl;
}

void FraudDetectionService::markTransactionAsLegitimate(int transaction_id) {
    std::lock_guard<std::mutex> lock(service_mutex);
    
    auto it = std::find_if(flagged_transactions.begin(), flagged_transactions.end(),
        [transaction_id](const std::shared_ptr<Transaction>& tx) {
            return tx->getTransactionId() == transaction_id;
        });
    
    if (it != flagged_transactions.end()) {
        (*it)->setSuspiciousFlag(false);
        flagged_transactions.erase(it);
        std::cout << "Transaction " << transaction_id << " marked as legitimate" << std::endl;
    }
}

void FraudDetectionService::markTransactionAsFraud(int transaction_id) {
    std::cout << "Transaction " << transaction_id << " confirmed as fraud - taking appropriate action" << std::endl;
    // In a real system, this would trigger account freezing, notifications, etc.
}

void FraudDetectionService::sendFraudAlert(std::shared_ptr<Transaction> transaction) const {
    // In a real system, this would send emails, SMS, push notifications, etc.
    std::cout << "🚨 FRAUD ALERT sent for Transaction " << transaction->getTransactionId()
              << " - Amount: $" << transaction->getAmount() 
              << " - Location: " << transaction->getLocation() << std::endl;
}

// Private methods
bool FraudDetectionService::checkHighValueTransaction(std::shared_ptr<Transaction> transaction) {
    auto it = std::find_if(fraud_rules.begin(), fraud_rules.end(),
        [](const FraudRule& rule) {
            return rule.rule_name == "High Value Transaction" && rule.enabled;
        });
    
    if (it != fraud_rules.end()) {
        return transaction->getAmount() > it->threshold_value;
    }
    
    return false;
}

bool FraudDetectionService::checkUnusualLocation(std::shared_ptr<Transaction> transaction) {
    // Simple location check - in reality, this would be more sophisticated
    std::string location = transaction->getLocation();
    
    // Consider non-common locations as unusual
    std::vector<std::string> common_locations = {"New York", "Chicago", "Los Angeles", "Boston"};
    
    return std::find(common_locations.begin(), common_locations.end(), location) == common_locations.end();
}

bool FraudDetectionService::checkRapidTransactions(std::shared_ptr<Transaction> transaction) {
    // Simple implementation - check if there have been many recent transactions
    // In a real system, this would check transaction velocity per account
    
    std::lock_guard<std::mutex> lock(service_mutex);
    int recent_count = 0;
    auto current_time = transaction->getTimestamp();
    
    for (const auto& flagged_tx : flagged_transactions) {
        if (flagged_tx->getAccountId() == transaction->getAccountId()) {
            auto time_diff = std::chrono::duration_cast<std::chrono::minutes>(
                current_time - flagged_tx->getTimestamp()
            ).count();
            
            if (time_diff < 60) { // Within last hour
                recent_count++;
            }
        }
    }
    
    return recent_count >= 3; // 3 or more transactions in an hour
}

bool FraudDetectionService::checkUnusualTime(std::shared_ptr<Transaction> transaction) {
    auto time_t = std::chrono::system_clock::to_time_t(transaction->getTimestamp());
    auto tm = *std::localtime(&time_t);
    int hour = tm.tm_hour;
    
    // Consider transactions between 11 PM and 5 AM as unusual
    return (hour >= 23 || hour <= 5);
}

bool FraudDetectionService::checkVelocityPattern(std::shared_ptr<Transaction> transaction) {
    // Placeholder for velocity pattern detection
    return false;
}

void FraudDetectionService::updateAccountProfile(std::shared_ptr<Transaction> transaction) {
    int account_id = transaction->getAccountId();
    
    auto it = account_profiles.find(account_id);
    if (it == account_profiles.end()) {
        // Create new profile
        AccountProfile profile(account_id);
        profile.average_transaction_amount = transaction->getAmount();
        profile.max_transaction_amount = transaction->getAmount();
        profile.daily_transaction_count = 1;
        
        if (!transaction->getLocation().empty()) {
            profile.common_locations.push_back(transaction->getLocation());
        }
        
        account_profiles[account_id] = profile;
    } else {
        // Update existing profile
        AccountProfile& profile = it->second;
        
        // Update running averages (simplified)
        profile.average_transaction_amount = 
            (profile.average_transaction_amount + transaction->getAmount()) / 2.0;
        
        if (transaction->getAmount() > profile.max_transaction_amount) {
            profile.max_transaction_amount = transaction->getAmount();
        }
        
        profile.daily_transaction_count++;
        
        // Add location if not already present
        std::string location = transaction->getLocation();
        if (!location.empty()) {
            auto loc_it = std::find(profile.common_locations.begin(), 
                                  profile.common_locations.end(), location);
            if (loc_it == profile.common_locations.end()) {
                profile.common_locations.push_back(location);
            }
        }
    }
}

AccountProfile* FraudDetectionService::getAccountProfile(int account_id) {
    auto it = account_profiles.find(account_id);
    return (it != account_profiles.end()) ? &(it->second) : nullptr;
}

void FraudDetectionService::backgroundFraudDetection() {
    std::cout << "Background fraud detection thread started" << std::endl;
    
    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        
        // Perform background analysis
        std::lock_guard<std::mutex> lock(service_mutex);
        
        // In a real system, this would:
        // 1. Check for pattern anomalies
        // 2. Update risk scores
        // 3. Generate periodic reports
        // 4. Clean up old data
        
        if (!flagged_transactions.empty()) {
            // Simulate some background processing
            std::cout << "Background scan: " << flagged_transactions.size() 
                      << " suspicious transactions under review" << std::endl;
        }
    }
    
    std::cout << "Background fraud detection thread stopped" << std::endl;
}
