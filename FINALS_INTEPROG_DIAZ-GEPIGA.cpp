#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

// Class representing a product in the store
class Product {
public:
    string id; 
    string name; 
    double price; 
    string category;

// Constructor to initialize a product
    Product(string id, string name, double price, string category)
        : id(id), name(name), price(price), category(category) {}
};

// Struct representing an item that has been purchased
struct PurchasedItem {
    Product product;
    int quantity;
    double priceAtPurchase;
};

// Class representing an order made by a user
class Order {
public:
    long long orderNumber;
    time_t orderTime;
    string buyerName;
    string buyerPhone;
    string paymentMethod;
    vector<PurchasedItem> purchasedItems;
    double totalPrice;
    int totalQuantity;

// Constructor to initialize an order
Order(long long orderNumber,
    time_t orderTime,
    const string& buyerName,
    const string& buyerPhone,
    const string& paymentMethod,
    const vector<PurchasedItem>& purchasedItems,
    double totalPrice,
    int totalQuantity)
    : orderNumber(orderNumber),
    orderTime(orderTime),
    buyerName(buyerName),
    buyerPhone(buyerPhone),
    paymentMethod(paymentMethod),
    purchasedItems(purchasedItems),
    totalPrice(totalPrice),
    totalQuantity(totalQuantity) {}
};

// Class representing a shopping cart
class ShoppingCart {
private:
    vector<pair<Product, int>> items;

public: // Add an item to the cart
    void addItem(const Product& product, int quantity) {
        for (auto& item : items) {
            if (item.first.id == product.id) {
                item.second += quantity; // Updates quantity if product already exists
                return;
            }
        } // Add new product to cart
        items.push_back(make_pair(product, quantity));
    }

    // Remove an item from the cart by product ID
    void removeItem(const string& productId) {
        for (size_t i = 0; i < items.size(); ++i) {
            if (items[i].first.id == productId) {
                items.erase(items.begin() + i);
                return;
            }
        }
    }

    // Update the quantity of a specific item in the cart
    void updateQuantity(const string& productId, int newQuantity) {
        for (auto& item : items) {
            if (item.first.id == productId) {
                item.second = newQuantity;
                return;
            }
        }
    }

    // View the contents of the cart
    void viewCart() const {
        if (items.empty()) {
            cout << "\nYour cart is empty." << endl;
            return;
        }
        double total = 0;
        cout << "\nItems in your cart:\n" << string(30, '-') << endl;
        for (int i = items.size() - 1; i >= 0; --i) {
            cout << items[i].first.id << " - " << items[i].first.name
                 << " - Quantity: " << items[i].second
                 << " - Item Price: Php. " << fixed << setprecision(2) << items[i].first.price
                 << " - Subtotal: Php. " << fixed << setprecision(2) << items[i].first.price * items[i].second << endl;
            total += items[i].first.price * items[i].second;
        }
        cout << string(30, '-') << endl;
        cout << "Total items in cart: " << totalItems() << endl;
        cout << "Total Price: Php. " << fixed << setprecision(2) << total << endl;
    }

    // Get the total number of items in the cart
    int totalItems() const {
        int totalQuantity = 0;
        for (const auto& item : items) {
            totalQuantity += item.second;
        }
        return totalQuantity;
    }

    // Get the items in the cart
    vector<pair<Product, int>> getItems() const {
        return items;
    }

    // Clear the cart
    void clearCart() {
        items.clear();
    }

    // Check if the cart is empty
    bool isEmpty() const {
        return items.empty();
    }
    
    // Set new items for the cart
    void setItems(const vector<pair<Product, int>>& newItems) {
        items = newItems;
    }
};

// Class representing a user
class User {
public:
    string email;
    string password;
    vector<Order> purchaseHistory;
    ShoppingCart userCart;

    // Constructor to initialize a user
    User(string email = "", string password = "") : email(email), password(password) {}

    // Add an order to the user's purchase history
    void addOrder(const Order& order) {
        purchaseHistory.push_back(order);
    }
};

// Class for user authentication
class Auth {
private:
    vector<User> users;

public:
    // Constructor to initialize with a default user
    Auth() {
        users.emplace_back("alex_trisha@gmail.com", "inteprogfinals");
    }

      bool signUp(const string& email, const string& password, string* errorMessage) {
        bool hasGmail = false;
        for (size_t i = 0; i + 9 <= email.length(); ++i) {
            if (email.substr(i, 10) == "@gmail.com") {
                hasGmail = true; // Checks for valid Gmail format
                break;
            }
        }
        if (!hasGmail) {
            *errorMessage = "Invalid email format. It must be a valid gmail.";
            return false;
        }

        if (password.length() < 8) {
            *errorMessage = "Password must be at least 8 characters long.";
            return false;
        }
         for (const auto& user : users) {
            if (user.email == email) {
                *errorMessage = "User already exists.";
                return false; // Checks for existing user
            }
        }
        
        // Adds new user
        users.emplace_back(email, password);
        return true;
    }

    // Logs in a user
    User* logIn(const string& email, const string& password) {
        for (auto& user : users) {
            if (user.email == email && user.password == password) {
                return &user;
            }
        } return nullptr;
    }

    // Finds a user by email
    User* findUserByEmail(const string& email) {
        for (auto& user : users) {
            if (user.email == email) return &user;
        } return nullptr;
    }
};

// Abstract class for checkout strategy
class CheckoutStrategy {
public:
    virtual void checkout(const ShoppingCart& cart) = 0;
    virtual ~CheckoutStrategy() {}
};

// Standard checkout implementation
class StandardCheckout : public CheckoutStrategy {
public:
    void checkout(const ShoppingCart& cart) override {
        cart.viewCart();
        cout << "Proceeding to standard checkout..." << endl;

    }
};

// Main application class
class Application {
private:
    Auth auth;
    ShoppingCart currentActiveCart;
    vector<Product> products;
    User* currentUser = nullptr;

public: 
    // Constructor to initialize the application with products
    Application() {
        products.emplace_back("0001", "Canva Template Pack", 50.00, "Digital Templates");
        products.emplace_back("0002", "Resume Template", 10.00, "Digital Templates");
        products.emplace_back("0003", "Presentation Template PPT", 20.00, "Digital Templates");
        products.emplace_back("0004", "Instagram Story Templates", 8.00, "Digital Templates");
        products.emplace_back("0005", "Website Theme HTML", 120.00, "Digital Templates");

        products.emplace_back("1001", "Ebook: Learn Coding", 20.00, "Educational Content");
        products.emplace_back("1002", "Online Course: Design Basics", 50.00, "Educational Content");
        products.emplace_back("1003", "Study Guide - Math", 25.00, "Educational Content");
        products.emplace_back("1004", "Language Learning Pack", 22.00, "Educational Content");
        products.emplace_back("1005", "Printable Planner Set", 115.00, "Educational Content");

        products.emplace_back("2001", "Stock Photos Bundle", 30.00, "Creative Assets");
        products.emplace_back("2002", "Vector Graphics Pack", 28.00, "Creative Assets");
        products.emplace_back("2003", "Font Collection", 23.00, "Creative Assets");
        products.emplace_back("2004", "Icon Set", 18.00, "Creative Assets");
        products.emplace_back("2005", "Photoshop Presets", 45.00, "Creative Assets");

        products.emplace_back("3001", "Mobile App: Productivity", 40.00, "Software & Tools");
        products.emplace_back("3002", "WordPress Plugin Premium", 35.00, "Software & Tools");
        products.emplace_back("3003", "Website Builder Theme", 45.00, "Software & Tools");
        products.emplace_back("3004", "API Access Pack", 130.00, "Software & Tools");
        products.emplace_back("3005", "SDK for Developers", 180.00, "Software & Tools");

        products.emplace_back("4001", "Royalty-Free Music Pack", 30.00, "Music & Audio");
        products.emplace_back("4002", "Sound Effects Collection", 25.00, "Music & Audio");
        products.emplace_back("4003", "Audiobook: Business Success", 25.00, "Music & Audio");
        products.emplace_back("4004", "Paid Podcast Subscription", 55.00, "Music & Audio");
        products.emplace_back("4005", "Voiceover Samples", 20.00, "Music & Audio");

        products.emplace_back("5001", "Business Plan Template", 169.00, "Business & Marketing");
        products.emplace_back("5002", "Marketing Kit", 49.00, "Business & Marketing");
        products.emplace_back("5003", "Email Template Set", 18.00, "Business & Marketing");
        products.emplace_back("5004", "Logo Design Files", 45.00, "Business & Marketing");
        products.emplace_back("5005", "Brand Style Guide", 50.00, "Business & Marketing");
    }

    // Main application loop
    void run() {
        while (true) {
            cout << "\n" << string(50, '=') << endl;
            cout << "      Welcome to National Brokestore!         " << endl;
            cout << string(50, '=') << endl;

            char choice;
            cout << "Are you a registered user? (Y = Login, N = Sign Up, E = Exit): ";
            cin >> choice;
            cin.ignore();

            if (toupper(choice) == 'E') {
                cout << "Exiting program. Goodbye!" << endl;
                currentActiveCart.clearCart(); // Clear cart on exit
                break;
            }
            else if (toupper(choice) == 'Y') {
                bool loggedIn = logIn(); // Attempt to log in
                if (!loggedIn) {
                    cout << "Exiting program. Goodbye!" << endl;
                    currentActiveCart.clearCart();
                    break;
                }
                mainMenu(); // Show User Dashboard after login
            } else if (toupper(choice) == 'N') {
                bool signedUp = signUpAndLogin(); // Attempt to sign up and log in
                if (!signedUp) {
                    cout << "Exiting program. Goodbye!" << endl;
                    currentActiveCart.clearCart();
                    break;
                }
                mainMenu();
            } else {
                cout << "Invalid option. Please enter Y, N, or E." << endl;
            }
        }
    }

private: 
    // Convert string to lowercase
    string toLower(string s) {
        for (char &c : s) {
            c = tolower(c);
        } return s;
    }

    bool logIn() { // Log in a user
        string email, password;
        while (true) {
            cout << "\n=== Log In ===" << endl;
            cout << "Email (or 'exit' to quit): ";
            getline(cin,email);
            if(toLower(email) == "exit") return false;
            cout << "Password: ";
            getline(cin, password);
            cout << "==============" << endl;

            // Attempt to log in
            currentUser = auth.logIn(email, password);

            if (currentUser) {
                cout << "Login successful!" << endl;
                currentActiveCart.setItems(currentUser->userCart.getItems()); // Load user's cart
                return true;
             } else {

                cout << "Invalid email or password." << endl;
                cout << "Try again? (Y = yes, N = no/exit): ";
                char retry;
                cin >> retry;
                cin.ignore();
                if (toupper(retry) != 'Y') return false;
            }
        }
    }

    // Sign up a new user and log them in
    bool signUpAndLogin() {
        string email, password;
            string errorMessage;
        while (true) {
            cout << "\n=== Sign Up ===" << endl;
            cout << "Email (or 'exit' to quit): ";
            getline(cin,email);
            if(toLower(email) == "exit") return false;
            cout << "Password: ";
            getline(cin, password);
            cout << "==============" << endl;

            if (auth.signUp(email, password, &errorMessage)) {
                cout << "Sign up successful! Logging you in now..." << endl;
                currentUser = auth.findUserByEmail(email); // Find the newly signed-up user
                if (!currentUser) {
                    cout << "Unexpected error: user not found after sign-up." << endl;
                    return false;
                } 
                return true;
            } else {
                cout << errorMessage << endl;
                cout << "Try again? (Y = yes, N = no/exit): ";
                char retry;
                cin >> retry;
                cin.ignore();
                if (toupper(retry) != 'Y') return false;
            }
        }
    }

    // Display the User Dashboard for logged-in users
    void mainMenu() {
        int choice;
        while (true) {
           cout << "\n======= User Dashboard =======" << endl;
            cout << "1. Browse Products" << endl;
            cout << "2. Digital Shopping Cart" << endl;
            cout << "3. Purchase History" << endl;
            cout << "4. Logout" << endl;
            cout << "==============================" << endl;

            cout << "Enter your choice: ";
            while (!(cin >> choice)) {

                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(100, '\n'); 
            }
            cin.ignore(100, '\n'); 

            switch (choice) {
                case 1: browseProductsMenu(); break;
                case 2: digitalShoppingCartMenu(); break;
                case 3: purchaseHistory(); break;
                case 4:
                    cout << "Logging out..." << endl;
                    if (currentUser) {
                        currentUser->userCart.setItems(currentActiveCart.getItems()); // Save cart before logout
                        currentActiveCart.clearCart(); // Clear current cart
                        currentUser = nullptr; // Reset current user
                    } 
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    // Menu for viewing and editing the shopping cart
   void digitalShoppingCartMenu() {
        int choice;
        while (true) {
            cout << "\nViewing Cart:" << endl;
            currentActiveCart.viewCart();
            cout << "\n=== Digital Shopping Cart ===" << endl;
            cout << "1. Update Cart" << endl;
            cout << "2. Checkout Items" << endl;
            cout << "3. Back to User Dashboard" << endl;
            cout << "=============================" << endl;

            cout << "Enter your choice: ";
            while (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(100, '\n');
            }
            cin.ignore(100, '\n');

            if (choice == 1) {
                editCartOption();
            } else if (choice == 2) {
                checkoutOption();
                return;
            } else if (choice == 3) {
                return;
            } else {
                cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    // Display products in a formatted manner
    void displayProducts(const vector<Product>& productsToDisplay) {
        if (productsToDisplay.empty()) {
            cout << "No products to display." << endl;
            return;
        }

        cout << string(27, '-') << endl;

        for (const auto& product : productsToDisplay) {
            cout << product.id << " - " << product.name
                 << " - Category: " << product.category
                 << " - Price: Php. " << fixed << setprecision(2) << product.price << endl;
        }
        cout << string(27, '-') << endl;
    }

    // Menu for browsing products
    void browseProductsMenu() {
        int choice;
        while (true) {
            cout << "\n===== Browse Products =====" << endl;
            displayProducts(products);
            cout << "1. Search Products" << endl;
            cout << "2. Filter Products by Category" << endl;
            cout << "3. Add Product to Cart" << endl;
            cout << "4. Back to User Dashboard" << endl;
            cout << string(24, '=') << endl;

            cout << "Enter your choice: ";
            while (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(100, '\n');
            }
            cin.ignore(100, '\n');

            switch (choice) {
                case 1: searchProducts(); break;
                case 2: filterProducts(); break;
                case 3: addProductToCart(products); break;
                case 4: return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    // Handles user options after browsing/searching products results
    void handleProductSelectionFromResults(const vector<Product>& currentResults) {
        if (currentResults.empty()) {
            cout << "No products to select from." << endl;
            return;
        }

        int choice;
        while (true) {
            cout << "\nWhat would you like to do with these results?" << endl;
            cout << "1. Add a product to cart" << endl;
            cout << "2. Continue Browse" << endl;

            cout << "Enter your choice: ";

            while (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(100, '\n');
            }
            cin.ignore(100, '\n');

            switch (choice) {
                case 1:
                    addProductToCart(currentResults); 
                    cout << "\nCurrently viewing the same search/filter results." << endl;
                    displayProducts(currentResults);
                    break;
                case 2:
                    return; // Continue browsing
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    // Search products by name or ID
    void searchProducts() {
        cout << string(18, '=') << endl;
        cout << "Search Products:" << endl;
        cout << string(18, '=') << endl;

        cout << "Enter search term: ";
        string term;
        getline(cin, term);
        string lowerTerm = toLower(term);

        vector<Product> foundProducts;
        for (const auto& product : products) {
            string lowerName = toLower(product.name);
            bool found = false;
            for (size_t i = 0; i + lowerTerm.length() <= lowerName.length(); ++i) {
                if (lowerName.substr(i, lowerTerm.length()) == lowerTerm) {
                    found = true;
                    break;
                }
            }
            if (found) {
                foundProducts.push_back(product);
            }
        }

        if (foundProducts.empty()) {
            cout << "No products found containing: " << term << endl;
        } else {
            cout << "\nSearch Results for '" << term << "':" << endl;
            displayProducts(foundProducts);
            handleProductSelectionFromResults(foundProducts);
        }
    }

    // Filter products by category
    void filterProducts() {
        cout << string(29, '=') << endl;
        cout << "Filter Products By Category:" << endl;
        cout << string(29, '=') << endl;

        vector<string> categories;
        for (const auto& p : products) {
            bool exists = false;
            for (const string& cat : categories) {
                if (cat == p.category) {
                    exists = true;
                    break;
                }
            }
            if (!exists) {
                categories.push_back(p.category); // Collect unique categories
            }
        }

        if(categories.empty()) {
            cout << "No categories found." << endl;
            return;
        }

        for (size_t i = 0; i < categories.size(); ++i) {
            cout << i+1 << ". " << categories[i] << endl;
        }

        cout << string(29, '=') << endl;
        cout << "Choose category number: ";
        int catChoice;
        while (!(cin >> catChoice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear();
            cin.ignore(100, '\n');
        }
        cin.ignore(100, '\n');

        if (catChoice < 1 || catChoice > (int)categories.size()) {
            cout << "Invalid category choice." << endl;
            return;
        }

        string selectedCategory = categories[catChoice-1];
        vector<Product> filteredProducts;
        for (const auto& product : products) {
            if (product.category == selectedCategory) {
                 filteredProducts.push_back(product);
            }
        }

        cout << "\nProducts in category: " << selectedCategory << endl;
        displayProducts(filteredProducts);
        handleProductSelectionFromResults(filteredProducts);
    }

    // Add product to cart from a list of available products
    void addProductToCart(const vector<Product>& availableProducts) {
        cout << "\nAdd to Cart:" << endl;
        if (availableProducts.empty()) {
            cout << "No products available to add." << endl;
            return;
        }
  
        cout << "Enter product ID or name to add (or 'exit' to cancel): ";
        string input;
        getline(cin, input);
        if (toLower(input) == "exit") return;

        const Product* productPtr = nullptr;

        // Try find by ID
        for (const auto& product : availableProducts) {
            if (product.id == input) { 
                productPtr = &product; 
                break;
            }
        }

        // If not found by ID, try find by partial name
        if (!productPtr) {
            string lowerInput = toLower(input);
            for (const auto& product : availableProducts) {
                string lowerName = toLower(product.name);
                bool found = false;
                for (size_t i = 0; i + lowerInput.length() <= lowerName.length(); ++i) {
                    if (lowerName.substr(i, lowerInput.length()) == lowerInput) {
                        found = true;
                        break;
                    }
                } 
                if (found) {
                    productPtr = &product;
                    break;
                }
            }
        }

        if (!productPtr) {
            cout << "Product not found in the current list." << endl;
            return;
        }

        int quantity = 0;
        while (quantity <= 0) {
            cout << "Enter quantity: ";
            if (!(cin >> quantity) || quantity <= 0) {
                cout << "Invalid quantity. Please enter a positive number." << endl;
                cin.clear();
                cin.ignore(100, '\n');
                quantity = 0;
            }
        }
        cin.ignore(100, '\n'); 

        currentActiveCart.addItem(*productPtr, quantity);
        cout << "Successfully added " << productPtr->name << " to cart!" << endl;
    }

    // Add more products to cart, showing the full product list
    void addMoreProductToCart() {
        cout << "\nAdd more product to cart:" << endl;
        displayProducts(products);

        cout << "Enter product ID or name to add (or 'exit' to cancel): ";
        string input;
        getline(cin, input);
        if (toLower(input) == "exit") return;

        const Product* productPtr = nullptr;

        // Search by ID
        for (const auto& product : products) {
            if (product.id == input) { 
                productPtr = &product; 
                break; 
            }
        }

        // Search by partial name if not found by ID
        if (!productPtr) {
            string lowerInput = toLower(input);
            for (const auto& product : products) {
                string lowerName = toLower(product.name);
                bool found = false;
                for (size_t i = 0; i + lowerInput.length() <= lowerName.length(); ++i) {
                    if (lowerName.substr(i, lowerInput.length()) == lowerInput) {
                        found = true;
                        break;
                    }
                } 
                if (found) { 
                    productPtr = &product;
                    break;
                }
            }
        }

        if (!productPtr) {
            cout << "Product not found." << endl;
            return;
        }

        int quantity = 0;
        while (quantity <= 0) {
            cout << "Enter quantity: ";
            if (!(cin >> quantity) || quantity <= 0) {
                cout << "Invalid quantity. Please enter a positive number." << endl;
                cin.clear();
                cin.ignore(100, '\n');
                quantity = 0;
            }
        }
        cin.ignore(100, '\n');

        currentActiveCart.addItem(*productPtr, quantity);
        cout << "Successfully added " << productPtr->name << " to cart!" << endl;
    }

    // Allows editing of the current cart (add/remove/change quantity)
    void editCartOption() {
        if (currentActiveCart.isEmpty()) {
            cout << "\nYour cart is empty, nothing to edit." << endl;
            return;
        }

        cout << "\n================\nUpdate Cart:\n================" << endl;
        currentActiveCart.viewCart();

        cout << "\n=========================\nChoose an option:\n";
        cout << "1. Add more product\n";
        cout << "2. Remove product\n";
        cout << "3. Edit Quantity\n";
        cout << "4. Cancel\n";
        cout << "=========================\nEnter choice: ";

        int choice;
               
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear(); 
            cin.ignore(100, '\n');
        }
        cin.ignore(100, '\n');

        if (choice == 1) {
            addMoreProductToCart(); 
        } else if (choice == 2) {
            cout << "Enter product ID to remove: ";
            string id;
            getline(cin, id);

            auto items = currentActiveCart.getItems();
            bool exists = false;
            for (const auto& item : items) {
                if (item.first.id == id) { 
                exists = true; 
                break; 
                }
            }
            if (!exists) {
                cout << "Item not found in cart." << endl;
                return;
            }
        
        cout << "Remove item? (Y/N): ";
        char ans;
        cin >> ans;
        cin.ignore();

        if (toupper(ans) == 'Y') {
            currentActiveCart.removeItem(id);
            cout << "Successfully removed item!" << endl;
        } else {
            cout << "Removal canceled." << endl;
        }
    } else if (choice == 3) {
        cout << "Enter product ID to edit quantity: ";
        string id;
        getline(cin, id);

        auto items = currentActiveCart.getItems();
        bool exists = false;
        int currentQuantity = 0;

        for (const auto& item : items) {
            if (item.first.id == id) { 
                exists = true; 
                currentQuantity = item.second; 
                break; 
            }
        }

        if (!exists) {
            cout << "Item not found in cart." << endl;
            return;
        }

           cout << "Current quantity: " << currentQuantity << endl;

            int newQuantity = 0;
            while (newQuantity <= 0) {
                cout << "Enter new quantity: ";
                if (!(cin >> newQuantity) || newQuantity <= 0) {
                    cout << "Invalid quantity. Please enter a positive number." << endl;
                    cin.clear();
                    cin.ignore(100, '\n');
                    newQuantity = 0; 
                }
            }
            cin.ignore(100, '\n');

            currentActiveCart.updateQuantity(id, newQuantity);
            cout << "Successfully adjusted quantity of item!" << endl;
        } else if (choice == 4) {
            cout << "Update cancelled." << endl;
            return;
        } else {
            cout << "Invalid choice." << endl;
        }
    }

    // Checkout flow implementation
    void checkoutOption() {
        if (currentActiveCart.isEmpty()) {
            cout << "\nYour cart is empty. Cannot proceed to checkout." << endl;
            return;
        }

        cout << string(12, '=') << "\nCheckout:\n" << string(12, '=') << endl;
        currentActiveCart.viewCart();

        cout << "Continue checkout? (Y/N): ";
        char cont;
        cin >> cont;
        cin.ignore();

        if (toupper(cont) != 'Y') {
            cout << "Checkout cancelled." << endl;
            return;
        }

        // Collect buyer details
        string name, phone;
        cout << string(30, '=') << endl;
        cout << "Enter your details to complete purchase:" << endl;
        cout << "Name: ";
        getline(cin, name);

        // Validate phone number: digits only, 8 to 15 characters
        while (true) {
            cout << "Phone number: ";
            getline(cin, phone);

           bool isValidPhone = (phone.length() >= 8 && phone.length() <= 15);
            for (char c : phone) {
                if (!isdigit(c)) { 
                    isValidPhone = false;
                    break;
                }
            }
            if (isValidPhone) {
                break; 
            } else {
                cout << "Invalid phone number. Please enter digits only, minimum 8 and maximum 15 characters." << endl;
            }
        }

        // Payment details collection
        string paymentMethod;
        string cardType;
        string cardNumber;
        string cardHolder;
        string ccv;

        // Select payment method
        while (true) {
            cout << string(30, '-') << endl;
            cout << "Select payment method:\n1. Debit Card\n2. Credit Card\nChoose (1 or 2): ";
            string pmChoice;
            getline(cin, pmChoice);

            if (pmChoice == "1") {
                cardType = "Debit card";
                break;
            } else if (pmChoice == "2") {
                cardType = "Credit card";
                break;
            } else {
                cout << "Invalid choice. Please enter 1 or 2." << endl;
            }
        }

        // Validate card number: digits only, length 13-19
        while (true) {
            cout << string(30, '-') << endl;
            cout << cardType << " number: ";
            getline(cin, cardNumber);
            bool valid = (cardNumber.length() >= 13 && cardNumber.length() <= 19);
            for (char c : cardNumber) {
                if (!isdigit(c)) {
                    valid = false;
                    break;
                }
            }
            if (valid) break;
            cout << "Invalid card number. Please enter digits only, between 13 and 19 characters." << endl;
        }

        // Collect cardholder name
        cout << "Card holder name: ";
        getline(cin, cardHolder);

        // Validate CCV: 3 or 4 digits
        while (true) {
            cout << "CCV: ";
            getline(cin, ccv);
            bool valid = (ccv.length() == 3 || ccv.length() == 4);
            for (char c : ccv) {
                if (!isdigit(c)) {
                    valid = false;
                    break;
                }
            }
            if (valid) break;
            cout << "Invalid CCV. Please enter 3 or 4 digit number." << endl;
        }

        // Validate expiration date MM/YY format and logical checks
        string expDate;
        while (true) {
            cout << "Expiration date (MM/YY): ";
            getline(cin, expDate);

            // Check basic format: must be 5 characters with '/' at position 2 (e.g., "06/25")
            if (expDate.length() == 5 && expDate[2] == '/') {
                string monthStr = expDate.substr(0, 2); // Extract month part
                string yearStr = expDate.substr(3, 2); // Extract year part (last two digits)
                int month = 0;
                int year = 0;

                try {
                    // Convert strings to integers
                    month = stoi(monthStr);
                    year = stoi(yearStr) + 2000; // Convert to full year (e.g., "25" -> 2025)

                } catch (...) {
                    cout << "Invalid numbers in expiration date." << endl;
                    continue;
                }

                // Check that month is within valid range
                if (month >= 1 && month <= 12) {

                    // Get current year for validation
                    time_t now = time(0);
                    tm* ltm = localtime(&now);
                    int currentYear = 1900 + ltm->tm_year;
                    if (year >= currentYear) {

                        break;
                    } else {
                        cout << "Invalid expiration year. Please enter a valid year." << endl;
                        }
                    } else {
                        cout << "Invalid month. Please enter a month between 01 and 12." << endl;
                        }
                    } else {
                        cout << "Invalid format. Please enter in MM/YY format." << endl;
                        }
                    }

        // Prepare purchase details for order
        vector<PurchasedItem> purchasedItems;
        double totalPrice = 0;
        int totalQuantity = 0;

        for (const auto& item : currentActiveCart.getItems()) {
            purchasedItems.push_back({item.first, item.second, item.first.price});
            totalPrice += item.first.price * item.second;
            totalQuantity += item.second;
        }

        // Generate order number (using current time)
        time_t now = time(0);
        long long orderNumber = static_cast<long long>(now);

        // Format payment method display string
        string paymentMethodDisplay = cardType + " ending in ";
        if (cardNumber.length() >= 4) {
            paymentMethodDisplay += cardNumber.substr(cardNumber.length() - 4);
        } else {
            paymentMethodDisplay += cardNumber;
        }

        // Create new order and add it to user's purchase history
        Order newOrder(orderNumber, now, name, phone, paymentMethodDisplay, purchasedItems, totalPrice, totalQuantity);
        if (currentUser)
            currentUser->addOrder(newOrder);

        printReceipt(newOrder);
        currentActiveCart.clearCart();

        cout << "Your order was successfully placed! You can now proceed to download your items." << endl;
        cout << "If you want to view and download your purchases, just visit your Purchase History page." << endl;
        cout << "Thank you for shopping with us! :)" << endl;
    }

        // Print a detailed receipt for an order
        void printReceipt(const Order& order) {
            cout << "\n" << string(42, '=') << endl;
            cout << "                RECEIPT                  " << endl;
            cout << string(42, '=') << endl;

            tm* ltm = localtime(&(order.orderTime));
            cout << "Purchase Date:  " << 1900 + ltm->tm_year << "/" 
                << 1 + ltm->tm_mon << "/" << ltm->tm_mday << endl;
            cout << "Purchase Time:  " << setw(2) << setfill('0') << ltm->tm_hour << ":"
                << setw(2) << setfill('0') << ltm->tm_min << ":"
                << setw(2) << setfill('0') << ltm->tm_sec << endl;
            cout << string(42, '-') << endl;

            cout << "Buyer Contact Details:\n";
            cout << "Name:  " << order.buyerName << endl;
            cout << "Phone: " << order.buyerPhone << endl;
            cout << string(42, '-') << endl;
            cout << "Payment Method: " << order.paymentMethod << endl;
            cout << string(42, '-') << endl;
            cout << "Order Number: " << order.orderNumber << endl;
        
            cout << "\nPurchased Digital Products:\n" << string(18,'-') << endl;

            for (int i = order.purchasedItems.size() - 1; i >= 0; --i) {
                const PurchasedItem& pi = order.purchasedItems[i];
                cout << "[Download Here] " << pi.product.id << " - " << pi.product.name
                    << " - Quantity: " << pi.quantity
                    << " - Price: Php. " << fixed << setprecision(2) << pi.priceAtPurchase * pi.quantity << endl;
    }

            cout << string(18, '-') << endl;
            cout << "Total Items: " << order.totalQuantity << endl;
            cout << "Total Price: Php. " << fixed << setprecision(2) << order.totalPrice << endl;
            cout << string(42, '=') << "\n" << endl;
    }

        // Display purchase history for the current user
        void purchaseHistory() {
            if (!currentUser) {
                cout << "\nNo user logged in. Cannot display purchase history." << endl;
                return;
        }
            cout << "\nPurchase History for " << currentUser->email << ":" << endl;
            if (currentUser->purchaseHistory.empty()) {
            cout << "No purchase history found." << endl;
            return;
        }

        // Print receipts for each order in reverse chronological order
        for (int i = currentUser->purchaseHistory.size() - 1; i >= 0; --i) {
            printReceipt(currentUser->purchaseHistory[i]);
        }
    }
};

// Main entry point of the program
int main() {
    Application app;
    app.run();
    return 0;
}