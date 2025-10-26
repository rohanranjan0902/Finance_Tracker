#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class InsufficientFundsException : public std::runtime_error {
public:
    explicit InsufficientFundsException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidAccountException : public std::runtime_error {
public:
    explicit InvalidAccountException(const std::string& message)
        : std::runtime_error(message) {}
};

class InvalidTransactionException : public std::runtime_error {
public:
    explicit InvalidTransactionException(const std::string& message)
        : std::runtime_error(message) {}
};

class DatabaseException : public std::runtime_error {
public:
    explicit DatabaseException(const std::string& message)
        : std::runtime_error(message) {}
};

class AuthenticationException : public std::runtime_error {
public:
    explicit AuthenticationException(const std::string& message)
        : std::runtime_error(message) {}
};

#endif // EXCEPTIONS_H

