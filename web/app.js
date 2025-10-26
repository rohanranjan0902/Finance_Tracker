// Data storage using localStorage
const StorageKeys = {
    USERS: 'fintrack_users',
    ACCOUNTS: 'fintrack_accounts',
    TRANSACTIONS: 'fintrack_transactions',
    CURRENT_USER: 'fintrack_current_user'
};

// Initialize data structures
let users = JSON.parse(localStorage.getItem(StorageKeys.USERS)) || [];
let accounts = JSON.parse(localStorage.getItem(StorageKeys.ACCOUNTS)) || [];
let transactions = JSON.parse(localStorage.getItem(StorageKeys.TRANSACTIONS)) || [];
let currentUser = JSON.parse(localStorage.getItem(StorageKeys.CURRENT_USER)) || null;

// Save data to localStorage
function saveData() {
    localStorage.setItem(StorageKeys.USERS, JSON.stringify(users));
    localStorage.setItem(StorageKeys.ACCOUNTS, JSON.stringify(accounts));
    localStorage.setItem(StorageKeys.TRANSACTIONS, JSON.stringify(transactions));
    localStorage.setItem(StorageKeys.CURRENT_USER, JSON.stringify(currentUser));
}

// Utility functions
function generateId(array) {
    return array.length > 0 ? Math.max(...array.map(item => item.id)) + 1 : 1;
}

function hashPassword(password) {
    // Simple hash for demo - in production use proper encryption
    return 'hashed_' + password;
}

function showToast(message, type = 'success') {
    const toast = document.getElementById('toast');
    toast.textContent = message;
    toast.className = `toast show ${type}`;
    setTimeout(() => {
        toast.className = 'toast';
    }, 3000);
}

// Tab switching
function showTab(tab) {
    const loginForm = document.getElementById('login-form');
    const registerForm = document.getElementById('register-form');
    const tabButtons = document.querySelectorAll('.tab-btn');
    
    tabButtons.forEach(btn => btn.classList.remove('active'));
    
    if (tab === 'login') {
        loginForm.style.display = 'block';
        registerForm.style.display = 'none';
        tabButtons[0].classList.add('active');
    } else {
        loginForm.style.display = 'none';
        registerForm.style.display = 'block';
        tabButtons[1].classList.add('active');
    }
}

// Modal management
function showModal(modalId) {
    const modal = document.getElementById(modalId);
    modal.classList.add('active');
    
    // Populate account dropdowns
    if (modalId.includes('deposit') || modalId.includes('withdraw') || 
        modalId.includes('transfer') || modalId.includes('history')) {
        populateAccountSelects();
    }
}

function closeModal(modalId) {
    const modal = document.getElementById(modalId);
    modal.classList.remove('active');
}

// Close modal when clicking outside
window.onclick = function(event) {
    if (event.target.classList.contains('modal')) {
        event.target.classList.remove('active');
    }
}

// Register form handler
document.getElementById('register-form').addEventListener('submit', function(e) {
    e.preventDefault();
    
    const name = document.getElementById('register-name').value;
    const email = document.getElementById('register-email').value;
    const password = document.getElementById('register-password').value;
    const confirm = document.getElementById('register-confirm').value;
    
    if (password !== confirm) {
        showToast('Passwords do not match', 'error');
        return;
    }
    
    if (users.find(u => u.email === email)) {
        showToast('Email already exists', 'error');
        return;
    }
    
    const newUser = {
        id: generateId(users),
        name,
        email,
        password: hashPassword(password)
    };
    
    users.push(newUser);
    saveData();
    
    showToast('Account created successfully!', 'success');
    this.reset();
    showTab('login');
});

// Login form handler
document.getElementById('login-form').addEventListener('submit', function(e) {
    e.preventDefault();
    
    const email = document.getElementById('login-email').value;
    const password = document.getElementById('login-password').value;
    
    const user = users.find(u => u.email === email && u.password === hashPassword(password));
    
    if (!user) {
        showToast('Invalid email or password', 'error');
        return;
    }
    
    currentUser = user;
    saveData();
    
    showDashboard();
    this.reset();
});

// Show dashboard
function showDashboard() {
    document.getElementById('auth-section').style.display = 'none';
    document.getElementById('dashboard-section').style.display = 'block';
    document.getElementById('user-name').textContent = currentUser.name;
    
    loadAccounts();
}

// Logout
function logout() {
    currentUser = null;
    saveData();
    
    document.getElementById('auth-section').style.display = 'flex';
    document.getElementById('dashboard-section').style.display = 'none';
    
    showToast('Logged out successfully', 'success');
}

// Load accounts
function loadAccounts() {
    const accountsList = document.getElementById('accounts-list');
    const userAccounts = accounts.filter(a => a.userId === currentUser.id);
    
    if (userAccounts.length === 0) {
        accountsList.innerHTML = `
            <div class="empty-state">
                <h3>No accounts yet</h3>
                <p>Create your first account to get started</p>
            </div>
        `;
        return;
    }
    
    accountsList.innerHTML = userAccounts.map(account => `
        <div class="account-card">
            <div class="account-type">${account.type}</div>
            <div class="account-balance">$${account.balance.toFixed(2)}</div>
            <div class="account-id">ID: ${account.id}</div>
        </div>
    `).join('');
}

// Create account handler
document.getElementById('create-account-form').addEventListener('submit', function(e) {
    e.preventDefault();
    
    const type = document.getElementById('account-type').value;
    const balance = parseFloat(document.getElementById('initial-balance').value);
    
    const newAccount = {
        id: generateId(accounts),
        userId: currentUser.id,
        type,
        balance
    };
    
    accounts.push(newAccount);
    saveData();
    
    loadAccounts();
    closeModal('create-account-modal');
    showToast('Account created successfully!', 'success');
    this.reset();
});

// Populate account selects
function populateAccountSelects() {
    const userAccounts = accounts.filter(a => a.userId === currentUser.id);
    const allAccounts = accounts; // For transfers to other users
    
    const selectIds = ['deposit-account', 'withdraw-account', 'transfer-from', 'transfer-to', 'history-account'];
    
    selectIds.forEach(id => {
        const select = document.getElementById(id);
        if (select) {
            const accountsToShow = (id === 'transfer-to') ? allAccounts : userAccounts;
            select.innerHTML = accountsToShow.map(acc => `
                <option value="${acc.id}">
                    ${acc.type} (ID: ${acc.id}) - $${acc.balance.toFixed(2)}
                </option>
            `).join('');
        }
    });
}

// Deposit handler
document.getElementById('deposit-form').addEventListener('submit', function(e) {
    e.preventDefault();
    
    const accountId = parseInt(document.getElementById('deposit-account').value);
    const amount = parseFloat(document.getElementById('deposit-amount').value);
    const description = document.getElementById('deposit-description').value || 'Deposit';
    
    const account = accounts.find(a => a.id === accountId);
    if (!account) {
        showToast('Account not found', 'error');
        return;
    }
    
    account.balance += amount;
    
    transactions.push({
        id: generateId(transactions),
        accountId,
        type: 'DEPOSIT',
        amount,
        description,
        timestamp: new Date().toISOString()
    });
    
    saveData();
    loadAccounts();
    closeModal('deposit-modal');
    showToast('Deposit successful!', 'success');
    this.reset();
});

// Withdraw handler
document.getElementById('withdraw-form').addEventListener('submit', function(e) {
    e.preventDefault();
    
    const accountId = parseInt(document.getElementById('withdraw-account').value);
    const amount = parseFloat(document.getElementById('withdraw-amount').value);
    const description = document.getElementById('withdraw-description').value || 'Withdrawal';
    
    const account = accounts.find(a => a.id === accountId);
    if (!account) {
        showToast('Account not found', 'error');
        return;
    }
    
    if (account.balance < amount && account.type !== 'CREDIT') {
        showToast('Insufficient funds', 'error');
        return;
    }
    
    account.balance -= amount;
    
    transactions.push({
        id: generateId(transactions),
        accountId,
        type: 'WITHDRAWAL',
        amount: -amount,
        description,
        timestamp: new Date().toISOString()
    });
    
    saveData();
    loadAccounts();
    closeModal('withdraw-modal');
    showToast('Withdrawal successful!', 'success');
    this.reset();
});

// Transfer handler
document.getElementById('transfer-form').addEventListener('submit', function(e) {
    e.preventDefault();
    
    const fromId = parseInt(document.getElementById('transfer-from').value);
    const toId = parseInt(document.getElementById('transfer-to').value);
    const amount = parseFloat(document.getElementById('transfer-amount').value);
    const description = document.getElementById('transfer-description').value || 'Transfer';
    
    if (fromId === toId) {
        showToast('Cannot transfer to the same account', 'error');
        return;
    }
    
    const fromAccount = accounts.find(a => a.id === fromId);
    const toAccount = accounts.find(a => a.id === toId);
    
    if (!fromAccount || !toAccount) {
        showToast('Account not found', 'error');
        return;
    }
    
    if (fromAccount.balance < amount && fromAccount.type !== 'CREDIT') {
        showToast('Insufficient funds', 'error');
        return;
    }
    
    fromAccount.balance -= amount;
    toAccount.balance += amount;
    
    transactions.push({
        id: generateId(transactions),
        accountId: fromId,
        type: 'TRANSFER_OUT',
        amount: -amount,
        description: `${description} to Account ${toId}`,
        timestamp: new Date().toISOString()
    });
    
    transactions.push({
        id: generateId(transactions),
        accountId: toId,
        type: 'TRANSFER_IN',
        amount: amount,
        description: `${description} from Account ${fromId}`,
        timestamp: new Date().toISOString()
    });
    
    saveData();
    loadAccounts();
    closeModal('transfer-modal');
    showToast('Transfer successful!', 'success');
    this.reset();
});

// Load transaction history
function loadTransactionHistory() {
    const accountId = parseInt(document.getElementById('history-account').value);
    const historyDiv = document.getElementById('transaction-history');
    
    const accountTransactions = transactions
        .filter(t => t.accountId === accountId)
        .sort((a, b) => new Date(b.timestamp) - new Date(a.timestamp))
        .slice(0, 20);
    
    if (accountTransactions.length === 0) {
        historyDiv.innerHTML = `
            <div class="empty-state">
                <h3>No transactions yet</h3>
                <p>Start making transactions to see them here</p>
            </div>
        `;
        return;
    }
    
    historyDiv.innerHTML = accountTransactions.map(tx => {
        const amountClass = tx.amount >= 0 ? 'positive' : 'negative';
        const amountSign = tx.amount >= 0 ? '+' : '';
        const date = new Date(tx.timestamp).toLocaleString();
        
        return `
            <div class="transaction-item">
                <div class="transaction-info">
                    <div class="transaction-type">${tx.type.replace('_', ' ')}</div>
                    <div class="transaction-description">${tx.description}</div>
                    <div class="transaction-date">${date}</div>
                </div>
                <div class="transaction-amount ${amountClass}">
                    ${amountSign}$${Math.abs(tx.amount).toFixed(2)}
                </div>
            </div>
        `;
    }).join('');
}

// Initialize app
if (currentUser) {
    showDashboard();
}

