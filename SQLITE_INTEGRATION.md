# SQLite Integration Guide

## Overview

Currently, FinTrack stores data in memory, which means all data is lost when the application closes. To add persistent storage, you'll need to integrate SQLite.

## Current State

The project includes:
- ✅ `DatabaseService.h` - Header file with database interface
- ✅ Exception handling for database errors
- ⚠️ `DatabaseService.cpp` - Not yet implemented (requires SQLite library)

## Implementation Steps

### 1. Download SQLite

Download the SQLite amalgamation from: https://www.sqlite.org/download.html

You need:
- **sqlite3.c** - The SQLite library source
- **sqlite3.h** - The SQLite header file

### 2. Add SQLite to Project

Create a vendor directory and add the files:

```
FinTrack/
├── vendor/
│   └── sqlite/
│       ├── sqlite3.c
│       └── sqlite3.h
```

### 3. Update CMakeLists.txt

Add SQLite to your build:

```cmake
# After line 21 (include_directories(src))
include_directories(vendor/sqlite)

# After line 29 (SERVICE_SOURCES)
set(VENDOR_SOURCES
    vendor/sqlite/sqlite3.c
)

# Update ALL_SOURCES (around line 36)
set(ALL_SOURCES
    ${MODEL_SOURCES}
    ${SERVICE_SOURCES}
    ${VENDOR_SOURCES}  # Add this line
    src/main.cpp
)
```

### 4. Implement DatabaseService.cpp

Create `src/services/DatabaseService.cpp`:

```cpp
#include "DatabaseService.h"
#include "../models/User.h"
#include "../models/Account.h"
#include "../models/Transaction.h"
#include "../models/Budget.h"
#include "../../vendor/sqlite/sqlite3.h"
#include <sstream>

DatabaseService::DatabaseService(const std::string& database_path) 
    : db(nullptr), db_path(database_path) {
    
    int rc = sqlite3_open(db_path.c_str(), (sqlite3**)&db);
    if (rc != SQLITE_OK) {
        throw DatabaseException("Failed to open database");
    }
    
    createTables();
}

DatabaseService::~DatabaseService() {
    if (db) {
        sqlite3_close((sqlite3*)db);
    }
}

void DatabaseService::createTables() {
    // Create Users table
    const char* create_users = 
        "CREATE TABLE IF NOT EXISTS users ("
        "user_id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL,"
        "email TEXT UNIQUE NOT NULL,"
        "password_hash TEXT NOT NULL"
        ");";
    
    // Create Accounts table
    const char* create_accounts = 
        "CREATE TABLE IF NOT EXISTS accounts ("
        "account_id INTEGER PRIMARY KEY,"
        "user_id INTEGER NOT NULL,"
        "type TEXT NOT NULL,"
        "balance REAL NOT NULL,"
        "FOREIGN KEY (user_id) REFERENCES users(user_id)"
        ");";
    
    // Create Transactions table
    const char* create_transactions = 
        "CREATE TABLE IF NOT EXISTS transactions ("
        "transaction_id INTEGER PRIMARY KEY,"
        "account_id INTEGER NOT NULL,"
        "to_account_id INTEGER,"
        "amount REAL NOT NULL,"
        "type TEXT NOT NULL,"
        "category TEXT NOT NULL,"
        "status TEXT NOT NULL,"
        "description TEXT,"
        "timestamp INTEGER NOT NULL,"
        "suspicious_flag INTEGER DEFAULT 0,"
        "location TEXT,"
        "FOREIGN KEY (account_id) REFERENCES accounts(account_id)"
        ");";
    
    // Create Budgets table
    const char* create_budgets = 
        "CREATE TABLE IF NOT EXISTS budgets ("
        "budget_id INTEGER PRIMARY KEY,"
        "user_id INTEGER NOT NULL,"
        "category TEXT NOT NULL,"
        "monthly_limit REAL NOT NULL,"
        "current_spent REAL DEFAULT 0,"
        "alert_threshold REAL DEFAULT 0.8,"
        "FOREIGN KEY (user_id) REFERENCES users(user_id)"
        ");";
    
    executeSQL(create_users);
    executeSQL(create_accounts);
    executeSQL(create_transactions);
    executeSQL(create_budgets);
}

void DatabaseService::executeSQL(const std::string& sql) {
    char* err_msg = nullptr;
    int rc = sqlite3_exec((sqlite3*)db, sql.c_str(), nullptr, nullptr, &err_msg);
    
    if (rc != SQLITE_OK) {
        std::string error = err_msg ? err_msg : "Unknown error";
        sqlite3_free(err_msg);
        throw DatabaseException("SQL error: " + error);
    }
}

void DatabaseService::saveUser(const User& user) {
    std::ostringstream sql;
    sql << "INSERT OR REPLACE INTO users VALUES ("
        << user.getUserId() << ", "
        << "'" << user.getName() << "', "
        << "'" << user.getEmail() << "', "
        << "'" << user.getPasswordHash() << "');";
    
    executeSQL(sql.str());
}

// Implement other methods similarly...
```

### 5. Update main.cpp

Modify your main.cpp to use the DatabaseService:

```cpp
#include "services/DatabaseService.h"

// At the start of main():
int main() {
    DatabaseService db("fintrack.db");
    
    // Load existing users from database
    auto users = db.loadAllUsers();
    for (const auto& user : users) {
        users_by_email[user->getEmail()] = user;
        
        // Load accounts for each user
        auto accounts = db.loadAccountsForUser(user->getUserId());
        for (const auto& account : accounts) {
            user->addAccount(account);
            accounts_by_id[account->getAccountId()] = account;
        }
    }
    
    // ... rest of main() code
}
```

### 6. Persist Data Changes

After each data modification, save to database:

```cpp
// In handleCreateUser():
auto user = std::make_shared<User>(next_user_id++, name, email, hashPassword(password));
users_by_email[email] = user;
db.saveUser(*user);  // Add this line

// In handleCreateAccount():
auto account = std::make_shared<Account>(next_account_id++, logged_in_user->getUserId(), type, initial_balance);
logged_in_user->addAccount(account);
accounts_by_id[account->getAccountId()] = account;
db.saveAccount(*account);  // Add this line

// In transaction handlers:
transaction_service.processDeposit(account, amount, description);
db.updateAccountBalance(account->getAccountId(), account->getBalance());  // Add this
```

## Testing Database Integration

1. Run the application and create a user
2. Close the application
3. Run it again - your user should still exist
4. Create an account and make transactions
5. Close and reopen - all data should persist

## Benefits of SQLite

- **Persistent Storage**: Data survives application restarts
- **ACID Compliance**: Reliable transactions
- **Zero Configuration**: No separate database server needed
- **Portable**: Single file database
- **Fast**: Optimized for local storage
- **Widely Used**: Battle-tested in millions of applications

## Optional Enhancements

### Add Indices for Performance

```sql
CREATE INDEX idx_user_email ON users(email);
CREATE INDEX idx_account_user ON accounts(user_id);
CREATE INDEX idx_transaction_account ON transactions(account_id);
```

### Add Backup Functionality

```cpp
void backupDatabase(const std::string& backup_path) {
    // SQLite provides built-in backup API
    sqlite3_backup* backup = sqlite3_backup_init(
        backup_db, "main", db, "main"
    );
    sqlite3_backup_step(backup, -1);
    sqlite3_backup_finish(backup);
}
```

### Add Data Migration

If you change your schema:

```cpp
void migrateDatabase() {
    // Check schema version
    // Apply migrations as needed
}
```

## Troubleshooting

### Database locked error
- Only one process can write at a time
- Use WAL mode: `PRAGMA journal_mode=WAL;`

### Slow performance
- Add indices on frequently queried columns
- Use prepared statements for repeated queries
- Batch multiple inserts in a transaction

### Database corruption
- Always close properly: `sqlite3_close()`
- Handle crashes gracefully
- Keep regular backups

## Resources

- [SQLite Official Documentation](https://www.sqlite.org/docs.html)
- [SQLite C/C++ Interface](https://www.sqlite.org/c3ref/intro.html)
- [SQL Tutorial](https://www.w3schools.com/sql/)

## Alternative: SQLiteCpp Library

For a more C++-friendly interface, consider using SQLiteCpp:

https://github.com/SRombauts/SQLiteCpp

It provides:
- RAII for automatic resource management
- C++ exceptions instead of error codes
- Prepared statements wrapper
- Type-safe parameter binding

## Next Steps

1. Download SQLite3 amalgamation
2. Implement DatabaseService.cpp
3. Update CMakeLists.txt
4. Modify main.cpp to load/save data
5. Test thoroughly
6. Add proper error handling
7. Consider adding automatic backups

---

**Note**: The current version of FinTrack is fully functional without database persistence. This guide is for users who want to add that feature.

