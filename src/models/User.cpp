#include "User.h"
#include "Account.h"
#include "../exceptions.h"
#include <algorithm>
#include <regex>

User::User() : user_id(0), name(""), email(""), password_hash("") {}

User::User(int id, const std::string& name, const std::string& email, const std::string& password_hash)
    : user_id(id), name(name), email(email), password_hash(password_hash) {}

User::~User() {
    accounts.clear();
}
int User::getUserId() const {
    return user_id;
}

std::string User::getName() const {
    return name;
}

std::string User::getEmail() const {
    return email;
}

std::string User::getPasswordHash() const {
    return password_hash;
}

std::vector<std::shared_ptr<Account>> User::getAccounts() const {
    return accounts;
}

void User::setName(const std::string& name) {
    this->name = name;
}

void User::setEmail(const std::string& email) {
    if (!validateEmail(email)) {
        throw std::invalid_argument("Invalid email format");
    }
    this->email = email;
}

void User::setPasswordHash(const std::string& password_hash) {
    this->password_hash = password_hash;
}

void User::addAccount(std::shared_ptr<Account> account) {
    if (!account) {
        throw InvalidAccountException("Cannot add null account");
    }
    if (account->getUserId() != user_id) {
        throw InvalidAccountException("Account user ID mismatch");
    }
    accounts.push_back(account);
}

void User::removeAccount(int account_id) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [account_id](const std::shared_ptr<Account>& acc) {
            return acc->getAccountId() == account_id;
        });
    
    if (it == accounts.end()) {
        throw InvalidAccountException("Account not found");
    }
    accounts.erase(it);
}

std::shared_ptr<Account> User::getAccount(int account_id) {
    auto it = std::find_if(accounts.begin(), accounts.end(),
        [account_id](const std::shared_ptr<Account>& acc) {
            return acc->getAccountId() == account_id;
        });
    
    if (it != accounts.end()) {
        return *it;
    }
    return nullptr;
}

void User::displayUserInfo() const {
    // This method is intentionally left for backwards compatibility
    // but should not be used in production. UI layer should handle display.
}

bool User::validateEmail(const std::string& email) const {
    const std::regex pattern(R"([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})");
    return std::regex_match(email, pattern);
}
