#include <fstream>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

enum Gender { Male, Female };

struct Dog {
    string breed;
    string name;
    int age;
    string color;
    Gender gender;
};

#pragma region Utils
string to_string(Gender g) {
    switch (g) {
    case Male:
        return "Kan";
    case Female:
        return "Szuka";
    }
    return "Ismeretlen";
}

string tolower(string str) {
    for (size_t i = 0; i < str.length(); i++)
        if (str[i] <= 'Z' && str[i] >= 'A')
            str[i] += 32;
    return str;
}

void clearScreen() { cout << "\033[2J\033[1;1H"; }
void waitUser() {
    cout << "Nyomjon entert a folytatáshoz...";
    cin.ignore();
    cin.get();
}

int actuallLength(char *s) {
    int len = 0;
    while (*s)
        len += (*s++ & 0xc0) != 0x80;
    return len;
}
string padd(string str, int len) {
    int length = actuallLength((char *)str.c_str());
    if (length > len - 4)
        return str.substr(0, len - 4) + "... ";
    str += string(len - length, ' ');
    return str;
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
#pragma endregion

#pragma region IO
vector<Dog> fetchDB() {
    vector<Dog> dogs;
    string line;
    ifstream input_file("db.csv");
    while (getline(input_file, line, '\n')) {
        Dog newDog;
        auto data = split(line, ';');
        newDog.breed = data[0];
        newDog.name = data[1];
        newDog.age = stoi(data[2]);
        newDog.color = data[3];
        newDog.gender = data[4] == "F" ? Female : Male;
        dogs.push_back(newDog);
    }
    input_file.close();
    return dogs;
}
void saveDB(vector<Dog> dogs) {
    ofstream output_file("db.csv");
    for (size_t i = 0; i < dogs.size(); i++) {
        output_file << dogs[i].breed << ";" << dogs[i].name << ";"
                    << dogs[i].age << ";" << dogs[i].color << ";"
                    << (dogs[i].gender == Female ? "F" : "M") << endl;
    }
    output_file.close();
}
#pragma endregion

void printDogs(vector<Dog> dogs) {
    int col_width = 30;
    cout << padd("ID", 7) << padd("Név", col_width) << padd("Fajta", col_width)
         << padd("Kor", 10) << padd("Szín", col_width)
         << padd("Ivar", col_width) << endl;
    for (size_t i = 0; i < dogs.size(); i++) {
        cout << padd(to_string(i + 1) + ".", 7) << padd(dogs[i].name, col_width)
             << padd(dogs[i].breed, col_width)
             << padd(to_string(dogs[i].age), 10)
             << padd(dogs[i].color, col_width)
             << padd(to_string(dogs[i].gender), col_width) << endl;
    }
}

void readFilter(string out[2]) {
    cout << "Kérem adja meg a szűrőfeltételt "
            "(Fajta,Név,Kor,Szín,Ivar): ";
    string filter;
    cin >> filter;
    cout << "Kérem adja meg a szűrőértéket: ";
    string value;
    cin >> value;
    out[0] = filter;
    out[1] = value;
}

string flattenDog(Dog *d) {
    return "fajta:" + d->breed + ";név:" + d->name +
           ";kor:" + to_string(d->age) + ";szín:" + d->color +
           ";ivar:" + to_string(d->gender);
}

bool dogMatchesFilter(Dog *d, string *filter) {
    string flat = tolower(flattenDog(d));
    string fkey = filter[0];
    string fvalue = tolower(filter[1]);

    string temp = flat.substr(flat.find(fkey) + fkey.length() + 1);
    string vvalue = temp.substr(0, temp.find(";"));

    return vvalue.find(fvalue) != string::npos;
}

vector<Dog> filterDogs(vector<Dog> dogs, string *filter) {
    vector<Dog> filteredDogs;
    for (size_t i = 0; i < dogs.size(); i++)
        if (dogMatchesFilter(&dogs[i], filter))
            filteredDogs.push_back(dogs[i]);
    return filteredDogs;
}

Dog readDog() {
    Dog newDog;
    cout << "Kérem adja meg a kutya fajtáját: ";
    cin >> newDog.breed;
    cout << "Kérem adja meg a kutya nevét: ";
    cin >> newDog.name;
    cout << "Kérem adja meg a kutya korát: ";
    cin >> newDog.age;
    cout << "Kérem adja meg a kutya színét: ";
    cin >> newDog.color;
    cout << "Kérem adja meg a kutya ivarát (F/M): ";
    string temp = "";
    cin >> temp;
    newDog.gender = tolower(temp) == "F" ? Female : Male;
    return newDog;
}

int main() {
    vector<Dog> dogs = fetchDB();
    vector<Dog> filteredDogs;
    string filter[2];
    string choice;
    while (true) {
        clearScreen();
        cout << "---------------Kutyák menhelyen----------\n";
        if (filteredDogs.size() == 0)
            printDogs(dogs);
        else {
            cout << "Szűrés: " << filter[0] << "=" << filter[1] << endl;
            printDogs(filteredDogs);
        }
        cout << endl;
        cout << "1. Szűrés\n2. Összes kutya\n3. Hozzáadás\n4. Kilépés és "
                "mentés\nVálasz: ";
        cin >> choice;

        if (choice == "1") {
            readFilter(filter);
            filteredDogs = filterDogs(dogs, filter);
            if (filteredDogs.size() == 0) {
                cout << "Nincs találat\n";
                waitUser();
                continue;
            }
        } else if (choice == "2")
            filteredDogs.clear();
        else if (choice == "3")
            dogs.push_back(readDog());
        else if (choice == "4")
            break;
    };
    saveDB(dogs);
    return 1;
}