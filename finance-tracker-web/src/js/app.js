// JavaScript logic for handling user authentication and dynamic content updates

document.addEventListener('DOMContentLoaded', function() {
    const loginForm = document.getElementById('login-form');
    const registerForm = document.getElementById('register-form');
    const userNameDisplay = document.getElementById('user-name');
    const accountsList = document.getElementById('accounts-list');
    const toast = document.getElementById('toast');

    loginForm.addEventListener('submit', function(event) {
        event.preventDefault();
        const email = document.getElementById('login-email').value;
        const password = document.getElementById('login-password').value;

        // Simulate login process
        if (email && password) {
            showToast('Login successful!');
            userNameDisplay.textContent = email.split('@')[0]; // Display username as part of email
            showDashboard();
        } else {
            showToast('Please fill in all fields.');
        }
    });

    registerForm.addEventListener('submit', function(event) {
        event.preventDefault();
        const name = document.getElementById('register-name').value;
        const email = document.getElementById('register-email').value;
        const password = document.getElementById('register-password').value;
        const confirmPassword = document.getElementById('register-confirm').value;

        // Simulate registration process
        if (name && email && password && confirmPassword) {
            if (password === confirmPassword) {
                showToast('Registration successful! You can now log in.');
                showTab('login');
            } else {
                showToast('Passwords do not match.');
            }
        } else {
            showToast('Please fill in all fields.');
        }
    });

    window.showTab = function(tab) {
        if (tab === 'login') {
            registerForm.style.display = 'none';
            loginForm.style.display = 'block';
            document.querySelector('.tab-btn.active').classList.remove('active');
            document.querySelector('.tab-btn:nth-child(1)').classList.add('active');
        } else {
            loginForm.style.display = 'none';
            registerForm.style.display = 'block';
            document.querySelector('.tab-btn.active').classList.remove('active');
            document.querySelector('.tab-btn:nth-child(2)').classList.add('active');
        }
    };

    window.showDashboard = function() {
        document.getElementById('auth-section').style.display = 'none';
        document.getElementById('dashboard-section').style.display = 'block';
        loadAccounts();
    };

    window.logout = function() {
        userNameDisplay.textContent = '';
        document.getElementById('auth-section').style.display = 'block';
        document.getElementById('dashboard-section').style.display = 'none';
        showToast('Logged out successfully.');
    };

    function loadAccounts() {
        // Simulate loading accounts
        accountsList.innerHTML = '<p>Account 1: $1000</p><p>Account 2: $500</p>';
    }

    function showToast(message) {
        toast.textContent = message;
        toast.classList.add('show');
        setTimeout(() => {
            toast.classList.remove('show');
        }, 3000);
    }
});