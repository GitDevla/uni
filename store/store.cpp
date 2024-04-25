#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Order {
    int id;
    vector<string> products;
};

string flatten(vector<string> products) {
    string out;
    for (size_t i = 0; i < products.size(); i++) {
        out += products[i];
        if (i != products.size() - 1)
            out += " ";
    }
    return out;
}

string ilovecpp(float x) {
    auto temp = to_string(round(x * 100) / 100);
    return temp.substr(0, temp.find('.') + 3);
}

vector<string> split(string in, char delim) {
    vector<string> out;
    while (in.find(delim) != string::npos) {
        out.push_back(in.substr(0, in.find(delim)));
        in = in.substr(in.find(delim) + 1);
    }
    out.push_back(in);
    return out;
}

vector<Order> getOrders(string filename) {
    vector<Order> orders;
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        Order order;
        auto temp = split(line, ' ');
        order.id = stoi(temp[0]);
        for (size_t i = 1; i < temp.size(); i++)
            order.products.push_back(temp[i]);
        orders.push_back(order);
    }
    return orders;
}

map<string, int> noOfProducts(vector<Order> orders) {
    map<string, int> products;
    for (auto order : orders) {
        for (auto product : order.products) {
            if (products[product])
                products[product]++;
            else
                products[product] = 1;
        }
    }
    return products;
}

// https://github.com/GitDevla/AoC/blob/6dc1c6504c4f5582a2f862593a32807b98567900/2015/day24/src/main.rs#L103
void combinations(vector<string> keys, vector<vector<string>> &collection,
                  int i = 0, vector<string> prev = {}) {
    if (i == keys.size())
        return;

    for (size_t j = i; j < keys.size(); j++) {
        auto temp = prev;
        temp.push_back(keys[j]);
        collection.push_back(temp);
        combinations(keys, collection, j + 1, temp);
    }
}

bool matchesOrder(Order order, vector<string> products) {
    for (auto product : products) {
        auto flat = flatten(order.products);
        if (flat.find(product) == string::npos)
            return false;
    }
    return true;
}

vector<string> keys(map<string, int> m) {
    vector<string> out;
    for (auto &&i : m) {
        out.push_back(i.first);
    }
    return out;
}

int main() {
    auto orders = getOrders("db.txt");
    cout << "Rendelések: " << endl;
    for (auto order : orders) {
        cout << order.id << " " << flatten(order.products) << endl;
    }
    cout << endl;

    auto productsCount = noOfProducts(orders);
    cout << "Termék neve / Átlagos rendelési mennyiség: " << endl;
    for (auto product : productsCount)
        cout << product.first << ": "
             << ilovecpp((float)product.second / orders.size()) << endl;
    cout << endl;

    vector<string> products = keys(productsCount);
    vector<vector<string>> product_combinations;
    combinations(products, product_combinations);
    cout << "Termék kombinációk / Átlagos rendelési mennyiség: " << endl;
    for (auto &&i : product_combinations) {
        cout << flatten(i) << ": ";
        int sum = 0;
        for (auto &&j : orders)
            if (matchesOrder(j, i))
                sum++;
        cout << ilovecpp((float)sum / orders.size()) << endl;
    }

    return 0;
}