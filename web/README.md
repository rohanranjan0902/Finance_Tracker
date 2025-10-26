# FinTrack Web Interface

A modern, responsive web frontend for FinTrack Personal Finance Manager.

## Features

✨ **User Management**
- Create new user accounts
- Login with email/password
- Secure logout

💳 **Account Management**
- Create multiple accounts (Savings, Checking, Credit, Investment)
- View all accounts with real-time balances
- Beautiful card-based interface

💰 **Transactions**
- Deposit money
- Withdraw money
- Transfer between accounts
- Optional transaction descriptions
- Input validation

📊 **Transaction History**
- View transaction history for any account
- See last 20 transactions
- Color-coded amounts (green for positive, red for negative)
- Detailed transaction information with timestamps

🎨 **Modern UI/UX**
- Clean, professional design
- Responsive layout (works on mobile, tablet, desktop)
- Smooth animations and transitions
- Toast notifications for all actions
- Modal dialogs for data entry

## How to Use

### 1. Opening the Application

Simply open `index.html` in any modern web browser:

**Option A: Double-click**
- Double-click on `index.html`

**Option B: From Browser**
- Open your browser
- Press `Ctrl+O` (Windows/Linux) or `Cmd+O` (Mac)
- Navigate to the `web` folder and select `index.html`

**Option C: Local Server (Recommended)**
```bash
# If you have Python installed:
cd web
python -m http.server 8000

# Then open: http://localhost:8000
```

### 2. Creating Your First Account

1. Click on "Register" tab
2. Enter your full name
3. Enter a valid email address
4. Create a password
5. Confirm your password
6. Click "Create Account"

### 3. Logging In

1. Enter your email
2. Enter your password
3. Click "Login"

### 4. Creating a Financial Account

1. After logging in, click "+ New Account"
2. Select account type
3. Enter initial balance
4. Click "Create Account"

### 5. Making Transactions

**Deposit:**
1. Click the "Deposit" button
2. Select the account
3. Enter amount
4. Add description (optional)
5. Click "Deposit"

**Withdraw:**
1. Click the "Withdraw" button
2. Select the account
3. Enter amount
4. Add description (optional)
5. Click "Withdraw"

**Transfer:**
1. Click the "Transfer" button
2. Select "From" account
3. Select "To" account
4. Enter amount
5. Add description (optional)
6. Click "Transfer"

### 6. Viewing History

1. Click the "History" button
2. Select an account from the dropdown
3. View your transaction history

## Data Storage

The application uses **browser localStorage** to persist your data:

✅ **Advantages:**
- No server required
- Works completely offline
- Instant performance
- Private (data stays on your device)

⚠️ **Important Notes:**
- Data is stored in your browser
- Clearing browser data will delete your financial records
- Data is not synced across devices
- Not shared between different browsers

### Backing Up Your Data

To backup your data:

1. Open Browser DevTools (`F12`)
2. Go to "Console" tab
3. Type: `localStorage`
4. Copy the data shown
5. Save it to a text file

To restore:
1. Open Browser DevTools
2. Go to "Console" tab
3. Paste your saved data
4. Refresh the page

## Browser Compatibility

✅ **Supported Browsers:**
- Google Chrome (recommended)
- Microsoft Edge
- Mozilla Firefox
- Safari
- Opera

**Minimum Requirements:**
- JavaScript enabled
- localStorage support
- CSS3 support

## Features in Detail

### Account Types

- **Savings**: For long-term savings
- **Checking**: For daily transactions
- **Credit**: Credit card accounts (can go negative)
- **Investment**: Investment portfolios

### Security Features

- Password hashing (basic)
- Form validation
- Input sanitization
- Error handling

### Error Handling

The application provides clear error messages for:
- Invalid login credentials
- Insufficient funds
- Invalid transfers
- Duplicate email addresses
- Password mismatches

## Customization

### Changing Colors

Edit `style.css` and modify the CSS variables:

```css
:root {
    --primary-color: #4f46e5;  /* Change this */
    --primary-dark: #4338ca;   /* And this */
}
```

### Adding New Account Types

Edit `index.html` line 122-127 to add more options:

```html
<option value="NEWTYPE">New Type</option>
```

## Troubleshooting

### Data Not Saving

- Check if browser allows localStorage
- Ensure you're not in Incognito/Private mode
- Check browser settings for storage permissions

### Page Not Loading

- Ensure all three files are in the same folder:
  - `index.html`
  - `style.css`
  - `app.js`
- Check browser console for errors (`F12`)

### Transactions Not Working

- Ensure you're logged in
- Check that you have accounts created
- Verify sufficient funds for withdrawals/transfers

## Development

### File Structure

```
web/
├── index.html    # Main HTML structure
├── style.css     # All styling and animations
├── app.js        # Application logic
└── README.md     # This file
```

### Technologies Used

- **HTML5**: Structure and forms
- **CSS3**: Styling, animations, responsive design
- **Vanilla JavaScript**: No frameworks, pure JS
- **localStorage API**: Data persistence

### Extending the Application

Want to add features? Here's where to start:

**Add a new transaction type:**
1. Add button in HTML (`index.html`)
2. Create modal for the form
3. Add event handler in `app.js`
4. Update `saveData()` function

**Add charts/graphs:**
```javascript
// Consider using Chart.js or similar library
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
```

**Add export functionality:**
```javascript
function exportData() {
    const data = {
        users, accounts, transactions
    };
    const blob = new Blob([JSON.stringify(data, null, 2)], 
        {type: 'application/json'});
    // Create download link
}
```

## Security Notes

⚠️ **For Production Use:**

This is a client-side demo application. For real-world use:

1. **Use proper backend**: Move data storage to server
2. **Real authentication**: Use OAuth or JWT
3. **Password encryption**: Use bcrypt or similar
4. **HTTPS**: Always use secure connections
5. **Input validation**: Server-side validation
6. **SQL injection protection**: Use prepared statements
7. **CSRF protection**: Add CSRF tokens

## License

This web interface is part of the FinTrack project.

## Support

For issues or questions:
1. Check this README
2. Review the code comments
3. Open browser console for errors
4. Check localStorage data

---

**Enjoy managing your finances with FinTrack!** 🏦💰


