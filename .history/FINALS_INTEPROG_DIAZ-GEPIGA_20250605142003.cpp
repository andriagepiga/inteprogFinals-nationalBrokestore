#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <stdexcept>
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

// User class, now holds purchase history vector of Orders
class User {
public:
    string email;
    string password;
    vector<Order> purchaseHistory;

    User(string email = "", string password = "") : email(email), password(password) {}

    void addOrder(const Order& order) {
        purchaseHistory.push_back(order);
    }
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
                 << " - Price: $" << fixed << setprecision(2) << it->first.price * it->second << endl;
            total += it->first.price * it->second;
        }
        cout << "-------------------" << endl;
        cout << "Total items: " << totalItems() << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << total << endl;
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
};

// Exception class for user-related errors
class UserException : public runtime_error {
public:
    UserException(const string& message) : runtime_error(message) {}
};

// Authentication class
class Auth {
private:
    vector<User> users;

public:
    Auth() {
        // Adding a test user for testing
        users.emplace_back("testuser@example.com", "password123");
    }

    void signUp(const string& email, const string& password) {
        for (const auto& user : users) {
            if (user.email == email) {
                throw UserException("User already exists.");
            }
        }
        users.emplace_back(email, password);
    }

    User& logIn(const string& email, const string& password) {
        for (auto& user : users) {
            if (user.email == email && user.password == password) {
                return user;
            }
        }
        throw UserException("Invalid email or password.");
    }

    User* findUserByEmail(const string& email) {
        for (auto& user : users) {
            if (user.email == email) return &user;
        }
        return nullptr;
    }
};

// Strategy Design Pattern for Checkout
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
        // Additional checkout logic can be added here
    }
};

// Main application class
class Application {
private:
    Auth auth;
    ShoppingCart cart;
    vector<Product> products;
    User* currentUser = nullptr;

public:
    Application() {
        // Sample products - digital product categories and items
        products.emplace_back("0001", "Canva Template Pack", 15.00, "Digital Templates");
        products.emplace_back("0002", "Resume Template", 10.00, "Digital Templates");
        products.emplace_back("0003", "Presentation Template PPT", 12.00, "Digital Templates");
        products.emplace_back("0004", "Instagram Story Templates", 8.00, "Digital Templates");
        products.emplace_back("0005", "Website Theme HTML", 25.00, "Digital Templates");

        products.emplace_back("1001", "Ebook: Learn Coding", 20.00, "Educational Content");
        products.emplace_back("1002", "Online Course: Design Basics", 50.00, "Educational Content");
        products.emplace_back("1003", "Study Guide - Math", 15.00, "Educational Content");
        products.emplace_back("1004", "Language Learning Pack", 22.00, "Educational Content");
        products.emplace_back("1005", "Printable Planner Set", 9.00, "Educational Content");

        products.emplace_back("2001", "Stock Photos Bundle", 30.00, "Creative Assets");
        products.emplace_back("2002", "Vector Graphics Pack", 18.00, "Creative Assets");
        products.emplace_back("2003", "Font Collection", 25.00, "Creative Assets");
        products.emplace_back("2004", "Icon Set", 12.00, "Creative Assets");
        products.emplace_back("2005", "Photoshop Presets", 20.00, "Creative Assets");

        products.emplace_back("3001", "Mobile App: Productivity", 40.00, "Software & Tools");
        products.emplace_back("3002", "WordPress Plugin Premium", 35.00, "Software & Tools");
        products.emplace_back("3003", "Website Builder Theme", 45.00, "Software & Tools");
        products.emplace_back("3004", "API Access Pack", 100.00, "Software & Tools");
        products.emplace_back("3005", "SDK for Developers", 80.00, "Software & Tools");

        products.emplace_back("4001", "Royalty-Free Music Pack", 30.00, "Music & Audio");
        products.emplace_back("4002", "Sound Effects Collection", 25.00, "Music & Audio");
        products.emplace_back("4003", "Audiobook: Business Success", 15.00, "Music & Audio");
        products.emplace_back("4004", "Paid Podcast Subscription", 10.00, "Music & Audio");
        products.emplace_back("4005", "Voiceover Samples", 20.00, "Music & Audio");

        products.emplace_back("5001", "Business Plan Template", 35.00, "Business & Marketing");
        products.emplace_back("5002", "Marketing Kit", 40.00, "Business & Marketing");
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
                break;
            }
            else if (toupper(choice) == 'Y') {
                bool loggedIn = logIn();
                if (!loggedIn) {
                    cout << "Exiting program. Goodbye!" << endl;
                    break;
                }
                mainMenu();
                cart.clearCart();
                currentUser = nullptr;
            }
            else if (toupper(choice) == 'N') {
                bool signedUp = signUpAndLogin();
                if (!signedUp) {
                    cout << "Exiting program. Goodbye!" << endl;
                    break;
                }
                mainMenu();
                cart.clearCart();
                currentUser = nullptr;
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

            try {
                currentUser = &auth.logIn(email, password);
                cout << "Login successful!" << endl;
                return true;
            } catch (const UserException& e) {
                cout << e.what() << endl;
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
        while (true) {
            cout << "\n=== Sign Up ===" << endl;
            cout << "Email (or 'exit' to quit): ";
            getline(cin,email);
            if(email == "exit" || email == "EXIT") return false;
            cout << "Password: ";
            getline(cin,password);

            try {
                auth.signUp(email, password);
                cout << "Sign up successful! Logging you in now..." << endl;
                currentUser = auth.findUserByEmail(email);
                if (!currentUser) {
                    cout << "Unexpected error: user not found after sign-up." << endl;
                    return false;
                }
                return true;
            } catch (const UserException& e) {
                cout << e.what() << endl;
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
            cout << "1. Browse Products\n2. Search Products\n3. Filter Products\n4. View Cart\n5. Add to Cart\n6. Edit Cart\n7. Checkout\n8. Purchase History\n9. Logout\n";
            cout << "==========================\n";
            cout << "Enter your choice: ";
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: browseProducts(); break;
                case 2: searchProducts(); break;
                case 3: filterProducts(); break;
                case 4: viewCartOption(); break;
                case 5: addToCartOption(); break;
                case 6: editCartOption(); break;
                case 7: checkoutOption(); break;
                case 8: purchaseHistory(); break;
                case 9:
                    cout << "Logging out..." << endl;
                    return;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }
    }

    void browseProducts() {
        cout << "\nAvailable Digital Products:\n-------------------" << endl;
        for (const auto& product : products) {
            cout << product.id << " - " << product.name
                 << " - Category: " << product.category
                 << " - Price: $" << fixed << setprecision(2) << product.price << endl;
        }
    }

    void searchProducts() {
        cout << "\nSearch Products:" << endl;
        cout << "Enter search term: ";
        string term;
        getline(cin, term);
        transform(term.begin(), term.end(), term.begin(), ::tolower);

        bool found = false;
        for (const auto& product : products) {
            string nameLower = product.name;
            transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
            if (nameLower.find(term) != string::npos) {
                cout << product.id << " - " << product.name
                     << " - Category: " << product.category
                     << " - Price: $" << fixed << setprecision(2) << product.price << endl;
                found = true;
            }
        }
        if (!found) cout << "No products found containing: " << term << endl;
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
        cin >> catChoice;
        cin.ignore();
        if (catChoice < 1 || catChoice > (int)categories.size()) {
            cout << "Invalid category choice." << endl;
            return;
        }

        string selectedCategory = categories[catChoice-1];
        cout << "\nProducts in category: " << selectedCategory << "\n-------------------" << endl;
        for (const auto& product : products) {
            if (product.category == selectedCategory) {
                cout << product.id << " - " << product.name
                     << " - Price: $" << fixed << setprecision(2) << product.price << endl;
            }
        }
    }

    void viewCartOption() {
        cout << "\nViewing Cart:" << endl;
        cart.viewCart();
    }

    void addToCartOption() {
        cout << "\nAdd to Cart:" << endl;
        if (products.empty()) {
            cout << "No products available." << endl;
            return;
        }
        browseProducts();
        cout << "Enter product ID or name to add (or 'exit' to cancel): ";
        string input;
        getline(cin, input);
        if (input == "exit") return;

        Product* productPtr = nullptr;
        for (auto& product : products) {
            if (product.id == input) { productPtr = &product; break; }
        }
        if (!productPtr) {
            string inputLower = input;
            transform(inputLower.begin(), inputLower.end(), inputLower.begin(), ::tolower);
            for (auto& product : products) {
                string nameLower = product.name;
                transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);
                if (nameLower.find(inputLower) != string::npos) {
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
                cin.ignore(10000, '\n');
                quantity = 0;
            }
        }
        cin.ignore();

        cart.addItem(*productPtr, quantity);
        cout << "Successfully added " << productPtr->name << " to cart!" << endl;
    }

    void editCartOption() {
        if (cart.isEmpty()) {
            cout << "\nYour cart is empty, nothing to edit." << endl;
            return;
        }

        cout << "\nEdit Cart:\n------------" << endl;
        cart.viewCart();

        cout << "Choose an option: 1. Remove items 2. Edit quantity 3. Cancel\nEnter choice: ";
        int choice;
        cin >> choice;
        cin.ignore();

        if (choice == 1) {
            cout << "Enter product ID to remove: ";
            string id;
            getline(cin, id);

            auto items = cart.getItems();
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
                cart.removeItem(id);
                cout << "Successfully removed item!" << endl;
            } else {
                cout << "Removal canceled." << endl;
            }
        }
        else if (choice == 2) {
            cout << "Enter product ID to edit quantity: ";
            string id;
            getline(cin, id);

            auto items = cart.getItems();
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
                    cin.ignore(10000, '\n');
                    newQuantity = 0;
                }
            }
            cin.ignore();

            cart.updateQuantity(id, newQuantity);
            cout << "Successfully adjusted quantity of item!" << endl;
        }
        else if (choice == 3) {
            cout << "Edit cancelled." << endl;
            return;
        }
        else {
            cout << "Invalid choice." << endl;
        }
    }

    void checkoutOption() {
        if (cart.isEmpty()) {
            cout << "\nYour cart is empty. Cannot proceed to checkout." << endl;
            return;
        }

        cout << "\nCheckout:\n------------------" << endl;
        cart.viewCart();

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
        cout << "Phone number: ";
        getline(cin, phone);

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

        for (const auto& item : cart.getItems()) {
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

        cart.clearCart();

        cout << "Successfully placed your order! You can now download your digital products." << endl;
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
            cout << pi.product.id << " - " << pi.product.name
                 << " - Quantity: " << pi.quantity
                 << " - Price: $" << fixed << setprecision(2) << pi.priceAtPurchase * pi.quantity << endl;
        }

        cout << "------------------" << endl;
        cout << "Total Items: " << order.totalQuantity << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << order.totalPrice << endl;

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
