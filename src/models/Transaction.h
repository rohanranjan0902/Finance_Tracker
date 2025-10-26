#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <chrono>
#include <ctime>

enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER_OUT,
    TRANSFER_IN,
    PAYMENT,
    REFUND
};

enum class TransactionCategory {
    FOOD,
    TRAVEL,
    BILLS,
    ENTERTAINMENT,
    SHOPPING,
    HEALTHCARE,
    INVESTMENT,
    SALARY,
    OTHER
};

enum class TransactionStatus {
    PENDING,
    COMPLETED,
    FAILED,
    CANCELLED
};

class Transaction {
private:
    int transaction_id;
    int account_id;
    int to_account_id;  // For transfers
    double amount;
    TransactionType type;
    TransactionCategory category;
    TransactionStatus status;
    std::string description;
    std::chrono::system_clock::time_point timestamp;
    bool suspicious_flag;
    std::string location;
    std::string ip_address;

public:
    // Constructors
    Transaction();
    Transaction(int tx_id, int account_id, double amount, TransactionType type, 
               TransactionCategory category = TransactionCategory::OTHER, 
               const std::string& description = "");
    
    // Destructor
    ~Transaction();
    
    // Getters
    int getTransactionId() const;
    int getAccountId() const;
    int getToAccountId() const;
    double getAmount() const;
    TransactionType getType() const;
    TransactionCategory getCategory() const;
    TransactionStatus getStatus() const;
    std::string getDescription() const;
    std::chrono::system_clock::time_point getTimestamp() const;
    std::string getTimestampString() const;
    bool isSuspicious() const;
    std::string getLocation() const;
    std::string getIpAddress() const;
    
    // Setters
    void setToAccountId(int to_account_id);
    void setStatus(TransactionStatus status);
    void setSuspiciousFlag(bool suspicious);
    void setLocation(const std::string& location);
    void setIpAddress(const std::string& ip_address);
    void setCategory(TransactionCategory category);
    
    // Utility functions
    void displayTransaction() const;
    std::string getTypeString() const;
    std::string getCategoryString() const;
    std::string getStatusString() const;
    
    // Static utility functions
    static std::string transactionTypeToString(TransactionType type);
    static std::string transactionCategoryToString(TransactionCategory category);
    static std::string transactionStatusToString(TransactionStatus status);
    static TransactionCategory stringToCategory(const std::string& category_str);
    static TransactionType stringToType(const std::string& type_str);
};

#endif // TRANSACTION_H
