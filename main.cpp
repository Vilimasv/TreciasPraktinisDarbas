#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

struct menuItemType {
    string menuItem;
    double menuPrice;
};

void getData(menuItemType menuList[], int &kiek) {
    ifstream fin("menu.txt");

    if (!fin.is_open()) {
        cout << "Nepavyko atidaryti menu.txt failo!\n";
        exit(1);
    }

    kiek = 0;

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

void showMenu(menuItemType menuList[], int kiek) {
    cout << "\n--- PUSRYČIŲ MENIU ---\n";

    for (int i = 0; i < kiek; i++) {
        cout << i + 1 << ". " << menuList[i].menuItem
             << "  " << fixed << setprecision(2)
             << menuList[i].menuPrice << "€\n";
    }

    cout << "\nPasirinkite patiekalo numerį (0 - baigti užsakymą)\n";
}

void printCheck(menuItemType menuList[], int kiek, int pasirinkimai[], int porcijos[]) {
    ofstream fout("receipt.txt");

    double suma = 0;

    cout << "\nSveiki atvykę į restoraną „Pavadinimas“\n\n";
    fout << "Sveiki atvykę į restoraną „Pavadinimas“\n\n";

    for (int i = 0; i < kiek; i++) {
        if (pasirinkimai[i] == 1) {

            double tarpine = menuList[i].menuPrice * porcijos[i];
            suma += tarpine;

            cout << porcijos[i] << "  " << menuList[i].menuItem
                 << "   " << fixed << setprecision(2)
                 << tarpine << "€\n";

            fout << porcijos[i] << "  " << menuList[i].menuItem
                 << "   " << fixed << setprecision(2)
                 << tarpine << "€\n";
        }
    }

    double pvm = suma * 0.21;
    double galutine = suma + pvm;

    cout << "\nMokesčiai (21%)   " << fixed << setprecision(2) << pvm << "€\n";
    cout << "Galutinė suma     " << fixed << setprecision(2) << galutine << "€\n";

    fout << "\nMokesčiai (21%)   " << fixed << setprecision(2) << pvm << "€\n";
    fout << "Galutinė suma     " << fixed << setprecision(2) << galutine << "€\n";

    fout.close();
}