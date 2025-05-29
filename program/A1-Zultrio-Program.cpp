#include <iostream>
#include <string>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
using namespace std;

#define MaxTiket 40

struct Tiket {
    string Pembeli;
    string hari;
    string durasi;
    double harga;
    bool kausKaki;
    double totalHarga; 
    string status;
};

struct User {
    string username;
    string password;
    string role; 
};

User daftarUser[40] = { {"admin", "admin1", "admin"} };
int jumlahUser = 1;
Tiket daftarTiket[MaxTiket];
int jumlahTiket = 0;

const int MaxHari = 10;
const int MaxDurasi = 10;

string daftarHari[MaxHari] = {"Weekday", "Weekend"};
string daftarDurasi[MaxDurasi] = {"1 Jam", "2 Jam", "Sepuasnya"};
double hargaTiket[MaxHari][MaxDurasi] = {
    {20000, 35000, 45000},
    {25000, 40000, 50000} 
};

int jumlahHari = 2;
int jumlahDurasi = 3;

void registrasi(User *user);
bool login(User *user);
void menuTiketAdmin();
void menuTiketUser(const User& userLogin);
void menuSorting();
void simpanKeCSV(const string& namaFile, Tiket daftarTiket[], int jumlahTiket);
void beliTiketAdmin();
void beliTiketUser(const User& userLogin);
void tampilkanTiket();
void ubahTiket();
void hapusTiket();
void loadTiketFile();
void saveTiketFile();
void saveUserFile();

void bubbleSortHariDesc(Tiket arr[], int n) {
    for (int i = 0; i < n-1; i++) {
        for (int j = 0; j < n-i-1; j++) {
            if (arr[j].hari < arr[j+1].hari) {
                // tukar elemen
                Tiket temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

void selectionSortHargaAsc(Tiket arr[], int panjang) {
    for (int i = 0; i < panjang - 1; i++) {
        int min = i;
        for (int j = i + 1; j < panjang; j++) {
            if (arr[j].totalHarga < arr[min].totalHarga) {
                min = j;
            }
        }
        // tukar elemen
        Tiket temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }
}

void insertionSortDurasiAsc(Tiket arr[], int panjang) {
    for (int i = 1; i < panjang; i++) {
        Tiket key = arr[i];  
        int j = i - 1;
        
        while (j >= 0 && arr[j].durasi > key.durasi) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;  
    }
}

void loadUserFile() {
    ifstream file("user.csv");
    if (!file.is_open()) {
        cout << "Tidak dapat membuka file user.csv untuk memuat data" << endl;
        return;
    }
    
    string line;
    jumlahUser  = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        User userBaru;
        if (!getline(ss, userBaru.username, ',')) continue;
        if (!getline(ss, userBaru.password, ',')) continue;
        if (!getline(ss, userBaru.role)) continue;
        
        daftarUser [jumlahUser ++] = userBaru;
        if (jumlahUser  >= 40) break;
    }
    file.close();
}

void saveUserFile() {
    ofstream file("user.csv");
    if (!file.is_open()) {
        cout << "Tidak dapat membuka file user.csv untuk menyimpan data" << endl;
        return;
    }  
    for (int i = 0; i < jumlahUser ; i++) {
        User& user = daftarUser[i];
        file << user.username << ","
        << user.password << ","
        << user.role << "\n";
    }
    file.close();
}

void simpanKeCSV(const string& namaFile, Tiket daftarTiket[], int jumlahTiket) {
    ofstream file("tiket.csv");
    if (!file.is_open()) {
        return;
    }
    for (int i = 0; i < jumlahTiket; i++) {
        file << daftarTiket[i].Pembeli << ","
             << daftarTiket[i].hari << ","
             << daftarTiket[i].durasi << ","
             << daftarTiket[i].harga << ","
             << daftarTiket[i].kausKaki << ","
             << daftarTiket[i].totalHarga << ","
             << daftarTiket[i].status << "\n";
    }
    file.close();
}

void registrasi(User *user) { 
    system("cls");
    cout << "=== Registrasi ===" << endl;
    cin.ignore();
    while (true) {
        cout << "Masukkan Username: ";
        getline(cin, daftarUser [jumlahUser ].username);
        bool usernameExists = false;
        for (int i = 0; i < jumlahUser ; i++) {
            if (daftarUser [i].username == daftarUser [jumlahUser ].username) {
                usernameExists = true;
                break;
            }
        }
        if (usernameExists) {
            cout << "Username sudah terdaftar! Silakan gunakan username lain." << endl;
        } else if (daftarUser [jumlahUser ].username.length() < 3) {
            cout << "Username minimal 3 karakter!" << endl;
        } else {
            break;
        }
    }
    cout << "Masukkan Password: ";
    getline(cin, daftarUser [jumlahUser ].password);
    while (daftarUser [jumlahUser ].password.empty() || daftarUser [jumlahUser ].password.find_first_not_of(' ') == string::npos) {
        cout << "Password tidak boleh kosong atau hanya spasi!" << endl;
        cout << "Masukkan Password: ";
        getline(cin, daftarUser [jumlahUser ].password);
    }
    daftarUser [jumlahUser ].role = "user";
    *user = daftarUser [jumlahUser ];
    system("cls");
    cout << "Registrasi berhasil! Silakan lanjut login." << endl; 
    jumlahUser ++;   
    saveUserFile();
}

bool login(User *user) {
    system("cls");
    string inputUsername, inputPassword;
    int login = 0;
    while (login < 3) {
        cout << "=== Login ===" << endl;
        cout << "Masukkan Username: ";
        cin >> ws;
        getline(cin, inputUsername);
        cout << "Masukkan Password: ";
        cin >> ws;
        getline(cin, inputPassword);
        while (inputPassword.empty() || inputPassword.find_first_not_of(' ') == string::npos) {
            cout << "Password tidak boleh kosong atau hanya spasi! Masukkan ulang: ";
            getline(cin, inputPassword);
        }
        for (int i = 0; i < jumlahUser; i++) {
            if (inputUsername == daftarUser[i].username && inputPassword == daftarUser[i].password) {
                *user = daftarUser[i]; 
                system("cls");
                cout << "Login berhasil! Selamat datang " << user->username << "." << endl; 
                return true;
            }
        } 
        login++;
        if (login < 3) {
            cout << "Username atau password salah! Coba lagi" << endl;
        }
    }
    system("cls");
    cout << "Anda sudah mencoba 3 Kali. Program berhenti." << endl;
    exit(0);
}

void menuUtama(){
    User userLogin;
    User user;
    while (true){
        try{
            cout << "========================" << endl;
            cout << "|      MENU UTAMA      |" << endl;
            cout << "========================" << endl;
            cout << "1. Registrasi\n";
            cout << "2. Login\n";
            cout << "3. Keluar\n";
            cout << "Pilih Menu (1/2/3): ";
            int pilih;
            cin >> pilih; 
            if (cin.fail()) {
                system("cls");
                throw invalid_argument("Input harus berupa angka!");
            }    
            if (pilih == 1){
                registrasi(&user); 
            }
            else if(pilih == 2){
                if (login(&user)){
                    if (user.role == "admin") {
                        system("cls");
                        menuTiketAdmin();
                    } 
                    else {
                        menuTiketUser(user);
                    }
                }
            }
            else if (pilih == 3) { 
                cout << "Terima Kasih!" << endl;
                break;
            }
            else {
                system("cls");
                cout << "Pilihan tidak valid! Silakan pilih lagi." << endl;
            }
        }
        catch (const invalid_argument& e) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: " << e.what() << endl;
        }
    }
}

void beliTiketAdmin() {
    string NamaPembeli;
    cout << "=== MENU BELI TIKET ===" << endl;  
    Tiket tiketBaru;
    int inputHari, inputDurasi;
    char inputKausKaki;
    if (jumlahTiket < MaxTiket) {
        cin.ignore();
        while (true) {
            cout << "Nama pembeli: ";
            getline(cin, NamaPembeli);
            if (NamaPembeli.length() < 3) {
                system("cls");
                cout << "Nama Minimal 3 Karakter" << endl;
            }
            else if (NamaPembeli.find_first_not_of(' ') == string::npos) {
                system("cls");
                cout << "Nama Tidak Boleh Kosong atau Hanya Spasi!" << endl;
            }
            else {
                break;
            }
        }
        tiketBaru.Pembeli = NamaPembeli;
        while (true) {
            cout << "Pilih Hari: " << endl;
            for (int i = 0; i < jumlahHari; i++) {
                cout << i + 1 << ". " << daftarHari[i] << endl;
            }
            cout << "0 Untuk Membatalkan" << endl;
            cout << "Masukkan Hari (1-" << jumlahHari << "): ";
            cin >> inputHari;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Input harus berupa angka!" << endl;
                continue;
            }
            if (inputHari == 0) {
                system("cls");
                return;
            }
            else if (inputHari >= 1 && inputHari <= jumlahHari) {
                break;
            }
            else {
                cout << "Input tidak valid!" << endl;
            }
        }
        tiketBaru.hari = daftarHari[inputHari - 1];
        while (true) {
            cout << "Pilih Durasi:" << endl;
            for (int i = 0; i < jumlahDurasi; i++) {
                cout << i + 1 << ". " << daftarDurasi[i] << endl;
            }
            cout << "0 Untuk Membatalkan" << endl;
            cout << "Masukkan Durasi (1-" << jumlahDurasi << "): ";
            cin >> inputDurasi;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Input harus berupa angka!" << endl;
                continue;
            }
            if (inputDurasi == 0) {
                system("cls");
                return;
            }
            else if (inputDurasi >= 1 && inputDurasi <= jumlahDurasi) {
                break;
            }
            else {
                cout << "Input tidak valid!" << endl;
            }
        }
        tiketBaru.durasi = daftarDurasi[inputDurasi - 1];
        tiketBaru.harga = hargaTiket[inputHari - 1][inputDurasi - 1]; 
        while (true) {
            string inputKausKaki;
            cout << "Apakah Anda membawa kaus kaki? (Y/y untuk Ya, T/t untuk Tidak), input B untuk batal beli: ";
            cin >> inputKausKaki;
            if (inputKausKaki == "b" || inputKausKaki == "B") {
                cout << "Pembelian dibatalkan" << endl;
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
                return;
            }
            cin.ignore();
            if (inputKausKaki != "Y" && inputKausKaki != "y" && inputKausKaki != "T" && inputKausKaki != "t") {
                if (inputKausKaki.length() > 1);
                cout << "Input kaus kaki tidak valid!" << endl;
            }
            else {
                break;
            }
        }
        tiketBaru.kausKaki = (inputKausKaki == 'Y' || inputKausKaki == 'y');
        tiketBaru.totalHarga = tiketBaru.harga + (tiketBaru.kausKaki ? 0 : 10000);
        tiketBaru.status = "Aktif";
        daftarTiket[jumlahTiket++] = tiketBaru;
        cout << "Tiket berhasil ditambahkan!" << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.get();
        saveTiketFile();
        system("cls");
    } 
    else {
        cout << "Jumlah tiket sudah mencapai batas maksimum!" << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.ignore();
        cin.get();
    }
}

int interpolationSearch(Tiket arr[], int n, string NamaPembeli) {
    int low = 0;
    int high = n - 1;

    while (low <= high && NamaPembeli >= arr[low].Pembeli && NamaPembeli <= arr[high].Pembeli) {
        if (low == high) {
            if (arr[low].Pembeli == NamaPembeli) return low;
            return -1;
        }

        int pos = low + ((double)(high - low) /
                         (arr[high].Pembeli[0] - arr[low].Pembeli[0])) *
                        (NamaPembeli[0] - arr[low].Pembeli[0]);

        if (arr[pos].Pembeli == NamaPembeli)
            return pos;

        if (arr[pos].Pembeli < NamaPembeli)
            low = pos + 1;
        else
            high = pos - 1;
    }
    return -1;
}

void tampilkanTiket() {
    while (true) {
        cout << "=== DAFTAR TIKET ===" << endl;    
        if (jumlahTiket == 0) {
                cout << "Tidak ada tiket tersedia" << endl;
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
        } 
        else {
            cout << setw(10) << "No" << setw(15) << "Pembeli" << setw(15) << "Hari" << setw(15) << "Durasi" << setw(10) << "Harga" << setw(15) << "Kaus Kaki" << setw(15) << "Total Harga" << setw(15) << "Status" << endl;
            for (int i = 0; i < jumlahTiket; i++) {
                cout << setw(10) << (i + 1) 
                    << setw(15) << daftarTiket[i].Pembeli
                    << setw(15) << daftarTiket[i].hari
                    << setw(15) << daftarTiket[i].durasi
                    << setw(10) << daftarTiket[i].harga
                    << setw(15) << (daftarTiket[i].kausKaki ? "0" : "10.000")
                    << setw(15) << daftarTiket[i].totalHarga
                    << setw(15) << daftarTiket[i].status << endl;
            }
        }
        cout << "\n1. cari tiket" << endl;
        cout << "0. Keluar" << endl;
        cout << "Pilihan: ";
        int pilihan;
        cin >> pilihan;
        if (cin.fail()) {
            system("cls");
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input harus berupa angka!" << endl;
            continue;
        }
        if (pilihan == 1) {
            system("cls");
            Tiket temp[MaxTiket];
            for (int i = 0; i < jumlahTiket; i++) {
                temp[i] = daftarTiket[i];
            }
            int n = jumlahTiket;
            for (int i = 0; i < n - 1; i++) {
                for (int j = 0; j < n - i - 1; j++) {
                    if (temp[j].Pembeli > temp[j + 1].Pembeli) {
                        swap(temp[j], temp[j + 1]);
                    }
                }
            }
            string namaCari;
            cin.ignore();
            cout << "Masukkan nama pembeli yang ingin dicari: ";
            getline(cin, namaCari);
            int index = interpolationSearch(temp, jumlahTiket, namaCari);
            if (index == -1) {
                cout << "Tidak ada tiket ditemukan atas nama \"" << namaCari << "\"." << endl;
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.get();
                system("cls");
            } 
            else {
                int i = index;
                while (i > 0 && temp[i - 1].Pembeli == namaCari) i--;
                int count = 1;
                cout << "\nTiket ditemukan atas nama \"" << namaCari << "\":" << endl;
                cout << setw(10) << "No" << setw(15) << "Pembeli" << setw(15) << "Hari"
                    << setw(15) << "Durasi" << setw(10) << "Harga" << setw(15) << "Kaus Kaki"
                    << setw(15) << "Total Harga" << setw(15) << "Status" << endl;

                while (i < jumlahTiket && temp[i].Pembeli == namaCari) {
                    cout << setw(10) << count++
                        << setw(15) << temp[i].Pembeli
                        << setw(15) << temp[i].hari
                        << setw(15) << temp[i].durasi
                        << setw(10) << temp[i].harga
                        << setw(15) << (temp[i].kausKaki ? "0" : "10.000")
                        << setw(15) << temp[i].totalHarga
                        << setw(15) << temp[i].status << endl;
                    i++;
                }
                cout << "Tekan ENTER untuk melanjutkan...";
                cin.get();
                system("cls");
            }
        }
        else if (pilihan == 0) {
            system("cls");
            break;
        }
        else {
            system("cls");
            cout << "Pilihan Tidak Valid! \n" << endl;
        }
    }
}

void ubahTiket() {
    cout << "UBAH TIKET" << endl; 
    cout << "=== DAFTAR TIKET ===" << endl;
    if (jumlahTiket == 0) {
        cout << "Tidak ada tiket tersedia" << endl;
    } else {
        cout << setw(10) << "No" << setw(15) << "Pembeli" << setw(15) << "Hari" << setw(15) << "Durasi" << setw(10) << "Harga" << setw(15) << "Kaus Kaki" << setw(15) << "Total Harga" << setw(15) << "Status" << endl;
        for (int i = 0; i < jumlahTiket; i++) {
            cout << setw(10) << (i + 1) 
                 << setw(15) << daftarTiket[i].Pembeli
                 << setw(15) << daftarTiket[i].hari
                 << setw(15) << daftarTiket[i].durasi
                 << setw(10) << daftarTiket[i].harga
                 << setw(15) << (daftarTiket[i].kausKaki ? "0" : "10.000")
                 << setw(15) << daftarTiket[i].totalHarga
                 << setw(15) << daftarTiket[i].status << endl;
                }
    }  
    if(jumlahTiket == 0){
        cout << "Tidak ada tiket untuk diubah." << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.ignore();
        cin.get();
        system("cls");
        return;
    }
    int index;
    cout << "Masukkan Urutan Tiket yang ingin diubah (1-" << jumlahTiket << "): ";
    cin >> index;
    if (cin.fail() || index < 1 || index > jumlahTiket) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Index tidak valid!" << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.get();
        system("cls");
        return;
    }
    index--;
    Tiket &tiket = daftarTiket[index];
    cout << "\nData Tiket Ditemukan:" << endl;
    cout << "Nama: " << tiket.Pembeli << endl 
    << "Hari: " << tiket.hari << endl
    << "Durasi: " << tiket.durasi << endl
    << "Harga: " << tiket.harga << endl
    << "Kaus Kaki: " << (tiket.kausKaki ? "Ya" : "Tidak") << endl
    << "Total Harga: " << tiket.totalHarga << endl
    << "Status: " << tiket.status << endl;
    string konfirmasi;
    while (true) {
        cout << "\nApakah Anda yakin ingin mengubah tiket ini? (Y/y untuk Ya, T/t untuk Tidak): ";
        cin >> konfirmasi;
        if (konfirmasi != "Y" && konfirmasi != "y" && konfirmasi != "T" && konfirmasi != "t") {
            if (konfirmasi.length() > 1);
            system("cls");
            cout << "Input tidak valid!" << endl;
        }
        else if (konfirmasi == "Y" || konfirmasi == "y") {
            break;
        }
        else if (konfirmasi == "T" || konfirmasi == "t"){
            cout << "Perubahan dibatalkan." << endl;
            cout << "Tekan ENTER untuk melanjutkan....";
            cin.ignore();
            cin.get();
            system("cls");
            return;
        }
        else {
            cout << "Input Tidak Valid" << endl;
            cout << "Tekan ENTER untuk melanjutkan....";
            cin.get();
            system("cls");
        }
    }
    int inputHari, inputDurasi;
    string inputKausKaki;
    while (true) {
        cout << "Pilih Hari: " << endl;
        for (int i = 0; i < jumlahHari; i++) {
            cout << i + 1 << ". " << daftarHari[i] << endl;
        }
        cout << "0 Untuk Membatalkan" << endl;
        cout << "Masukkan Hari (1-" << jumlahHari << "): ";
        cin >> inputHari;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input harus berupa angka!" << endl;
            continue;
        }
        if (inputHari == 0) {
            system("cls");
            return;
        }
        else if (inputHari >= 1 && inputHari <= jumlahHari) {
            break;
        }
        else cout << "Input tidak valid!" << endl;
    }
    tiket.hari = daftarHari[inputHari - 1];
    while (true) {
        cout << "Pilih Durasi:" << endl;
        for (int i = 0; i < jumlahDurasi; i++) {
            cout << i + 1 << ". " << daftarDurasi[i] << endl;
        }
        cout << "0 Untuk Membatalkan" << endl;
        cout << "Masukkan Durasi (1-" << jumlahDurasi << "): ";
        cin >> inputDurasi;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input harus berupa angka!" << endl;
            continue;
        }
        if (inputDurasi == 0) {
            system("cls");
            return;
        }
        else if (inputDurasi >= 1 && inputDurasi <= jumlahDurasi) {
            break;
        }
        else cout << "Input tidak valid!" << endl;
    }
    tiket.durasi = daftarDurasi[inputDurasi - 1];
    tiket.harga = hargaTiket[inputHari-1][inputDurasi-1];
    while (true) {
            cout << "Apakah Anda membawa kaus kaki? (Y/y untuk Ya, T/t untuk Tidak), input B untuk batal beli: ";
            cin >> inputKausKaki;
            if (inputKausKaki == "b" || inputKausKaki == "B") {
                cout << "Pembelian dibatalkan" << endl;
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
                system("cls");
                return;
            }
            cin.ignore();
            if (inputKausKaki != "Y" && inputKausKaki != "y" && inputKausKaki != "T" && inputKausKaki != "t") {
                if (inputKausKaki.length() > 1);
                cout << "Input kaus kaki tidak valid!" << endl;
            }
            else {
                break;
            }
        }
    tiket.kausKaki = (inputKausKaki == "Y" || inputKausKaki == "y");
    tiket.totalHarga = tiket.harga + (tiket.kausKaki ? 0 : 10000);
    while (true){
        cout << "Masukkan status baru tiket (Aktif/Nonaktif): ";
        getline(cin, tiket.status);
        if (tiket.status == "Aktif" || tiket.status == "Nonaktif" || tiket.status == "aktif" || tiket.status == "nonaktif"){
            if (tiket.status == "aktif") {
                tiket.status = "Aktif";
            }
            else if (tiket.status == "nonaktif") {
                tiket.status = "Nonaktif";
            }
            break;
        }
        else{
            cout << "Status Tidak Valid" << endl;
        }
    }
    cout << "Tiket berhasil diubah!" << endl;
    cout << "Tekan ENTER untuk melanjutkan....";
    cin.get();
    saveTiketFile();
    system("cls");
}

void hapusTiket() {
    cout << "=== MENU HAPUS TIKET ===" << endl;  
    cout << "=== DAFTAR TIKET ===" << endl;
    if (jumlahTiket == 0) {
        cout << "Tidak ada tiket tersedia" << endl;
    } else {
        cout << setw(10) << "No" << setw(15) << "Pembeli" << setw(15) << "Hari" << setw(15) << "Durasi" << setw(10) << "Harga" << setw(15) << "Kaus Kaki" << setw(15) << "Total Harga" << setw(15) << "Status" << endl;
        for (int i = 0; i < jumlahTiket; i++) {
            cout << setw(10) << (i + 1) 
                 << setw(15) << daftarTiket[i].Pembeli
                 << setw(15) << daftarTiket[i].hari
                 << setw(15) << daftarTiket[i].durasi
                 << setw(10) << daftarTiket[i].harga
                 << setw(15) << (daftarTiket[i].kausKaki ? "0" : "10.000")
                 << setw(15) << daftarTiket[i].totalHarga
                 << setw(15) << daftarTiket[i].status << endl;
                }
    }  
    if (jumlahTiket == 0) {
        cout << "Tidak ada tiket untuk dihapus." << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.ignore();
        cin.get();
        system("cls");
        return;
    }
    int index;
    cout << "Masukkan Urutan Tiket yang ingin dihapus (1-" << jumlahTiket << "): ";
    cin >> index;
    if (cin.fail() || index < 1 || index > jumlahTiket) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Index tidak valid!" << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.get();
        system("cls");
        return;
    }
    index--;
    Tiket &tiket = daftarTiket[index];
    cout << "\nData Tiket Ditemukan:" << endl;
    cout << "Nama: " << tiket.Pembeli << endl 
    << "Hari: " << tiket.hari << endl
    << "Durasi: " << tiket.durasi << endl
    << "Harga: " << tiket.harga << endl
    << "Kaus Kaki: " << (tiket.kausKaki ? "Ya" : "Tidak") << endl
    << "Total Harga: " << tiket.totalHarga << endl
    << "Status: " << tiket.status << endl;
    string konfirmasi;
    while (true) {
        cout << "\nApakah Anda yakin ingin menghapus tiket ini? (Y/y untuk Ya, T/t untuk Tidak): ";
        cin >> konfirmasi;
        if (konfirmasi != "Y" && konfirmasi != "y" && konfirmasi != "T" && konfirmasi != "t") {
            if (konfirmasi.length() > 1);
            system("cls");
            cout << "Input kaus kaki tidak valid!" << endl;
        }
        else if (konfirmasi == "Y" || konfirmasi == "y") {
            for (int i = index; i < jumlahTiket - 1; i++) {
                daftarTiket[i] = daftarTiket[i + 1];
            }
            jumlahTiket--;
            cout << "Tiket berhasil dihapus!" << endl;
            saveTiketFile();
            cout << "Tekan ENTER untuk melanjutkan....";
            cin.ignore();
            cin.get();
            system("cls");
            return;
        }
        else if (konfirmasi == "T" || konfirmasi == "t"){
            cout << "Penghapusan dibatalkan." << endl;
            cout << "Tekan ENTER untuk melanjutkan....";
            cin.ignore();
            cin.get();
            system("cls");
            return;
        }
        else {
            cout << "Input Tidak Valid" << endl;
            cout << "Tekan ENTER untuk melanjutkan....";
            cin.get();
            system("cls");
        }
    }
}

void tampilkanTiketSort(Tiket arr[], int n) {
    cout << "=== HASIL SORTING TIKET ===" << endl;
    if (n == 0) {
        cout << "Tidak ada tiket tersedia" << endl;
    } else {
        cout << setw(10) << "No" << setw(15) << "Pembeli" << setw(15) << "Hari" << setw(15) << "Durasi" << setw(10) << "Harga" << setw(15) << "Kaus Kaki" << setw(15) << "Total Harga" << setw(15) << "Status" << endl;
        for (int i = 0; i < n; i++) {
            cout << setw(10) << (i + 1) 
                 << setw(15) << arr[i].Pembeli
                 << setw(15) << arr[i].hari
                 << setw(15) << arr[i].durasi
                 << setw(10) << arr[i].harga
                 << setw(15) << (arr[i].kausKaki ? "0" : "10.000")
                 << setw(15) << arr[i].totalHarga
                 << setw(15) << arr[i].status << endl;
        }
    }
}

void menuSorting() {
    Tiket tempTiket[MaxTiket];
    for (int i = 0; i < jumlahTiket; i++) {
        tempTiket[i] = daftarTiket[i];
    }
    while (true) {
        system("cls");
        cout << "\n=========================" << endl;
        cout << "|      MENU SORTING     |" << endl;
        cout << "=========================" << endl;
        cout << "1. Sorting berdasarkan Hari (Descending) - Bubble Sort" << endl;
        cout << "2. Sorting Harga (Ascending) - Selection Sort" << endl;
        cout << "3. Sorting berdasarkan Durasi (Ascending) - Insertion Sort" << endl;
        cout << "4. Keluar ke Menu Tiket" << endl;
        cout << "Pilih metode sorting (1-4): ";
        int pilihan;
        cin >> pilihan;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: Input harus berupa angka!" << endl;
            cout << "Tekan ENTER untuk melanjutkan...";
            cin.get();
            continue;
        }
        switch (pilihan) {
            case 1:
                bubbleSortHariDesc(tempTiket, jumlahTiket);
                tampilkanTiketSort(tempTiket, jumlahTiket);
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
                break;
            case 2:
                selectionSortHargaAsc(tempTiket, jumlahTiket);
                tampilkanTiketSort(tempTiket, jumlahTiket);
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
                break;
            case 3:
                insertionSortDurasiAsc(tempTiket, jumlahTiket);
                tampilkanTiketSort(tempTiket, jumlahTiket);
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
                break;
            case 4:
            system("cls");
                return;
            default:
                cout << "Pilihan tidak valid!\n";
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
        }
    }
}

void ubahUser () {   
    cout << "Daftar User yang ada:" << endl;
    for (int i = 0; i < jumlahUser ; i++) {
        cout << i + 1 << ". " << daftarUser [i].username << " (Role: " << daftarUser [i].role << ")" << endl;
    }
    cout << "0 Untuk Membatalkan" << endl;
    string username;
    cout << "Masukkan Username yang ingin diubah: ";
    cin.ignore(); 
    getline(cin, username);
    if (username == "0") {
        system("cls");
        return;
    }
    for (int i = 0; i < jumlahUser ; i++) {
        if (daftarUser [i].username == username) {
            system("cls");
            cout << "Data User Ditemukan:" << endl;
            cout << "Username: " << daftarUser [i].username << endl;
            cout << "Role: " << daftarUser [i].role << endl;
            
            string newPassword;
            cout << "Masukkan Password baru (kosongkan untuk tidak mengubah): ";
            getline(cin, newPassword);
            if (!newPassword.empty()) {
                daftarUser [i].password = newPassword;
            }

            string newRole;
            cout << "Masukkan Role baru (admin/user, kosongkan untuk tidak mengubah): ";
            getline(cin, newRole);
            if (!newRole.empty()) {
                daftarUser [i].role = newRole;
            }
            
            cout << "User  berhasil diubah!" << endl;
            saveUserFile();
            system("cls");
            return;
        }
    }
    cout << "User  dengan username tersebut tidak ditemukan!" << endl;
    cout << "Tekan ENTER untuk melanjutkan....";
    cin.get();
    system("cls");
}

void hapusUser () {
    for (int i = 0; i < jumlahUser ; i++) {
        cout << i + 1 << ". " << daftarUser [i].username << " (Role: " << daftarUser [i].role << ")" << endl;
    }
    cout << "0 Untuk Membatalkan" << endl;
    string username;
    cout << "Masukkan Username yang ingin dihapus: ";
    cin.ignore();
    getline(cin, username);
    if (username == "0") {
        return;
    }
    for (int i = 0; i < jumlahUser ; i++) {
        if (daftarUser [i].username == username) {
            system("cls");
            cout << "Data User Ditemukan:" << endl;
            cout << "Username: " << daftarUser [i].username << endl;
            cout << "Role: " << daftarUser [i].role << endl;
            
            char konfirmasi;
            cout << "Apakah Anda yakin ingin menghapus user ini? (Y/y untuk Ya, T/t untuk Tidak): ";
            cin >> konfirmasi;
            if (konfirmasi == 'Y' || konfirmasi == 'y') {
                for (int j = i; j < jumlahUser  - 1; j++) {
                    daftarUser [j] = daftarUser [j + 1];
                }
                jumlahUser --;
                cout << "User  berhasil dihapus!" << endl;
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
                saveUserFile();
                system("cls");
                return;
            } else {
                cout << "Penghapusan dibatalkan." << endl;
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
                system("cls");
                return;
            }
        }
    }
    cout << "User  dengan username tersebut tidak ditemukan!" << endl;
    cout << "Tekan ENTER untuk melanjutkan....";
    cin.get();
    system("cls");
}

void kelolaHargaTiket() {
    while (true) {
        system("cls");
        cout << "=== KELOLA HARGA TIKET  ===\n";
        cout << "1. Lihat Harga" << endl;
        cout << "2. Tambah Hari" << endl;
        cout << "3. Hapus Hari" << endl;
        cout << "4. Tambah Durasi" << endl;
        cout << "5. Hapus Durasi" << endl;
        cout << "6. Ubah Harga Tiket" << endl;
        cout << "0. Kembali" << endl;
        cout << "Pilihan: ";
        int pilihan;
        cin >> pilihan;
        if (cin.fail()) {
            cout << "Input Harus Berupa Angka!" << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Tekan ENTER...";
            cin.get();
            continue;
        }
        if (pilihan == 1) {
            cout << setw(15) << "Durasi/Hari";
            for (int h = 0; h < jumlahHari; h++) {
                cout << setw(15) << daftarHari[h];
            }
            cout << endl;
            for (int d = 0; d < jumlahDurasi; d++) {
                cout << setw(15) << daftarDurasi[d];
                for (int h = 0; h < jumlahHari; h++) {
                    cout << setw(15) << hargaTiket[h][d];
                }
                cout << endl;
            }
            cin.ignore();
            cout << "Tekan ENTER...";
            cin.get();
        }
        else if (pilihan == 2) {
            if (jumlahHari >= MaxHari) {
                cout << "Maksimal hari tercapai!\n";
                cin.ignore();
                cin.get();
                continue;
            }
            cout << "Nama hari baru: ";
            cin.ignore(); getline(cin, daftarHari[jumlahHari]);
            for (int d = 0; d < jumlahDurasi; d++) {
                cout << "Harga untuk durasi " << daftarDurasi[d] << ": ";
                cin >> hargaTiket[jumlahHari][d];
            }
            jumlahHari++;
        }
        else if (pilihan == 3) {
            for (int i = 0; i < jumlahHari; i++) {
                cout << i + 1 << ". " << daftarHari[i] << endl;
            }
            cout << "Pilih hari yang ingin dihapus: ";
            int idx; cin >> idx;
            if (idx > 0 && idx <= jumlahHari) {
                for (int i = idx - 1; i < jumlahHari - 1; i++) {
                    daftarHari[i] = daftarHari[i + 1];
                    for (int d = 0; d < jumlahDurasi; d++) {
                        hargaTiket[i][d] = hargaTiket[i + 1][d];
                    }
                }
                jumlahHari--;
            }
        }
        else if (pilihan == 4) {
            if (jumlahDurasi >= MaxDurasi) {
                cout << "Maksimal durasi tercapai!\n";
                cin.ignore();
                cin.get();
                continue;
            }
            cout << "Nama durasi baru: ";
            cin.ignore(); getline(cin, daftarDurasi[jumlahDurasi]);
            for (int h = 0; h < jumlahHari; h++) {
                cout << "Harga untuk " << daftarHari[h] << ": ";
                cin >> hargaTiket[h][jumlahDurasi];
            }
            jumlahDurasi++;
        }
        else if (pilihan == 5) {
            for (int i = 0; i < jumlahDurasi; i++) {
                cout << i + 1 << ". " << daftarDurasi[i] << endl;
            }
            cout << "Pilih durasi yang ingin dihapus: ";
            int idx; cin >> idx;
            if (idx > 0 && idx <= jumlahDurasi) {
                for (int i = idx - 1; i < jumlahDurasi - 1; i++) {
                    daftarDurasi[i] = daftarDurasi[i + 1];
                    for (int h = 0; h < jumlahHari; h++) {
                        hargaTiket[h][i] = hargaTiket[h][i + 1];
                    }
                }
                jumlahDurasi--;
            }
        }
        else if (pilihan == 6) {
            for (int h = 0; h < jumlahHari; h++)
                cout << h + 1 << ". " << daftarHari[h] << endl;
            int pilihHari;
            cout << "Pilih hari: "; cin >> pilihHari;

            for (int d = 0; d < jumlahDurasi; d++)
                cout << d + 1 << ". " << daftarDurasi[d] << endl;
            int pilihDurasi;
            cout << "Pilih durasi: "; cin >> pilihDurasi;

            if (pilihHari >= 1 && pilihHari <= jumlahHari && pilihDurasi >= 1 && pilihDurasi <= jumlahDurasi) {
                cout << "Masukkan harga baru: ";
                cin >> hargaTiket[pilihHari - 1][pilihDurasi - 1];
            }
        }
        else if (pilihan == 0) {
            system("cls");
            break;
        }
        else {
            cout << "Pilihan tidak valid. Tekan ENTER...";
            cin.ignore();
            cin.get();
        }
    }
}

void menuTiketAdmin() {
    while (true) {
        try {
            cout << "=========================" << endl;
            cout << "|    MENU TIKET ADMIN   |" << endl;
            cout << "=========================" << endl;
            cout << "1. Beli Tiket" << endl;
            cout << "2. Tampilkan Tiket" << endl;
            cout << "3. Ubah data Tiket" << endl;
            cout << "4. Hapus Tiket" << endl;
            cout << "5. Menu Sorting tiket" << endl;
            cout << "6. Ubah User" << endl;
            cout << "7. Hapus User" << endl;
            cout << "8. Kelola Harga Tiket" << endl;
            cout << "9. Keluar ke menu utama" << endl;
            cout << "Pilih menu (1/2/3/4/5/6/7/8/9): ";
            int pilihan;
            cin >> pilihan;
            if (cin.fail()) {
                throw invalid_argument("Input harus berupa angka!");
            }
            switch (pilihan) {
                case 1:
                system("cls");
                cout << "=======================================================" << endl;
                cout << "|          DAFTAR HARGA TIKET MINI PLAYGROUND         |" << endl;
                cout << "=======================================================" << endl;
                cout << "| Hari       | Paket Durasi       | Harga Tiket       |" << endl;
                cout << "-------------------------------------------------------" << endl;
                for (int h = 0; h < jumlahHari; h++) {
                    bool firstRow = true;
                    for (int d = 0; d < jumlahDurasi; d++) {
                        cout << "| " << setw(10) << (firstRow ? daftarHari[h] : "") 
                            << " | " << setw(18) << daftarDurasi[d]
                            << " | Rp" << fixed << setprecision(0) << setw(13) << hargaTiket[h][d] << " |" << endl;
                        firstRow = false;
                    }
                    cout << "-------------------------------------------------------" << endl;
                }
                cout << "| Kaus Kaki (Wajib, jika tidak bawa) | Rp10.000       |" << endl;
                cout << "=======================================================" << endl;
                beliTiketAdmin();
                break;
                case 2:
                system("cls");
                tampilkanTiket();
                break;
                case 3:
                system("cls");
                ubahTiket();
                break;
                case 4:
                system("cls");
                hapusTiket();
                break;
                case 5:
                system("cls");
                menuSorting();
                break;
                case 6:
                system("cls");
                ubahUser ();
                break;
                case 7:
                system("cls");
                hapusUser ();
                break;
                case 8:
                system("cls");
                kelolaHargaTiket();
                break;
                case 9:
                system("cls");
                return;
                default:
                system("cls");
                cout << "Pilihan tidak valid" << endl;
            }
        }
        catch (const invalid_argument& e) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: " << e.what() << endl;
        }
    }
}

void beliTiketUser(const User& userLogin) {
    cout << "=== MENU BELI TIKET ===" << endl;
    if (jumlahTiket < MaxTiket) {
        Tiket tiketBaru;
        int inputHari, inputDurasi;
        char inputKausKaki;
        while (true) {
            cout << "Pilih Hari: " << endl;
            for (int i = 0; i < jumlahHari; i++) {
                cout << i + 1 << ". " << daftarHari[i] << endl;
            }
            cout << "0 Untuk Membatalkan" << endl;
            cout << "Masukkan Hari (1-" << jumlahHari << "): ";
            cin >> inputHari;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Input harus berupa angka!" << endl;
                continue;
            }
            if (inputHari == 0) {
                return;
            }
            else if (inputHari >= 1 && inputHari <= jumlahHari) {
                break;
            }
            else cout << "Input tidak valid!" << endl;
        }
        tiketBaru.hari = daftarHari[inputHari - 1];
        while (true) {
            cout << "Pilih Durasi:" << endl;
            for (int i = 0; i < jumlahDurasi; i++) {
                cout << i + 1 << ". " << daftarDurasi[i] << endl;
            }
            cout << "0 Untuk Membatalkan" << endl;
            cout << "Masukkan Durasi (1-" << jumlahDurasi << "): ";
            cin >> inputDurasi;
            if (cin.fail()) {
                cin.clear();
                cin.ignore(1000, '\n');
                cout << "Input harus berupa angka!" << endl;
                continue;
            }
            if (inputDurasi == 0) {
                return;
            }
            else if (inputDurasi >= 1 && inputDurasi <= jumlahDurasi) {
                break;
            }
            else cout << "Input tidak valid!" << endl;
        }
        tiketBaru.durasi = daftarDurasi[inputDurasi - 1];
        tiketBaru.harga = hargaTiket[inputHari - 1][inputDurasi - 1]; 
        while (true) {
            string inputKausKaki;
            cout << "Apakah Anda membawa kaus kaki? (Y/y untuk Ya, T/t untuk Tidak), input B untuk batal beli: ";
            cin >> inputKausKaki;
            if (inputKausKaki == "b" || inputKausKaki == "B") {
                cout << "Pembelian dibatalkan" << endl;
                cout << "Tekan ENTER untuk melanjutkan....";
                cin.ignore();
                cin.get();
                return;
            }
            cin.ignore();
            if (inputKausKaki != "Y" && inputKausKaki != "y" && inputKausKaki != "T" && inputKausKaki != "t") {
                if (inputKausKaki.length() > 1);
                cout << "Input kaus kaki tidak valid!" << endl;
            }
            else {
                break;
            }
        }
        tiketBaru.kausKaki = (inputKausKaki == 'Y' || inputKausKaki == 'y');
        tiketBaru.totalHarga = tiketBaru.harga + (tiketBaru.kausKaki ? 0 : 10000);
        tiketBaru.status = "Aktif";
        tiketBaru.Pembeli = userLogin.username;
        daftarTiket[jumlahTiket++] = tiketBaru;
        cout << "Tiket berhasil ditambahkan!" << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.get();
        saveTiketFile();
    } 
    else {
        cout << "Jumlah tiket sudah mencapai batas maksimum!" << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.ignore();
        cin.get();
    }
}

void tampilkanTiketUser(const User& userLogin) {
    cout << "=== TIKET YANG Dipesan oleh " << userLogin.username << " ===" << endl;    
    bool found = false;
    for (int i = 0; i < jumlahTiket; i++) {
        if (daftarTiket[i].Pembeli == userLogin.username) {
            if (!found) {
                cout << setw(15) << "Pembeli" << setw(15) << "Hari" << setw(15) << "Durasi" 
                << setw(10) << "Harga" << setw(15) << "Kaus Kaki" 
                << setw(15) << "Total Harga" << setw(15) << "Status" << endl;
                found = true;
            }
            cout << setw(15) << daftarTiket[i].Pembeli
            << setw(15) << daftarTiket[i].hari
            << setw(15) << daftarTiket[i].durasi
            << setw(10) << daftarTiket[i].harga
            << setw(15) << (daftarTiket[i].kausKaki ? "0" : "10.000")
            << setw(15) << daftarTiket[i].totalHarga
            << setw(15) << daftarTiket[i].status << endl;
        }
    }
    if (!found) {
        cout << "Tidak ada tiket yang dipesan oleh " << userLogin.username << "." << endl;
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.ignore();
        cin.get();
    }
    else {
        cout << "Tekan ENTER untuk melanjutkan....";
        cin.ignore();
        cin.get();
    }
}

void menuTiketUser(const User& userLogin) {
    while (true) {
        try{
            system("cls");
            cout << "========================" << endl;
            cout << "|    MENU TIKET USER   |" << endl;
            cout << "========================" << endl;
            cout << "1. Beli Tiket" << endl;
            cout << "2. Tampilkan Tiket" << endl;
            cout << "3. Keluar ke menu utama" << endl;
            cout << "Pilih menu (1/2/3): ";
            int pilihan;
            cin >> pilihan;
            if (cin.fail()) {
                throw invalid_argument("Input harus berupa angka!");
                }
            switch (pilihan) {
                case 1:
                    system("cls");
                    cout << "=======================================================" << endl;
                    cout << "|          DAFTAR HARGA TIKET MINI PLAYGROUND         |" << endl;
                    cout << "=======================================================" << endl;
                    cout << "| Hari       | Paket Durasi       | Harga Tiket       |" << endl;
                    cout << "-------------------------------------------------------" << endl;
                    for (int h = 0; h < jumlahHari; h++) {
                        bool firstRow = true;
                        for (int d = 0; d < jumlahDurasi; d++) {
                            cout << "| " << setw(10) << (firstRow ? daftarHari[h] : "") 
                                << " | " << setw(18) << daftarDurasi[d]
                                << " | Rp" << fixed << setprecision(0) << setw(13) << hargaTiket[h][d] << " |" << endl;
                            firstRow = false;
                        }
                        cout << "-------------------------------------------------------" << endl;
                    }
                    cout << "| Kaus Kaki (Wajib, jika tidak bawa) | Rp10.000       |" << endl;
                    cout << "=======================================================" << endl;
                    beliTiketUser(userLogin);
                    break;
                case 2:
                    system("cls");
                    tampilkanTiketUser (userLogin);
                    break;
                case 3:
                    system("cls");
                    return;
                default:
                    cout << "Pilihan tidak valid" << endl;
            }
        }
        catch (const invalid_argument& e) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Error: " << e.what() << endl;
        }
    }
}

void loadTiketFile() {
    ifstream file("tiket.csv");
    if (!file.is_open()) {
        return;
    }
    string line;
    jumlahTiket = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        Tiket tiketBaru;
        string kausKakiStr;
        string hargaStr, totalHargaStr;
        if(!getline(ss, tiketBaru.Pembeli, ',')) continue;
        if(!getline(ss, tiketBaru.hari, ',')) continue;
        if(!getline(ss, tiketBaru.durasi, ',')) continue;
        if(!getline(ss, hargaStr, ',')) continue;
        tiketBaru.harga = stod(hargaStr);

        if(!getline(ss, kausKakiStr, ',')) continue;
        tiketBaru.kausKaki = (kausKakiStr == "1");

        if(!getline(ss, totalHargaStr, ',')) continue;
        tiketBaru.totalHarga = stod(totalHargaStr);

        if(!getline(ss, tiketBaru.status)) continue;

        daftarTiket[jumlahTiket++] = tiketBaru;
        if(jumlahTiket >= MaxTiket) break;
    }
    file.close();
}

void saveTiketFile() {
    ofstream file("tiket.csv");
    if (!file.is_open()) {
        cout << "Tidak dapat membuka file tiket.csv untuk menyimpan data" << endl;
        return;
    }
    for (int i = 0; i < jumlahTiket; i++) {
        Tiket& tiket = daftarTiket[i];
        file << tiket.Pembeli << ","
             << tiket.hari << ","
             << tiket.durasi << ","
             << tiket.harga << ","
             << (tiket.kausKaki ? "1" : "0") << ","
             << tiket.totalHarga << ","
             << tiket.status << "\n";
    }
    file.close();
}

int main() {
    srand(time(0));
    loadUserFile();
    loadTiketFile();
    cout << "===============================================================" << endl;
    cout << "| SELAMAT DATANG DI APLIKASI PEMBELIAN TIKET MINI PLAYGROUND  |" << endl;
    cout << "===============================================================" << endl;
    cout << " Silakan registrasi atau login untuk mulai menggunakan.\n";
    menuUtama();
    return 0;
}