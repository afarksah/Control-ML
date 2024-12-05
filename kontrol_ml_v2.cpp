#include <windows.h>
#include <iostream>
#include <conio.h>
#include <map>
#include <fstream>
#include <string>
using namespace std;

// Struktur untuk menyimpan koordinat
struct Coordinate {
    int x, y;
    Coordinate(int _x = 0, int _y = 0) : x(_x), y(_y) {}
};

// Fungsi untuk melakukan klik pada koordinat tertentu
void simulateClick(int x, int y) {
    double screenWidth = GetSystemMetrics(SM_CXSCREEN);
    double screenHeight = GetSystemMetrics(SM_CYSCREEN);

    DWORD normalized_x = static_cast<DWORD>((x * 65535) / screenWidth);
    DWORD normalized_y = static_cast<DWORD>((y * 65535) / screenHeight);

    INPUT input[2] = {};

    input[0].type = INPUT_MOUSE;
    input[0].mi.dx = normalized_x;
    input[0].mi.dy = normalized_y;
    input[0].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE | MOUSEEVENTF_LEFTDOWN;

    input[1].type = INPUT_MOUSE;
    input[1].mi.dx = normalized_x;
    input[1].mi.dy = normalized_y;
    input[1].mi.dwFlags = MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP;

    SendInput(2, input, sizeof(INPUT));
}

// Fungsi untuk menyimpan koordinat ke file
void saveCoordinates(const map<char, Coordinate>& coords) {
    ofstream file("ml_coordinates.txt");
    if (file.is_open()) {
        for (const auto& pair : coords) {
            file << pair.first << " " << pair.second.x << " " << pair.second.y << endl;
        }
        cout << "Koordinat berhasil disimpan!\n";
    } else {
        cerr << "Error: Tidak dapat menyimpan koordinat!\n";
    }
}

// Fungsi untuk memuat koordinat dari file
bool loadCoordinates(map<char, Coordinate>& coords) {
    ifstream file("ml_coordinates.txt");
    if (file.is_open()) {
        char key;
        int x, y;
        coords.clear();
        while (file >> key >> x >> y) {
            coords[key] = Coordinate(x, y);
        }
        return true;
    }
    return false;
}

// Fungsi untuk menampilkan koordinat atersimpan
void displayCoordinates(const map<char, Coordinate>& coords) {
    if (coords.empty()) {
        cout << "Tidak ada koordinat yang tersimpan.\n";
        return;
    }
    cout << "Koordinat tersimpan:\n";
    for (const auto& pair : coords) {
        cout << pair.first << ": (" << pair.second.x << ", " << pair.second.y << ")\n";
    }
}

// Fungsi untuk mengatur koordinat skill
map<char, Coordinate> setupCoordinates() {
    map<char, Coordinate> coords;

    cout << "Pengaturan koordinat skill Mobile Legends\n";
    cout << "----------------------------------------\n";
    cout << "Masukkan koordinat untuk setiap skill (format: x y):\n";

    int x, y;

    for (const auto& skill : {'Z', 'X', 'C', 'V'}) {
        cout << "Masukkan koordinat untuk skill '" << skill << "': ";
        cin >> x >> y;
        while (x < 0 || y < 0) {
            cerr << "Koordinat tidak valid! Masukkan lagi: ";
            cin >> x >> y;
        }
        coords[skill] = Coordinate(x, y);
    }

    saveCoordinates(coords);
    return coords;
}

// Fungsi untuk menampilkan menu kontrol
void displayMenu() {
    cout << "Mobile Legends Keyboard Controller\n";
    cout << "---------------------------------\n";
    cout << "Kontrol yang tersedia:\n";
    cout << "Z: Basic Attack\n";
    cout << "X: Skill 1\n";
    cout << "C: Skill 2\n";
    cout << "V: Ultimate\n";
    cout << "R: Reset Koordinat\n";
    cout << "Q: Hapus Layar\n";
    cout << "K: Tampilkan Koordinat Tersimpan\n";
    cout << "ESC: Keluar\n\n";
}

int main() {
    map<char, Coordinate> skillCoords;

    cout << "Mencoba memuat koordinat tersimpan...\n";
    if (!loadCoordinates(skillCoords)) {
        cout << "Tidak ada koordinat tersimpan. Memulai pengaturan baru.\n";
        skillCoords = setupCoordinates();
    } else {
        cout << "Koordinat berhasil dimuat!\n";
    }

    system("cls");
    displayMenu();

    bool running = true;
    char key;

    while (running) {
        if (_kbhit()) {
            key = toupper(_getch());

            if (key == 'R') {
                skillCoords = setupCoordinates();
                system("cls");
                displayMenu();
            } else if (key == 'Q') {
                system("cls");
                displayMenu();
            } else if (key == 'K') {
                system("cls");
                displayCoordinates(skillCoords);
                cout << "\nTekan tombol apa saja untuk kembali ke menu...\n";
                _getch();
                system("cls");
                displayMenu();
            } else if (skillCoords.find(key) != skillCoords.end()) {
                cout << "Menggunakan skill pada koordinat ("
                     << skillCoords[key].x << ", " << skillCoords[key].y << ")\n";
                simulateClick(skillCoords[key].x, skillCoords[key].y);
            } else if (key == 27) { // ESC
                running = false;
            }
        }
    }

    cout << "\nProgram selesai!\n";
    return 0;
}
