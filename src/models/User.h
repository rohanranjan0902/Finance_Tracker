#ifndef USER_H
#define USER_H

#include <string>
#include <vector>
#include <memory>

class Account;

class User {
private:
    int user_id;
    std::string name;
    std::string email;
    std::string password_hash;
    std::vector<std::shared_ptr<Account>> accounts;

public:
    // Constructors
    User();
    User(int id, const std::string& name, const std::string& email, const std::string& password_hash);
    
    // Destructor
    ~User();
    
    // Getters
    int getUserId() const;
    std::string getName() const;
    std::string getEmail() const;
    std::string getPasswordHash() const;
    std::vector<std::shared_ptr<Account>> getAccounts() const;
    
    // Setters
    void setName(const std::string& name);
    void setEmail(const std::string& email);
    void setPasswordHash(const std::string& password_hash);
    
    // Account management
    void addAccount(std::shared_ptr<Account> account);
    void removeAccount(int account_id);
    std::shared_ptr<Account> getAccount(int account_id);
    
    // Display
    void displayUserInfo() const;
    
    // Validation
    bool validateEmail(const std::string& email) const;
};

#endif // USER_H
