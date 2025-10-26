#ifndef BUDGET_H
#define BUDGET_H

#include <string>
#include <map>
#include <chrono>
#include "Transaction.h"

class Budget {
private:
    int budget_id;
    int user_id;
    TransactionCategory category;
    double monthly_limit;
    double current_spent;
    std::chrono::system_clock::time_point start_date;
    std::chrono::system_clock::time_point end_date;
    bool alert_enabled;
    double alert_threshold; // Percentage (0.0 to 1.0)

public:
    // Constructors
    Budget();
    Budget(int budget_id, int user_id, TransactionCategory category, double monthly_limit, 
           double alert_threshold = 0.8);
    
    // Destructor
    ~Budget();
    
    // Getters
    int getBudgetId() const;
    int getUserId() const;
    TransactionCategory getCategory() const;
    double getMonthlyLimit() const;
    double getCurrentSpent() const;
    double getRemainingBudget() const;
    double getSpentPercentage() const;
    std::chrono::system_clock::time_point getStartDate() const;
    std::chrono::system_clock::time_point getEndDate() const;
    bool isAlertEnabled() const;
    double getAlertThreshold() const;
    
    // Setters
    void setMonthlyLimit(double limit);
    void setAlertEnabled(bool enabled);
    void setAlertThreshold(double threshold);
    
    // Budget operations
    void addExpense(double amount);
    void resetBudget(); // Reset for new month
    bool isOverBudget() const;
    bool shouldAlert() const;
    
    // Utility functions
    void displayBudgetInfo() const;
    std::string getBudgetStatus() const;
    std::string getCategoryString() const;
    
    // Date operations
    void updatePeriod(); // Update to current month
    bool isCurrentPeriod() const;
};

// Budget Manager class for handling multiple budgets
class BudgetManager {
private:
    std::map<TransactionCategory, Budget> budgets;
    int user_id;

public:
    // Constructor
    BudgetManager(int user_id);
    
    // Destructor
    ~BudgetManager();
    
    // Budget management
    void addBudget(const Budget& budget);
    void removeBudget(TransactionCategory category);
    Budget* getBudget(TransactionCategory category);
    
    // Expense tracking
    void recordExpense(TransactionCategory category, double amount);
    std::vector<Budget> getOverBudgets() const;
    std::vector<Budget> getAlertsNeeded() const;
    
    // Reports
    void displayAllBudgets() const;
    double getTotalBudget() const;
    double getTotalSpent() const;
    
    // Utility
    void resetAllBudgets(); // Reset all for new month
};

#endif // BUDGET_H
