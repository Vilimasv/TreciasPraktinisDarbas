#include <iostream>     // Leidžia naudoti cout, cin ir kita įvestį/išvestį
#include <fstream>      // Leidžia dirbti su failais (ifstream, ofstream)
#include <iomanip>      // Leidžia naudoti setw, setprecision formatavimui
using namespace std;    // Kad nereiketu rasyti std:: pries cout cin ir tt

// Struktira, kurioje saugomas patiekalo pavadinimas ir kaina
struct menuItemType {
    string menuItem;    // Patiekalo pavadinimas
    double menuPrice;   // Patiekalo kaina
};


// Funkcija nuskaito meniu duomenis iš failo menu.txt

int getData(menuItemType menuList[]) {
    ifstream fin("menu.txt");   // Atidarome faila skaitymui

    // Tikriname ar failas atsidare
    if (!fin.is_open()) {
        cout << "Nepavyko atidaryti menu.txt failo!\n";
        return 0;               // Ismetamas 0 jei nepavyko nuskaityti failo
    }

    int count = 0;

    // getline skaito iki TAB ('\t'), po to skaitoma kaina
    while (fin >> ws && getline(fin, menuList[count].menuItem, '\t') &&
           fin >> menuList[count].menuPrice) {
        count++;                // Didiname patiekalu skaiciu
    }

    fin.close();                // Uzdarome faila
    return count;               // Graziname kiek patiekalu buvo nuskaityta ir uzsakyta
}


// Funkcija išveda visą meniu į ekraną

void showMenu(menuItemType menuList[], int menuSize) {
    cout << "\n--- PUSRYČIŲ MENIU ---\n";

    // Einame per visus patiekalus
    for (int i = 0; i < menuSize; i++) {
        cout << i + 1 << ". "                         // Patiekalo numeris
             << left << setw(40) << menuList[i].menuItem  // setw(40) – uzpildo iki 40 simboliu
             << fixed << setprecision(2)                  // 2 skaiciai po kablelio
             << menuList[i].menuPrice << " EUR\n";        // Kaina
    }

    cout << "\nPasirinkite patiekalo numeri (0 - baigti uzsakyma)\n";
}


// Funkcija isveda saskaitą i ekrana ir i faila receipt.txt

void printCheck(menuItemType menuList[], int pasirinkti[], int kiekiai[], int pasirinkimuKiekis) {
    ofstream fout("receipt.txt");   // Atidarome faila rasymui ir jo nuskaitymui

    cout << "\n--- SASKAITA ---\n";
    fout << "--- SASKAITA ---\n";

    double suma = 0;                // bendra suma

    // Einame per visus pasirinkimus
    for (int i = 0; i < pasirinkimuKiekis; i++) {
        int id = pasirinkti[i];     // Patiekalo skaicius
        int kiek = kiekiai[i];      // Porciju kiekis

        double tarpine = menuList[id].menuPrice * kiek; // Tarpine suma
        suma += tarpine;            // Pridedame prie bendros sumos

        // Isvedama i ekraną
        cout << left << setw(3) << kiek
             << setw(40) << menuList[id].menuItem
             << fixed << setprecision(2) << tarpine << " EUR\n";

        // Isvedama i faila
        fout << left << setw(3) << kiek
             << setw(40) << menuList[id].menuItem
             << fixed << setprecision(2) << tarpine << " EUR\n";
    }

    // Skaiciuojame PVM ir galutine suma
    double pvm = suma * 0.21;
    double galutine = suma + pvm;

    // Isvedami mokesciai
    cout << "\nMokesčiai (21%)" << setw(33) << " "
         << fixed << setprecision(2) << pvm << " EUR\n";

    cout << "Galutinė suma" << setw(35) << " "
         << fixed << setprecision(2) << galutine << " EUR\n";

    // Kartojama i faila
    fout << "\nMokesčiai (21%)" << setw(33) << " "
         << fixed << setprecision(2) << pvm << " EUR\n";

    fout << "Galutinė suma" << setw(35) << " "
         << fixed << setprecision(2) << galutine << " EUR\n";

    fout.close();   // Uzdarome faila
}

int main() {

    // Komanda pakeicia Windows konsoles koduote i UTF-8 ( pavercia skaicius ir simbolius i 0 ir 1 sekas )
    // Kad nebutu keistu neaiskiu simboliu
    system("chcp 65001 > nul");

    menuItemType menuList[50];   // Masyvas meniu duomenims
    int menuSize = getData(menuList); // Nuskaitome meniu

    // Jei nepavyko nuskaityti – stabdome programą
    if (menuSize == 0) {
        cout << "Meniu negautas. Programa stabdoma.\n";
        return 0;   // Baigiame programa
    }

    // Masyvai pasirinktiems patiekalams ir ju kiekiams
    int pasirinkti[100];
    int kiekiai[100];
    int pasirinkimuKiekis = 0;

    bool programIsRunning = true;    // Pagrindinis ciklo valdymas


    // Pagrindinis programos ciklas

    while (programIsRunning) {

        showMenu(menuList, menuSize);    // Rodome meniu

        int pasirinkimas;
        cout << "Pasirinkimas: ";
        cin >> pasirinkimas;             // Vartotojo pasirinkimas

        // Jei 0 – baigiame uzsakyma
        if (pasirinkimas == 0) {
            programIsRunning = false;
            break;
        }

        // Tikriname ar pasirinkimas teisingas
        if (pasirinkimas < 1 || pasirinkimas > menuSize) {
            cout << "Neteisingas pasirinkimas!\n";
            continue;
        }

        // Ivedame porciju kieki
        int kiek;
        cout << "Kiek porciju norite? ";
        cin >> kiek;

        if (kiek < 1) kiek = 1;  // Minimaliai 1 porcija

        // Irasome pasirinkima i masyvus
        pasirinkti[pasirinkimuKiekis] = pasirinkimas - 1;
        kiekiai[pasirinkimuKiekis] = kiek;
        pasirinkimuKiekis++;

        cout << "Patiekalas pridėtas!\n";
    }

    // Jei buvo pasirinkimu – spausdiname saskaita
    if (pasirinkimuKiekis > 0) {
        printCheck(menuList, pasirinkti, kiekiai, pasirinkimuKiekis);
        cout << "\nSaskaita issaugota faile receipt.txt\n";
    }
    else {
        cout << "Nieko neuzsisakete.\n";
    }

    return 0;   // Programa baigta
}
