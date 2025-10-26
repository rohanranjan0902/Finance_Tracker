# FinTrack User Guide

Welcome to FinTrack, your personal finance management system!

## Getting Started

### Starting the Application

Run the FinTrack executable from your terminal:

**Windows:**
```cmd
FinTrack.exe
```

**Linux/macOS:**
```bash
./FinTrack
```

## Main Features

### 1. Creating a User Account

When you first start FinTrack, you'll see the main menu:

```
1. Create User
2. Login
0. Exit
```

To create a new account:
1. Select option `1`
2. Enter your full name
3. Enter a valid email address
4. Create a password
5. Confirm your password

**Note:** Your email must be unique and in a valid format (e.g., user@example.com)

### 2. Logging In

After creating an account:
1. Select option `2` from the main menu
2. Enter your email address
3. Enter your password

Upon successful login, you'll see a personalized menu with your name.

### 3. Managing Accounts

Once logged in, you can manage multiple financial accounts.

#### Creating an Account

1. Select `2. Create Account` from the logged-in menu
2. Choose your account type:
   - **Savings**: For saving money with typically higher interest
   - **Checking**: For day-to-day transactions
   - **Credit**: For credit card accounts
   - **Investment**: For investment portfolios
3. Enter an initial balance
4. Your new account will be assigned a unique Account ID

#### Viewing Your Accounts

Select `1. View Accounts` to see:
- Account ID
- Account Type
- Current Balance

Keep note of your Account IDs as you'll need them for transactions.

### 4. Making Transactions

#### Depositing Money

1. Select `3. Deposit`
2. Enter the Account ID where you want to deposit
3. Enter the amount to deposit
4. Optionally add a description (e.g., "Salary", "Gift")
5. Confirm the transaction

#### Withdrawing Money

1. Select `4. Withdraw`
2. Enter the Account ID to withdraw from
3. Enter the withdrawal amount
4. Optionally add a description (e.g., "ATM withdrawal", "Bills")
5. Confirm the transaction

**Note:** You must have sufficient funds in your account (except for Credit accounts)

#### Transferring Money

1. Select `5. Transfer`
2. Enter your source Account ID (where money comes from)
3. Enter the destination Account ID (where money goes to)
4. Enter the transfer amount
5. Optionally add a description
6. Confirm the transaction

**Note:** You can transfer to accounts belonging to other users if you know their Account ID

### 5. Viewing Transaction History

1. Select `6. View Transaction History`
2. Enter the Account ID you want to view
3. You'll see up to 10 recent transactions showing:
   - Date and time
   - Transaction type (Deposit, Withdrawal, Transfer)
   - Amount
   - Description

### 6. Logging Out

Select `7. Logout` to safely log out of your account while keeping the application running. This is useful in shared environments.

## Tips and Best Practices

### Security
- Choose a strong password for your account
- Always log out when finished, especially on shared computers
- Keep your Account IDs private

### Account Management
- Use Savings accounts for long-term goals
- Use Checking accounts for daily expenses
- Monitor your balances regularly
- Add descriptions to transactions for better tracking

### Error Handling
- If you enter invalid input, the system will prompt you to try again
- Common errors:
  - **Insufficient funds**: You're trying to withdraw more than your balance
  - **Account not found**: Double-check your Account ID
  - **Invalid amount**: Make sure to enter positive numbers

## Common Tasks

### Task: Pay a Bill

1. Log in to your account
2. Select `4. Withdraw`
3. Choose your Checking account
4. Enter the bill amount
5. Add a description like "Electric Bill - January"

### Task: Save Money

1. Log in to your account
2. Select `5. Transfer`
3. Transfer from your Checking account to your Savings account
4. Enter the amount to save
5. Add a description like "Monthly Savings"

### Task: Track Your Spending

1. Log in to your account
2. Select `6. View Transaction History`
3. Review your recent transactions
4. Note patterns in your spending

## Troubleshooting

### Problem: Can't log in
- **Solution**: Verify your email is correct and password matches what you created
- Make sure you created an account first

### Problem: Transaction failed
- **Solution**: Check if you have sufficient funds in your account
- Verify the Account IDs are correct

### Problem: Invalid input error
- **Solution**: Make sure you're entering numbers when asked for amounts
- Don't use negative numbers for transaction amounts

## Support

For additional assistance or to report issues, please contact your system administrator or refer to the project documentation.

## Quick Reference

| Action | Menu Option |
|--------|-------------|
| Create new user | Main Menu → 1 |
| Login | Main Menu → 2 |
| View accounts | Logged In → 1 |
| Create account | Logged In → 2 |
| Make deposit | Logged In → 3 |
| Make withdrawal | Logged In → 4 |
| Transfer money | Logged In → 5 |
| View history | Logged In → 6 |
| Logout | Logged In → 7 |
| Exit application | 0 (from any menu) |

---

**Thank you for using FinTrack!**

