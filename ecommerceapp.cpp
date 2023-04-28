#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <climits>
#include <fstream>

using namespace std;

struct Product {
    string name;
    int price;
    int quantity = 0;
    vector<vector<int>> purchaseHistory; // first element of each vector is purchase quantity, second is price
    vector<vector<int>> orderHistory; // first element of each vector is order quantity, second is price
};

struct Ecommerce {
public:
    void save_product(const string& product_id, const string& product_name, const int& product_price) {
        auto it = catalog.find({ product_id });
        if (it != catalog.end()) { // if the product is already in the catalog, modify information
            catalog[product_id].name = product_name;
            catalog[product_id].price = product_price;
        }
        else { // if the product isn't in the catalog, add it
            catalog[product_id] = { product_name, product_price };
        }
        cout << "Product was saved: id - " << product_id << "; name - " << product_name << "; price - " << product_price << endl;
    }

    void purchase_product(const string& product_id, const int& product_quantity, const int& product_price) {
        if (product_quantity == 0) { // so that it doesn't divide by 0 when calculating average price
            cout << "A purchase of 0 products can't be placed." << endl;
        }
        else {
            auto it = catalog.find({ product_id });
            if (it != catalog.end()) {
                catalog[product_id].quantity += product_quantity;
                vector<int> purchase = { product_quantity, product_price };
                catalog[product_id].purchaseHistory.push_back(purchase);
                cout << "Product was purchased: id - " << product_id << "; name - " << catalog[product_id].name << "; price - " << product_price << "; quantity purchased - " << product_quantity << endl;
            }
            else {
                cout << "The product doesn't exist in the catalog." << endl;
            }
        }
    }

    void order_product(const string& product_id, const int& product_quantity) {
        if (product_quantity == 0) { // so that it doesn't divide by 0 when calculating average price
            cout << "An order of 0 products can't be placed." << endl;
        }
        else {
            auto it = catalog.find({ product_id });
            if (it != catalog.end()) {
                if (catalog[product_id].quantity < product_quantity) {
                    cout << "Not enough quantity of this product left." << endl;
                }
                else {
                    catalog[product_id].quantity -= product_quantity;
                    vector<int> order = { product_quantity, catalog[product_id].price };
                    catalog[product_id].orderHistory.push_back(order);
                    cout << "Product was ordered: id - " << product_id << "; name - " << catalog[product_id].name << "; price - " << catalog[product_id].price << "; quantity ordered - " << product_quantity << endl;
                }
            }
            else {
                cout << "The product doesn't exist in the catalog." << endl;
            }
        }
    }

    void get_quantity_of_product(const string& product_id) {
        auto it = catalog.find({ product_id });
        if (it != catalog.end()) {
            cout << "id - " << product_id << "; quantity - " << catalog[product_id].quantity << endl;
        }
        else {
            cout << "The product doesn't exist in the catalog." << endl;
        }
    }

    void get_average_price(const string& product_id) {
        auto it = catalog.find({ product_id });
        if (it != catalog.end()) {
            if (catalog[product_id].purchaseHistory.empty()) {
                cout << "No purchases of this product were made." << endl;
            }
            else {
                cout << "The average price of the product based on its purchases is " << avgPrice(catalog[product_id].purchaseHistory) << endl;
            }
        }
        else {
            cout << "The product doesn't exist in the catalog." << endl;
        }
    }

    void get_product_profit(const string& product_id) {
        auto it = catalog.find({ product_id });
        if (it != catalog.end()) {
            if (catalog[product_id].orderHistory.empty()) {
                cout << "No orders of this product were made." << endl;
            }
            else {
                // calculating profit per unit
                int profitPerUnit = avgPrice(catalog[product_id].orderHistory) - avgPrice(catalog[product_id].purchaseHistory);

                // calculating total profit
                int totProfit = profitPerUnit * quantSum(catalog[product_id].orderHistory);
                cout << "Total profit from this product is " << totProfit << endl;
            }
        }
        else {
            cout << "The product doesn't exist in the catalog." << endl;
        }
    }

    void get_fewest_product() {
        // find the number of lowest quantity
        int minQ = INT_MAX;
        int currQ;
        for (auto it = catalog.begin(); it != catalog.end(); ++it) {
            currQ = it->second.quantity;
            if (currQ < minQ) {
                minQ = currQ;
            }
        }

        // check if there's more than one product
        vector<string> fewestNames;
        for (auto it = catalog.begin(); it != catalog.end(); ++it) {
            if (it->second.quantity == minQ) {
                fewestNames.push_back(it->second.name);
            }
        }

        // print the names of multiple products with the lowest remaining quantity
        if (fewestNames.size() > 1) {
            cout << "The names of the products with the lowest remaining quantities are: ";
            for (int i = 0; i < fewestNames.size(); i++) {
                cout << fewestNames[i];
                if (i == fewestNames.size() - 1) {
                    cout << "." << endl;
                }
                else {
                    cout << ", ";
                }
            }
        }
        else {
            cout << "The name of the product with the lowest remaining quantity is " << fewestNames[0]  << "." << endl;
        }
    }

    void get_most_popular_product() {
        // find the number of maximum orders
        int maxOrder = 0;
        for (auto it = catalog.begin(); it != catalog.end(); ++it) {
            int orderSum = 0;
            for (vector<int> order : it->second.orderHistory) {
                orderSum += order[0];
            }
            if (orderSum > maxOrder) {
                maxOrder = orderSum;
            }
        }
        if (maxOrder == 0) {
            cout << "No products were ordered." << endl;
        }
        else {
            // check if there's more than one product
            vector<string> popularNames;
            for (auto it = catalog.begin(); it != catalog.end(); ++it) {
                int orderSum = 0;
                for (vector<int> order : it->second.orderHistory) {
                    orderSum += order[0];
                }
                if (orderSum == maxOrder) {
                    popularNames.push_back(it->second.name);
                }
            }

            // print the names of multiple products with the highest number of orders
            if (popularNames.size() > 1) {
                cout << "The names of the products with the highest number of orders are: ";
                for (int i = 0; i < popularNames.size(); i++) {
                    cout << popularNames[i];
                    if (i == popularNames.size() - 1) {
                        cout << "." << endl;
                    }
                    else {
                        cout << ", ";
                    }
                }
            }
            else {
                cout << "The name of the product with the highest number of orders is " << popularNames[0] << "." << endl;
            }
        }
    }

    void get_orders_report() {
        for (auto it = catalog.begin(); it != catalog.end(); ++it) {
            if (!it->second.orderHistory.empty()) {
                for (vector<int> order : it->second.orderHistory) { // print every order made for each product
                    cout << "id - " << it->first << "; name - " << it->second.name << "; quantity sold - " << order[0] << "; selling price - " << order[1] << "; COGS - " << avgPrice(it->second.purchaseHistory) << "; quantity left - " << it->second.quantity << endl;
                }
            }
        }
    }

    void export_orders_report(string path) {
        if (path.length() >= 4 && path.substr(path.length() - 4) != ".csv" || path.length() < 4) {
            path += ".csv";
        }
        ofstream outfile(path);
        if (outfile.is_open()) {
            outfile << "id" << "," << "name" << "," << "quantity sold" << "," << "selling price" << "," << "COGS" << "," << "quantity left" << endl;
            for (auto it = catalog.begin(); it != catalog.end(); ++it) {
                if (!it->second.orderHistory.empty()) {
                    for (vector<int> order : it->second.orderHistory) { // print every order made for each product
                        outfile << it->first << "," << it->second.name << "," << order[0] << "," << order[1] << "," << avgPrice(it->second.purchaseHistory) << "," << it->second.quantity << endl;
                    }
                }
            }
            outfile.close();
            cout << "Orders report was exported to file path \"" << path << "\"." << endl;
        }
        else {
            cout << "Failed to open file at the requested path. Please try again." << endl;
        }
    }

private:
    map<string, Product> catalog;

    // calculate average price based on purchase/order history
    int avgPrice(const vector<vector<int>>& history) {
        int priceSum = 0;
        int quantSum = 0;
        for (vector<int> vec : history) {
            priceSum += vec[0] * vec[1];
            quantSum += vec[0];
        }
        return priceSum / quantSum;
    }

    // calculate total quantity based on purchase/order history
    int quantSum(const vector<vector<int>>& history) {
        int quantSum = 0;
        for (vector<int> vec : history) {
            quantSum += vec[0];
        }
        return quantSum;
    }
};

// check if string contains only digits
bool isInt(const string& s) {
    for (int i = 0; i < s.length(); i++) {
        if (i == s.length() - 1 && s[i] == '\r') { // so it doesn't include enter key
            return true;
        } else if (!isdigit(s[i])) {
            return false;
        }
    }

    return true;
}

int main()
{
    Ecommerce ec;
    string input;

    // message displaying all the possible functions
    cout << "Enter one of the following commands: \n"
        << "save_product {product_id} {product_name} {product_price}\n"
        << "purchase_product {product_id} {quantity} {price}\n"
        << "order_product {product_id} {quantity}\n"
        << "get_quantity_of_product {product_id}\n"
        << "get_average_price {product_id}\n"
        << "get_product_profit {product_id}\n"
        << "get_fewest_product\n"
        << "get_most_popular_product\n"
        << "get_orders_report\n"
        << "export_orders_report {path}\n"
        << "exit\n"
        << "--------------------------------------------------------\n";

    while (true) {
        // tokenize the input
        getline(cin, input);
        stringstream ss(input);
        vector<string> tokens;
        string token;
        while (getline(ss, token, ' ')) {
            tokens.push_back(token);
        }

        // so the enter key isn't included in the string
        string lastToken = tokens[tokens.size() - 1];
        if (lastToken[lastToken.length() - 1] == '\r') {
            lastToken.pop_back();
            tokens[tokens.size() - 1] = lastToken;
        }

        if (!tokens.empty()) {
            string func = tokens[0]; // first word is function name
            if (func == "save_product") {
                if (tokens.size() < 4) {
                    cout << "Parameters missing. Please try again." << endl;
                }
                else {
                    if (isInt(tokens[3])) {
                        ec.save_product(tokens[1], tokens[2], stoi(tokens[3]));
                    }
                    else {
                        cout << "The third parameter of the function save_product has to be an integer." << endl;
                    }
                }
            }
            else if (func == "purchase_product") {
                if (tokens.size() < 4) {
                    cout << "Parameters missing. Please try again." << endl;
                }
                else {
                    if (isInt(tokens[2]) && isInt(tokens[3])) {
                        ec.purchase_product(tokens[1], stoi(tokens[2]), stoi(tokens[3]));
                    }
                    else {
                        cout << "The second and third parameters of the function purchase_product have to be integers." << endl;
                    }
                }
            }
            else if (func == "order_product") {
                if (tokens.size() < 3) {
                    cout << "Parameters missing. Please try again." << endl;
                }
                else {
                    if (isInt(tokens[2])) {
                        ec.order_product(tokens[1], stoi(tokens[2]));
                    }
                    else {
                        cout << "The second parameter of the function order_product has to be an integer." << endl;
                    }
                }
            }
            else if (func == "get_quantity_of_product") {
                if (tokens.size() < 2) {
                    cout << "Parameters missing. Please try again." << endl;
                }
                else {
                    ec.get_quantity_of_product(tokens[1]);
                }
            }
            else if (func == "get_average_price") {
                if (tokens.size() < 2) {
                    cout << "Parameters missing. Please try again." << endl;
                }
                else {
                    ec.get_average_price(tokens[1]);
                }
            }
            else if (func == "get_product_profit") {
                if (tokens.size() < 2) {
                    cout << "Parameters missing. Please try again." << endl;
                }
                else {
                    ec.get_product_profit(tokens[1]);
                }
            }
            else if (func == "get_fewest_product") {
                ec.get_fewest_product();
            }
            else if (func == "get_most_popular_product") {
                ec.get_most_popular_product();
            }
            else if (func == "get_orders_report") {
                ec.get_orders_report();
            }
            else if (func == "export_orders_report") {
                if (tokens.size() < 2) {
                    cout << "Parameters missing. Please try again." << endl;
                }
                else {
                    ec.export_orders_report(tokens[1]);
                }
            }
            else if (func == "exit") {
                return 0;
            }
            else {
                cout << "Invalid function name. Please try again." << endl;
            }
        }
    }

    return 0;
}