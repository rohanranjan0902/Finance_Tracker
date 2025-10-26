#include "Transaction.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction() 
    : transaction_id(0), account_id(0), to_account_id(-1), amount(0.0),
      type(TransactionType::DEPOSIT), category(TransactionCategory::OTHER),
      status(TransactionStatus::PENDING), description(""),
      timestamp(std::chrono::system_clock::now()), suspicious_flag(false),
      location(""), ip_address("") {}

Transaction::Transaction(int tx_id, int account_id, double amount, TransactionType type,
                        TransactionCategory category, const std::string& description)
    : transaction_id(tx_id), account_id(account_id), to_account_id(-1), amount(amount),
      type(type), category(category), status(TransactionStatus::PENDING),
      description(description), timestamp(std::chrono::system_clock::now()),
      suspicious_flag(false), location(""), ip_address("") {}

Transaction::~Transaction() {}

// Getters
int Transaction::getTransactionId() const {
    return transaction_id;
}

int Transaction::getAccountId() const {
    return account_id;
}

int Transaction::getToAccountId() const {
    return to_account_id;
}

double Transaction::getAmount() const {
    return amount;
}

TransactionType Transaction::getType() const {
    return type;
}

TransactionCategory Transaction::getCategory() const {
    return category;
}

TransactionStatus Transaction::getStatus() const {
    return status;
}

std::string Transaction::getDescription() const {
    return description;
}

std::chrono::system_clock::time_point Transaction::getTimestamp() const {
    return timestamp;
}

std::string Transaction::getTimestampString() const {
    auto time_t = std::chrono::system_clock::to_time_t(timestamp);
    auto tm = *std::localtime(&time_t);
    
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

bool Transaction::isSuspicious() const {
    return suspicious_flag;
}

std::string Transaction::getLocation() const {
    return location;
}

std::string Transaction::getIpAddress() const {
    return ip_address;
}

void Transaction::setToAccountId(int to_account_id) {
    this->to_account_id = to_account_id;
}

void Transaction::setStatus(TransactionStatus status) {
    this->status = status;
}

void Transaction::setSuspiciousFlag(bool suspicious) {
    this->suspicious_flag = suspicious;
}

void Transaction::setLocation(const std::string& location) {
    this->location = location;
}

void Transaction::setIpAddress(const std::string& ip_address) {
    this->ip_address = ip_address;
}

void Transaction::setCategory(TransactionCategory category) {
    this->category = category;
}

void Transaction::displayTransaction() const {
    // This method is intentionally left for backwards compatibility
    // but should not be used in production. UI layer should handle display.
}

std::string Transaction::getTypeString() const {
    return transactionTypeToString(type);
}

std::string Transaction::getCategoryString() const {
    return transactionCategoryToString(category);
}

std::string Transaction::getStatusString() const {
    return transactionStatusToString(status);
}

// Static utility functions
std::string Transaction::transactionTypeToString(TransactionType type) {
    switch (type) {
        case TransactionType::DEPOSIT: return "Deposit";
        case TransactionType::WITHDRAWAL: return "Withdrawal";
        case TransactionType::TRANSFER_OUT: return "Transfer Out";
        case TransactionType::TRANSFER_IN: return "Transfer In";
        case TransactionType::PAYMENT: return "Payment";
        case TransactionType::REFUND: return "Refund";
        default: return "Unknown";
    }
}

std::string Transaction::transactionCategoryToString(TransactionCategory category) {
    switch (category) {
        case TransactionCategory::FOOD: return "Food";
        case TransactionCategory::TRAVEL: return "Travel";
        case TransactionCategory::BILLS: return "Bills";
        case TransactionCategory::ENTERTAINMENT: return "Entertainment";
        case TransactionCategory::SHOPPING: return "Shopping";
        case TransactionCategory::HEALTHCARE: return "Healthcare";
        case TransactionCategory::INVESTMENT: return "Investment";
        case TransactionCategory::SALARY: return "Salary";
        case TransactionCategory::OTHER: return "Other";
        default: return "Unknown";
    }
}

std::string Transaction::transactionStatusToString(TransactionStatus status) {
    switch (status) {
        case TransactionStatus::PENDING: return "Pending";
        case TransactionStatus::COMPLETED: return "Completed";
        case TransactionStatus::FAILED: return "Failed";
        case TransactionStatus::CANCELLED: return "Cancelled";
        default: return "Unknown";
    }
}

TransactionCategory Transaction::stringToCategory(const std::string& category_str) {
    if (category_str == "Food") return TransactionCategory::FOOD;
    if (category_str == "Travel") return TransactionCategory::TRAVEL;
    if (category_str == "Bills") return TransactionCategory::BILLS;
    if (category_str == "Entertainment") return TransactionCategory::ENTERTAINMENT;
    if (category_str == "Shopping") return TransactionCategory::SHOPPING;
    if (category_str == "Healthcare") return TransactionCategory::HEALTHCARE;
    if (category_str == "Investment") return TransactionCategory::INVESTMENT;
    if (category_str == "Salary") return TransactionCategory::SALARY;
    return TransactionCategory::OTHER; // Default
}

TransactionType Transaction::stringToType(const std::string& type_str) {
    if (type_str == "Deposit") return TransactionType::DEPOSIT;
    if (type_str == "Withdrawal") return TransactionType::WITHDRAWAL;
    if (type_str == "Transfer Out") return TransactionType::TRANSFER_OUT;
    if (type_str == "Transfer In") return TransactionType::TRANSFER_IN;
    if (type_str == "Payment") return TransactionType::PAYMENT;
    if (type_str == "Refund") return TransactionType::REFUND;
    return TransactionType::DEPOSIT; // Default
}
