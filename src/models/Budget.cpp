#include "Budget.h"
#include <algorithm>

// Budget class implementation
Budget::Budget() 
    : budget_id(0), user_id(0), category(TransactionCategory::OTHER), 
      monthly_limit(0.0), current_spent(0.0), alert_enabled(true), alert_threshold(0.8) {
    updatePeriod();
}

Budget::Budget(int budget_id, int user_id, TransactionCategory category, double monthly_limit, double alert_threshold)
    : budget_id(budget_id), user_id(user_id), category(category), monthly_limit(monthly_limit), 
      current_spent(0.0), alert_enabled(true), alert_threshold(alert_threshold) {
    updatePeriod();
}

Budget::~Budget() {}

// Getters
int Budget::getBudgetId() const {
    return budget_id;
}

int Budget::getUserId() const {
    return user_id;
}

TransactionCategory Budget::getCategory() const {
    return category;
}

double Budget::getMonthlyLimit() const {
    return monthly_limit;
}

double Budget::getCurrentSpent() const {
    return current_spent;
}

double Budget::getRemainingBudget() const {
    return monthly_limit - current_spent;
}

double Budget::getSpentPercentage() const {
    if (monthly_limit <= 0) return 0.0;
    return current_spent / monthly_limit;
}

std::chrono::system_clock::time_point Budget::getStartDate() const {
    return start_date;
}

std::chrono::system_clock::time_point Budget::getEndDate() const {
    return end_date;
}

bool Budget::isAlertEnabled() const {
    return alert_enabled;
}

double Budget::getAlertThreshold() const {
    return alert_threshold;
}

void Budget::setMonthlyLimit(double limit) {
    if (limit < 0) {
        throw std::invalid_argument("Monthly limit cannot be negative");
    }
    this->monthly_limit = limit;
}

void Budget::setAlertEnabled(bool enabled) {
    this->alert_enabled = enabled;
}

void Budget::setAlertThreshold(double threshold) {
    if (threshold < 0.0 || threshold > 1.0) {
        throw std::invalid_argument("Alert threshold must be between 0.0 and 1.0");
    }
    this->alert_threshold = threshold;
}

void Budget::addExpense(double amount) {
    if (amount > 0) {
        current_spent += amount;
    }
}

void Budget::resetBudget() {
    current_spent = 0.0;
    updatePeriod();
}

bool Budget::isOverBudget() const {
    return current_spent > monthly_limit;
}

bool Budget::shouldAlert() const {
    return alert_enabled && (getSpentPercentage() >= alert_threshold);
}

void Budget::displayBudgetInfo() const {
    // This method is intentionally left for backwards compatibility
    // but should not be used in production. UI layer should handle display.
}

std::string Budget::getBudgetStatus() const {
    if (isOverBudget()) {
        return "⚠️  OVER BUDGET";
    } else if (shouldAlert()) {
        return "🔔 APPROACHING LIMIT";
    } else if (getSpentPercentage() < 0.5) {
        return "✅ GOOD";
    } else {
        return "⚡ ON TRACK";
    }
}

std::string Budget::getCategoryString() const {
    return Transaction::transactionCategoryToString(category);
}

// Date operations
void Budget::updatePeriod() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    // Set start to first day of current month
    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    start_date = std::chrono::system_clock::from_time_t(std::mktime(&tm));
    
    // Set end to last day of current month
    tm.tm_mon += 1; // Next month
    if (tm.tm_mon > 11) {
        tm.tm_mon = 0;
        tm.tm_year += 1;
    }
    tm.tm_mday = 0; // This will be the last day of the previous month
    tm.tm_hour = 23;
    tm.tm_min = 59;
    tm.tm_sec = 59;
    end_date = std::chrono::system_clock::from_time_t(std::mktime(&tm));
}

bool Budget::isCurrentPeriod() const {
    auto now = std::chrono::system_clock::now();
    return (now >= start_date && now <= end_date);
}

// BudgetManager implementation
BudgetManager::BudgetManager(int user_id) : user_id(user_id) {}

BudgetManager::~BudgetManager() {}

void BudgetManager::addBudget(const Budget& budget) {
    if (budget.getUserId() != user_id) {
        throw std::invalid_argument("Budget user ID doesn't match manager user ID");
    }
    budgets[budget.getCategory()] = budget;
}

void BudgetManager::removeBudget(TransactionCategory category) {
    auto it = budgets.find(category);
    if (it == budgets.end()) {
        throw std::invalid_argument("Budget not found for specified category");
    }
    budgets.erase(it);
}

Budget* BudgetManager::getBudget(TransactionCategory category) {
    auto it = budgets.find(category);
    if (it != budgets.end()) {
        return &(it->second);
    }
    return nullptr;
}

void BudgetManager::recordExpense(TransactionCategory category, double amount) {
    auto* budget = getBudget(category);
    if (budget) {
        budget->addExpense(amount);
    }
}

std::vector<Budget> BudgetManager::getOverBudgets() const {
    std::vector<Budget> over_budgets;
    for (const auto& pair : budgets) {
        if (pair.second.isOverBudget()) {
            over_budgets.push_back(pair.second);
        }
    }
    return over_budgets;
}

std::vector<Budget> BudgetManager::getAlertsNeeded() const {
    std::vector<Budget> alert_budgets;
    for (const auto& pair : budgets) {
        if (pair.second.shouldAlert() && !pair.second.isOverBudget()) {
            alert_budgets.push_back(pair.second);
        }
    }
    return alert_budgets;
}

void BudgetManager::displayAllBudgets() const {
    // This method is intentionally left for backwards compatibility
    // but should not be used in production. UI layer should handle display.
}

double BudgetManager::getTotalBudget() const {
    double total = 0.0;
    for (const auto& pair : budgets) {
        total += pair.second.getMonthlyLimit();
    }
    return total;
}

double BudgetManager::getTotalSpent() const {
    double total = 0.0;
    for (const auto& pair : budgets) {
        total += pair.second.getCurrentSpent();
    }
    return total;
}

void BudgetManager::resetAllBudgets() {
    for (auto& pair : budgets) {
        pair.second.resetBudget();
    }
}
