
#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

// Struktūra meniu elementams
struct menuItemType {
    string menuItem;
    double menuPrice;
};

// Funkcija nuskaito meniu iš failo
void getData(menuItemType menuList[], int &kiek) {
    ifstream fin("menu.txt");

    if (!fin.is_open()) {
        cout << "Nepavyko atidaryti menu.txt failo!\n";
        exit(1);
    }

    kiek = 0;

    // Skaitome iki failo pabaigos
    while (!fin.eof()) {
        fin >> ws;
        getline(fin, menuList[kiek].menuItem);

        if (menuList[kiek].menuItem == "")
            break;

        fin >> menuList[kiek].menuPrice;
        fin >> ws;

        kiek++;
    }

    fin.close();
}