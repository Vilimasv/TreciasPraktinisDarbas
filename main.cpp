#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

struct menuItemType {
    string menuItem;
    double menuPrice;
};

// Nuskaito meniu iš failo
int getData(menuItemType menuList[]) {
    ifstream fin("menu.txt");

    if (!fin.is_open()) {
        cout << "Nepavyko atidaryti menu.txt failo!\n";
        return 0;
    }

    int count = 0;

    while (fin >> ws && getline(fin, menuList[count].menuItem, '\t') &&
           fin >> menuList[count].menuPrice) {
        count++;
    }

    fin.close();
    return count;
}

// Parodo meniu
void showMenu(menuItemType menuList[], int menuSize) {
    cout << "\n--- PUSRYČIŲ MENIU ---\n";

    for (int i = 0; i < menuSize; i++) {
        cout << i + 1 << ". "
             << left << setw(40) << menuList[i].menuItem
             << fixed << setprecision(2) << menuList[i].menuPrice << " EUR\n";
    }

    cout << "\nPasirinkite patiekalo numeri (0 - baigti užsakyma)\n";
}

// Spausdina sąskaita i ekrana ir į faila
void printCheck(menuItemType menuList[], int pasirinkti[], int kiekiai[], int pasirinkimuKiekis) {
    ofstream fout("receipt.txt");

    cout << "\n--- SASKAITA ---\n";
    fout << "--- SASKAITA ---\n";

    double suma = 0;

    for (int i = 0; i < pasirinkimuKiekis; i++) {
        int id = pasirinkti[i];
        int kiek = kiekiai[i];

        double tarpine = menuList[id].menuPrice * kiek;
        suma += tarpine;

        cout << left << setw(3) << kiek
             << setw(40) << menuList[id].menuItem
             << fixed << setprecision(2) << tarpine << " EUR\n";

        fout << left << setw(3) << kiek
             << setw(40) << menuList[id].menuItem
             << fixed << setprecision(2) << tarpine << " EUR\n";
    }

    double pvm = suma * 0.21;
    double galutine = suma + pvm;

    cout << "\nMokesčiai (21%)" << setw(33) << " "
         << fixed << setprecision(2) << pvm << " EUR\n";

    cout << "Galutinė suma" << setw(35) << " "
         << fixed << setprecision(2) << galutine << " EUR\n";

    fout << "\nMokesčiai (21%)" << setw(33) << " "
         << fixed << setprecision(2) << pvm << " EUR\n";

    fout << "Galutinė suma" << setw(35) << " "
         << fixed << setprecision(2) << galutine << " EUR\n";

    fout.close();
}

int main() {

    // ijungiam UTF-8, kad nebutu keistu simboliu
    system("chcp 65001 > nul");

    menuItemType menuList[50];
    int menuSize = getData(menuList);

    if (menuSize == 0) {
        cout << "Meniu negautas. Programa stabdoma.\n";
        return 0;
    }

    int pasirinkti[100];
    int kiekiai[100];
    int pasirinkimuKiekis = 0;

    bool programIsRunning = true;

    while (programIsRunning) {

        showMenu(menuList, menuSize);

        int pasirinkimas;
        cout << "Pasirinkimas: ";
        cin >> pasirinkimas;

        if (pasirinkimas == 0) {
            programIsRunning = false;
            break;
        }

        if (pasirinkimas < 1 || pasirinkimas > menuSize) {
            cout << "Neteisingas pasirinkimas!\n";
            continue;
        }

        int kiek;
        cout << "Kiek porciju norite? ";
        cin >> kiek;

        if (kiek < 1) kiek = 1;

        pasirinkti[pasirinkimuKiekis] = pasirinkimas - 1;
        kiekiai[pasirinkimuKiekis] = kiek;
        pasirinkimuKiekis++;

        cout << "Patiekalas pridėtas!\n";
    }

    if (pasirinkimuKiekis > 0) {
        printCheck(menuList, pasirinkti, kiekiai, pasirinkimuKiekis);
        cout << "\nSaskaita issaugota faile receipt.txt\n";
    } else {
        cout << "Nieko neuzsisakete.\n";
    }

    return 0;
}
