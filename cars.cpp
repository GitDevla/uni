#include <iostream>
#include <vector>
using namespace std;

#pragma region Declerations
void decrementPage();
void incrementPage();
string input(char* output);
void waitUser();
struct Car;
void clearScreen();
string padd(string str, int len);
string toLower(string str);
string flattenCar(const Car* c);
bool carMatchesFilter(const Car* c, vector<string> filter);
void updateFilterCars();
void printHeader();
void printCarsTable();
void printFilterMenu();
void printHelp();
void printViewMenu();
void printRemoveFilterMenu();
void drawUserInterface();
int actuallLength(char* s);
#pragma endregion

#pragma region Car and functions
struct Car {
    string brand;
    string model;
    int year;
    int price;
    int km;
    string color;
    string plate_number;

    Car(string brand, string model, int year, int price, int km, string color,
        string plate_number) {
        this->brand = brand;
        this->model = model;
        this->year = year;
        this->price = price;
        this->km = km;
        this->color = color;
        this->plate_number = plate_number;
    }
};
Car CARS[20] = {
    {"Toyota", "Corolla", 2018, 7200000, 45000, "Kék", "ABC123"},
    {"Toyota", "Camry", 2019, 9000000, 35000, "Piros", "DEF456"},
    {"Ford", "F-150", 2017, 10800000, 55000, "Fekete", "GHI789"},
    {"Ford", "Mustang", 2020, 16200000, 25000, "Fehér", "JKL012"},
    {"Honda", "Civic", 2016, 6480000, 60000, "Ezüst", "MNO345"},
    {"Honda", "Accord", 2015, 7920000, 70000, "Szürke", "PQR678"},
    {"BMW", "3 Series", 2019, 12600000, 40000, "Zöld", "STU901"},
    {"BMW", "5 Series", 2021, 18000000, 15000, "Sárga", "VWX234"},
    {"Hyundai", "Elantra", 2018, 6840000, 50000, "Narancs", "YZA567"},
    {"Hyundai", "Tucson", 2017, 6120000, 60000, "Lila", "BCD890"},
    {"Tesla", "Model S", 2020, 27000000, 20000, "Barna", "EFG123"},
    {"Tesla", "Model X", 2019, 32400000, 55000, "Kék", "HIJ456"},
    {"Mazda", "Mazda3", 2018, 8280000, 30000, "Piros", "KLM789"},
    {"Mazda", "CX-5", 2017, 9720000, 45000, "Fekete", "NOP012"},
    {"Chevrolet", "Silverado", 2016, 12600000, 65000, "Fehér", "QRS345"},
    {"Chevrolet", "Camaro", 2021, 21600000, 12000, "Ezüst", "TUV678"},
    {"Audi", "A4", 2016, 11520000, 80000, "Szürke", "WXY901"},
    {"Audi", "Q5", 2020, 14400000, 25000, "Zöld", "ZAB234"},
    {"Jeep", "Grand Cherokee", 2017, 16200000, 40000, "Sárga", "BCD567"},
    {"Jeep", "Wrangler", 2018, 10800000, 30000, "Narancs", "EFG890"}};

string flattenCar(const Car* c) {
    string temp = ("brand:" + c->brand + ";model:" + c->model +
                   ";year:" + to_string(c->year) +
                   ";price:" + to_string(c->price) + ";km:" + to_string(c->km) +
                   ";color:" + c->color + ";plate_number:" + c->plate_number);
    return toLower(temp);
}
bool carMatchesFilter(const Car* c, vector<string> filter) {
    string flat = flattenCar(c);
    string fkey = filter[0];
    string fvalue = toLower(filter[1]);

    string temp = flat.substr(flat.find(fkey) + fkey.length() + 1);
    string vvalue = temp.substr(0, temp.find(";"));
    if (fvalue[0] == '<' || fvalue[0] == '>') {
        int value = stoi(fvalue.substr(1));
        if (fvalue[0] == '<' && stoi(vvalue) >= value) return false;
        if (fvalue[0] == '>' && stoi(vvalue) <= value) return false;
        return true;
    }

    return vvalue.find(fvalue) != string::npos;
}
vector<vector<string>> filters;
vector<Car> filteredCars = vector<Car>(CARS, CARS + sizeof(CARS) / sizeof(Car));
#pragma endregion

#pragma region User Interface
bool filterMode = false;
bool run = true;
int page = 0;
const int PERPAGE = 7;
int totalCars = sizeof(CARS) / sizeof(Car);
int totalPages = totalCars / PERPAGE;

void printHeader() {
    cout << "Hibás adatok esetén a program nem fog működni helyesen\n\n";
    cout << "(" << page + 1 << "/" << totalPages + 1 << ". oldal)" << endl;
    cout << "Jelenlegi szűrők: ";
    if (filters.size() == 0) {
        cout << "Nincs" << endl;
    } else {
        int s = filters.size();
        for (size_t i = 0; i < filters.size() - 1; i++)
            cout << filters[i][0] << ": " << filters[i][1] << " és ";
        cout << filters[s - 1][0] << ": " << filters[s - 1][1] << endl;
    }
    cout << totalCars << " találat ("
         << min(PERPAGE, totalCars - (PERPAGE * page)) << " db mutatása)"
         << endl;
}

void printCarsTable() {
    cout << endl;
    cout << padd("Márka", 20) << padd("Model", 20) << padd("Évjárat", 20)
         << padd("Ár", 20) << padd("Kilóméteróra", 20) << padd("Szín", 20)
         << padd("Rendszám", 20) << endl;
    cout << string(140, '-') << endl;
    if (totalCars == 0) {
        cout << "Nincs találat" << endl;
    }
    for (int i = page * PERPAGE; i < PERPAGE * page + PERPAGE; i++) {
        if (i >= totalCars) break;

        cout << padd(filteredCars[i].brand, 20)
             << padd(filteredCars[i].model, 20)
             << padd(to_string(filteredCars[i].year), 20)
             << padd(to_string(filteredCars[i].price) + " Ft", 20)
             << padd(to_string(filteredCars[i].km) + " km", 20)
             << padd(filteredCars[i].color, 20)
             << padd(filteredCars[i].plate_number, 20) << endl;
    }
    cout << string(140, '-') << endl;
    cout << endl;
}

void printFilterMenu() {
    string options[][2] = {{"Márka", "brand"},          {"Modell", "model"},
                           {"Évjárat", "year"},         {"Ár", "price"},
                           {"Kilóméteróra", "km"},      {"Szín", "color"},
                           {"Rendszám", "plate_number"}};
    for (size_t i = 0; i < 7; i++)
        cout << padd(to_string(i + 1) + ": " + options[i][0], 20)
             << ((i + 1) % 3 == 0 ? "\n" : " ");
    cout << endl;

    cout << padd("c: Összess filter eltávolítása", 40)
         << padd("d: Specificus filter eltávolítása", 40) << endl;
    cout << padd("q: Kilépés filter módbol", 40) << endl;
    cout << endl;

    string command = input("Milyen szűrőt szeretne használni: ");
    if (command[0] == 'q')
        filterMode = false;
    else if (command[0] == 'd')
        printRemoveFilterMenu();
    else if (command[0] == 'c')
        filters.clear();
    else if (command[0] >= '1' && command[0] <= '7') {
        auto temp = options[atoi(command.c_str()) - 1];
        cout << "Adja meg a szűrő értékét Példa: tOyoTa, toy, >2015, <50000, "
                "4000\n"
             << endl
             << temp[0] << ": ";
        filters.push_back({temp[1], input("")});
    }

    updateFilterCars();
    page = 0;
}

void printRemoveFilterMenu() {
    if (filters.size() == 0) {
        cout << "Nincs szűrő" << endl;
        return;
    }
    for (size_t i = 0; i < filters.size(); i++)
        cout << to_string(i + 1) + "-(" + filters[i][0] + ": " + filters[i][1]
             << ")   ";
    cout << endl;
    cout << endl;

    string command =
        input("Melyik szűrőt szeretné eltávolítani (0 ha mégse): ");
    if (atoi(command.c_str()) - 1 < filters.size())
        filters.erase(filters.begin() + atoi(command.c_str()) - 1);
}

void printHelp() {
    cout << "A program használata: " << endl;
    cout << "j(obbra) - jobbra lapozás" << endl;
    cout << "b(alrra) - balra lapozás" << endl;
    cout << "f(ilter) - szűrők alkalmazása" << endl;
    cout << "s(egítég) - ez a súgó" << endl;
    cout << "q(uit) - kilépés" << endl;
    waitUser();
}
void printViewMenu() {
    string command = input("Felhasználó parancsa: ");
    if (command == "j" || command == "jobbra")
        incrementPage();
    else if (command == "b" || command == "balra")
        decrementPage();
    else if (command == "f" || command == "filter")
        filterMode = true;
    else if (command == "s" || command == "segítség")
        printHelp();
    else if (command == "q" || command == "quit")
        run = false;
    else {
        cout << "Ismeretlen parancs" << endl;
        printHelp();
    }
}

void drawUserInterface() {
    clearScreen();
    printHeader();
    printCarsTable();
    if (!filterMode)
        printViewMenu();
    else
        printFilterMenu();
}

void decrementPage() {
    if (page > 0) page--;
}
void incrementPage() {
    if (page < totalPages) page++;
}
#pragma endregion

#pragma region User Interface Utils
void clearScreen() { cout << string(100, '\n'); }

string input(char* output) {
    cout << output;
    string temp;
    cin >> temp;
    return temp;
}

void waitUser() {
    cout << "Nyomjon entert a folytatáshoz...";
    cin.ignore();
    cin.get();
}
#pragma endregion

#pragma region Utils
int actuallLength(char* s) {
    int len = 0;
    while (*s) len += (*s++ & 0xc0) != 0x80;
    return len;
}
string padd(string str, int len) {
    int length = actuallLength((char*)str.c_str());
    if (length > len - 4) return str.substr(0, len - 4) + "... ";
    str += string(len - length, ' ');
    return str;
}

string toLower(string str) {
    for (size_t i = 0; i < str.length(); i++)
        if (str[i] <= 'Z' && str[i] >= 'A') str[i] += 32;
    return str;
}
#pragma endregion

void updateFilterCars() {
    filteredCars.clear();
    if (filters.size() == 0)
        filteredCars = vector<Car>(CARS, CARS + sizeof(CARS) / sizeof(Car));
    else
        for (int i = 0; i < sizeof(CARS) / sizeof(Car); i++) {
            bool add = true;
            for (auto f : filters)
                if (!carMatchesFilter(&CARS[i], f)) {
                    add = false;
                    break;
                }
            if (add) filteredCars.push_back(CARS[i]);
        }
    totalCars = filteredCars.size();
    totalPages = totalCars / PERPAGE;
}

int main() {
    while (run) drawUserInterface();
    return 0;
}
