#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <map>

using namespace std;

// Product class
class Product {
public:
    string id;
    string name;
    double price;
    string category;

    Product(string id, string name, double price, string category)
        : id(id), name(name), price(price), category(category) {}
};

// Simple struct to store purchased product info with quantity and price at purchase time
struct PurchasedItem {
    Product product;
    int quantity;
    double priceAtPurchase;
};

// Order class, contains order details and purchased items
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

// ShoppingCart class
class ShoppingCart {
private:
    vector<pair<Product, int>> items; // Product and quantity pairs

public:
    void addItem(const Product& product, int quantity) {
        for (auto& item : items) {
            if (item.first.id == product.id) {
                item.second += quantity;
                return;
            }
        }
        items.push_back(make_pair(product, quantity));
    }

    void removeItem(const string& productId) {
        items.erase(remove_if(items.begin(), items.end(),
            [&productId](const pair<Product, int>& item) {
                return item.first.id == productId;
            }), items.end());
    }

    void updateQuantity(const string& productId, int newQuantity) {
        for (auto& item : items) {
            if (item.first.id == productId) {
                item.second = newQuantity;
                return;
            }
        }
    }

    void viewCart() const {
        if (items.empty()) {
            cout << "Your cart is empty." << endl;
            return;
        }
        double total = 0;
        cout << "Items in your cart:\n-------------------" << endl;
        for (auto it = items.rbegin(); it != items.rend(); ++it) {
                                    
            cout << it->first.id << " - " << it->first.name
                 << " - Quantity: " << it->second
                 << " - Item Price: Php. " << fixed << setprecision(2) << it->first.price
                 << " - Subtotal: Php. " << fixed << setprecision(2) << it->first.price * it->second << endl;
            total += it->first.price * it->second;
        }
        cout << "-------------------" << endl;
        cout << "Total items in cart: " << totalItems() << endl;
        cout << "Total Price: Php. " << fixed << setprecision(2) << total << endl;
    }

    int totalItems() const {
        int totalQuantity = 0;
        for (const auto& item : items) {
            totalQuantity += item.second;
        }
        return totalQuantity;
    }

    vector<pair<Product, int>> getItems() const {
        return items;
    }

    void clearCart() {
        items.clear();
    }

    bool isEmpty() const {
        return items.empty();
    }
      
// Setter for items (used when loading cart for a user)
 void setItems(const vector<pair<Product, int>>& newItems) {
        items = newItems;
    }
};

// User class, now holds purchase history vector of Orders and a ShoppingCart
class User {
public:
    string email;
    string password;
    vector<Order> purchaseHistory;
    ShoppingCart userCart; // Each user now has their own cart

    User(string email = "", string password = "") : email(email), password(password) {}

    void addOrder(const Order& order) {
        purchaseHistory.push_back(order);
    }
};

// Authentication class
class Auth {
private:
    vector<User> users;

public:
    Auth() {
        // Adding a test user for testing
        users.emplace_back("alex_trisha@gmail.com", "inteprogfinals");
    }

      bool signUp(const string& email, const string& password, string* errorMessage) {
        // Email validation. Must contain "@gmail.com"
        if (email.find("@gmail.com") >= email.length()) {
            *errorMessage = "Invalid email format. It must be a valid gmail.";
            return false;
        }

        // Password validation: Must be at least 8 characters long
        if (password.length() < 8) {
            *errorMessage = "Password must be at least 8 characters long.";
            return false;
        }
    for (const auto& user : users) {
            if (user.email == email) {
                *errorMessage = "User already exists.";

                return false;
            }
        }
        users.emplace_back(email, password);
        return true; // Sign-up successful
    }

    User* logIn(const string& email, const string& password) {

        for (auto& user : users) {
            if (user.email == email && user.password == password) {
                return &user;
            }
        }
        return nullptr; // Login failed
    }

    User* findUserByEmail(const string& email) {
        for (auto& user : users) {
            if (user.email == email) return &user;
        }
        return nullptr;
    }
};

class CheckoutStrategy {
public:
    virtual void checkout(const ShoppingCart& cart) = 0;
    virtual ~CheckoutStrategy() {}
};

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
    ShoppingCart currentActiveCart; // This will hold the cart of the currently logged-in user
    vector<Product> products;
    User* currentUser = nullptr; // Pointer to the currently logged-in user

public:
    Application() {
        // Digital product categories and items
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

    void run() {
        while (true) {
            cout << "==============================================" << endl;
            cout << "      Welcome to National Brokestore!         " << endl;
            cout << "==============================================" << endl;

            char choice;
            cout << "Are you a registered user? (Y = Login, N = Sign Up, E = Exit): ";
            cin >> choice;
            cin.ignore();

            if (toupper(choice) == 'E') {
                cout << "Exiting program. Goodbye!" << endl;
                currentActiveCart.clearCart(); // Clear the cart when the program exits
                break;
            }
            else if (toupper(choice) == 'Y') {
                bool loggedIn = logIn();
                if (!loggedIn) {
                    cout << "Exiting program. Goodbye!" << endl;
                    currentActiveCart.clearCart();  // Clear the cart if login process is exited
                    break;
                }
                mainMenu();

            /* When logging out from mainMenu, the cart is saved back to currentUser->userCart and currentUser is set to nullptr. The currentActiveCart is effectively cleared for the next login (as it will be loaded from the new currentUser's userCart). */
            }
            else if (toupper(choice) == 'N') {
                bool signedUp = signUpAndLogin();
                if (!signedUp) {
                    cout << "Exiting program. Goodbye!" << endl;
                  
                    currentActiveCart.clearCart(); // Clear the cart if signup process is exited
                    break;
                }
                mainMenu();
            // Same logic as above for logging out.
            }
            else {
                cout << "Invalid option. Please enter Y, N, or E." << endl;
            }
        }
    }

private:
    bool logIn() {
        string email, password;
        while (true) {
            cout << "\n=== Log In ===" << endl;
            cout << "Email (or 'exit' to quit): ";
            getline(cin,email);
            if(email == "exit" || email == "EXIT") return false;
            cout << "Password: ";
            getline(cin,password);

            currentUser = auth.logIn(email, password);

            if (currentUser) {
                cout << "Login successful!" << endl;
            // Load the user's saved cart into the active cart
                currentActiveCart.setItems(currentUser->userCart.getItems());
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

    bool signUpAndLogin() {
        string email, password;
            string errorMessage;
        while (true) {
            cout << "\n=== Sign Up ===" << endl;
            cout << "Email (or 'exit' to quit): ";
            getline(cin,email);
            if(email == "exit" || email == "EXIT") return false;
            cout << "Password: ";
            getline(cin,password);

            if (auth.signUp(email, password, &errorMessage)) {
                cout << "Sign up successful! Logging you in now..." << endl;
                currentUser = auth.findUserByEmail(email);
                if (!currentUser) {
                    cout << "Unexpected error: user not found after sign-up." << endl;
                    return false;
                }
            // For a new user, their cart will be empty, so no explicit loading needed.
                return true;
            } 

            else {
                cout << errorMessage << endl; // Displays the error message
                cout << "Try again? (Y = yes, N = no/exit): ";
                char retry;
                cin >> retry;
                cin.ignore();
                if (toupper(retry) != 'Y') return false;
            }
        }
    }

    void mainMenu() {
        int choice;
        while (true) {
            cout << "\n======= Main Menu =======\n";
            cout << "1. Browse Products\n2. View Cart\n3. Update Cart\n4. Checkout\n5. Purchase History\n6. Logout\n"; // Fixed numbering
            cout << "==========================\n";
            cout << "Enter your choice: ";
            while (!(cin >> choice)) {

                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(100, '\n'); 
            }
            cin.ignore(100, '\n'); 

            switch (choice) {
                case 1: browseProductsMenu(); break;
                case 2: viewCartOption(); break; // Corrected case
                case 3: editCartOption(); break; // Corrected case
                case 4: checkoutOption(); break; // Corrected case
                case 5: purchaseHistory(); break; // Corrected case
                case 6: // Logout - Corrected case
                    cout << "Logging out..." << endl;
                    if (currentUser) {
                        // Save the current active cart to the user's cart before logging out
                        currentUser->userCart.setItems(currentActiveCart.getItems());
                        currentActiveCart.clearCart(); // Clear the active cart for the next user
                        currentUser = nullptr; // No user is logged in now
                    }
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

   // Helper function to display products
    void displayProducts(const vector<Product>& productsToDisplay) {
        if (productsToDisplay.empty()) {
            cout << "No products to display." << endl;
            return;
        }

        cout << "\n------------------" << endl;

        for (const auto& product : productsToDisplay) {
            cout << product.id << " - " << product.name
                 << " - Category: " << product.category
                 << " - Price: Php. " << fixed << setprecision(2) << product.price << endl;
        }
        cout << "------------------" << endl;

    }
    void browseProductsMenu() {
        int choice;
        while (true) {
            cout << "\n===== Browse Products =====" << endl;
            displayProducts(products); // Display all products initially
            cout << "1. Search Products\n2. Filter Products by Category\n3. Add Product to Cart\n4. Back to Main Menu\n";
            cout << "===========================\n";
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
                case 3: addProductToCart(products); break; // Pass all products
                case 4: return; // Go back to main menu
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void handleProductSelectionFromResults(const vector<Product>& currentResults) {
        if (currentResults.empty()) {
            cout << "No products to select from." << endl;
            return;
        }

        int choice;
        while (true) {
            cout << "\nWhat would you like to do with these results?\n";
            cout << "1. Add a product to cart\n2. Continue Browse\n";
            cout << "Enter your choice: ";

            while (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number: ";
                cin.clear();
                cin.ignore(100, '\n');
            }

            cin.ignore(100, '\n');

            switch (choice) {
                case 1:
                    addProductToCart(currentResults); // Pass the current search/filter results
                    // Stay on the same results after adding to cart
                    cout << "\nCurrently viewing the same search/filter results." << endl;
                    displayProducts(currentResults);
                    break;

                case 2:
                    return; // Go back to the browse products menu
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void searchProducts() {
        cout << "\nSearch Products:\n------------------" << endl;
        cout << "Enter search term: ";
        string term;
        getline(cin, term);
        transform(term.begin(), term.end(), term.begin(), ::tolower);

        vector<Product> foundProducts;
        for (const auto& product : products) {
            string nameLower = product.name;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
           if (nameLower.find(term) < nameLower.length()) { // Using length() instead of npos
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

    void filterProducts() {
        cout << "\nFilter Products By Category:" << endl;
        vector<string> categories;
        for (const auto& p : products) {
            if (find(categories.begin(), categories.end(), p.category) == categories.end()) {
                categories.push_back(p.category);
            }
        }

        for (size_t i = 0; i < categories.size(); ++i) {
            cout << i+1 << ". " << categories[i] << endl;
        }

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

    void viewCartOption() {
        cout << "\nViewing Cart:" << endl;
        currentActiveCart.viewCart();
    }

    void addProductToCart(const vector<Product>& availableProducts) {
        cout << "\nAdd to Cart:" << endl;
        if (availableProducts.empty()) {
            cout << "No products available to add." << endl;
            return;
        }
  
        cout << "Enter product ID or name to add (or 'exit' to cancel): ";
        string input;
        getline(cin, input);
        if (input == "exit") return;

        const Product* productPtr = nullptr; // Changed to const Product*
        for (const auto& product : availableProducts) { // Search only within the provided list
            if (product.id == input) { productPtr = &product; break; }
        }
        if (!productPtr) {
            string inputLower = input;
            transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);
            for (const auto& product : availableProducts) { // Search only within the provided list
                string nameLower = product.name;
                transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
                if (nameLower.find(inputLower) < nameLower.length()) {
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

    void addMoreProductToCart() {
        cout << "\nAdd more product to cart:" << endl;
        displayProducts(products); // Display all products for this option
        cout << "Enter product ID or name to add (or 'exit' to cancel): ";
        string input;
        getline(cin, input);
        if (input == "exit") return;
        const Product* productPtr = nullptr; // Changed to const Product*
        for (const auto& product : products) { // Search all products
            if (product.id == input) { productPtr = &product; break; }
        }
        if (!productPtr) {
            string inputLower = input;
            transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);
            
            for (const auto& product : products) { // Search all products
                string nameLower = product.name;
                transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

                if (nameLower.find(inputLower) < nameLower.length()) { 
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
                cin.ignore(100, '\n'); // Discard invalid input
                quantity = 0;
            }
        }

        cin.ignore(100, '\n');

        currentActiveCart.addItem(*productPtr, quantity);
        cout << "Successfully added " << productPtr->name << " to cart!" << endl;
    }

    void editCartOption() {
        if (currentActiveCart.isEmpty()) {
            cout << "\nYour cart is empty, nothing to edit." << endl;
            return;
        }

        cout << "\nUpdate Cart:\n------------" << endl;
        currentActiveCart.viewCart();

        cout << "Choose an option: 1. Add more product 2. Remove product 3. Edit Quantity 4. Cancel\nEnter choice: ";
        int choice;
               
        while (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number: ";
            cin.clear(); 
            cin.ignore(100, '\n');
        }
        cin.ignore(100, '\n');

        if (choice == 1) {
            addMoreProductToCart(); }
        else if (choice == 2) {
            cout << "Enter product ID to remove: ";
            string id;
            getline(cin, id);

            auto items = currentActiveCart.getItems();
            bool exists = false;
            for (const auto& item : items) {
                if (item.first.id == id) { exists = true; break; }
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
        }
        else if (choice == 3) {
            cout << "Enter product ID to edit quantity: ";
            string id;
            getline(cin, id);

            auto items = currentActiveCart.getItems();
            bool exists = false;
            int currentQuantity = 0;
            for (const auto& item : items) {
                if (item.first.id == id) { exists = true; currentQuantity = item.second; break; }
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
        }
        else if (choice == 4) {
            cout << "Update cancelled." << endl;
            return;
        }
        else {
            cout << "Invalid choice." << endl;
        }
    }

    void checkoutOption() {
        if (currentActiveCart.isEmpty()) {
            cout << "\nYour cart is empty. Cannot proceed to checkout." << endl;
            return;
        }

        cout << "\nCheckout:\n------------------" << endl;
        currentActiveCart.viewCart();

        cout << "Continue checkout? (Y/N): ";
        char cont;
        cin >> cont;
        cin.ignore();

        if (toupper(cont) != 'Y') {
            cout << "Checkout cancelled." << endl;
            return;
        }

        string name, phone;
        cout << "Enter your details to complete purchase:" << endl;
        cout << "Name: ";
        getline(cin, name);
                while (true) {

            cout << "Phone number: ";

            getline(cin, phone);

            bool isValidPhone = true;

            for (char c : phone) {

                if (!isdigit(c)) { // Checks if the character is not a digit
                    isValidPhone = false;
                    break;
                }
            }

            if (isValidPhone) {
                break; // Exit loop if phone number is valid
            } else {
                cout << "Invalid phone number. Please enter digits only." << endl;
            }
        }

        string paymentMethod;
        while (true) {
            cout << "Select payment method (Credit only): ";
            getline(cin, paymentMethod);
            transform(paymentMethod.begin(), paymentMethod.end(), paymentMethod.begin(), ::tolower);
            if (paymentMethod == "credit") {
                paymentMethod = "Credit";
                break;
            } else {
                cout << "Invalid payment method. Only 'Credit' is accepted for digital products." << endl;
            }
        }

        vector<PurchasedItem> purchasedItems;
        double totalPrice = 0;
        int totalQuantity = 0;

        for (const auto& item : currentActiveCart.getItems()) {
            purchasedItems.push_back({item.first, item.second, item.first.price});
            totalPrice += item.first.price * item.second;
            totalQuantity += item.second;
        }

        time_t now = time(0);
        long long orderNumber = static_cast<long long>(now);

        Order newOrder(orderNumber, now, name, phone, paymentMethod, purchasedItems, totalPrice, totalQuantity);

        if(currentUser)
            currentUser->addOrder(newOrder);

        printReceipt(newOrder);
        currentActiveCart.clearCart();  // Clear the active cart after successful checkout
        cout << "Successfully placed your order! You can now download your digital products on your Purchased History." << endl;
    }

    void printReceipt(const Order& order) {
        cout << "\n==========================================" << endl;
        cout << "                 RECEIPT                  " << endl;
        cout << "==========================================" << endl;

        tm* ltm = localtime(&(order.orderTime));
        cout << "Purchase Date:  " << 1900 + ltm->tm_year << "/" 
             << 1 + ltm->tm_mon << "/" << ltm->tm_mday << endl;
        cout << "Purchase Time:  " << setw(2) << setfill('0') << ltm->tm_hour << ":"
             << setw(2) << setfill('0') << ltm->tm_min << ":"
             << setw(2) << setfill('0') << ltm->tm_sec << endl;
        cout << "------------------------------------------" << endl;

        cout << "Buyer Contact Details:\n";
        cout << "Name:  " << order.buyerName << endl;
        cout << "Phone: " << order.buyerPhone << endl;
        cout << "------------------------------------------" << endl;
        cout << "Payment Method: " << order.paymentMethod << endl;
        cout << "------------------------------------------" << endl;
        cout << "Order Number: " << order.orderNumber << endl;
       
        cout << "\nPurchased Digital Products:\n------------------" << endl;
        for (auto it = order.purchasedItems.rbegin(); it != order.purchasedItems.rend(); ++it) {
            const PurchasedItem& pi = *it;
            cout << "[Download Here] " << pi.product.id << " - " << pi.product.name
             << " - Quantity: " << pi.quantity
             << " - Price: Php. " << fixed << setprecision(2) << pi.priceAtPurchase * pi.quantity << endl;
    }

        cout << "------------------" << endl;
        cout << "Total Items: " << order.totalQuantity << endl;
        cout << "Total Price: Php. " << fixed << setprecision(2) << order.totalPrice << endl;
        cout << "==========================================\n" << endl;
    }

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
        cout << "==========================================" << endl;
        for (const auto& order : currentUser->purchaseHistory) {
            printReceipt(order);
        }
    }
};

int main() {
    Application app;
    app.run();
    return 0;
}