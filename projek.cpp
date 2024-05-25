#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

using namespace std;

// Struktur data untuk menyimpan transaksi
struct Transaction {
    string type; // "income" atau "expense"
    string category;
    double amount;
    Transaction* next;
};

// Helper function to split the list into two halves
Transaction* split(Transaction* head) {
    Transaction* fast = head;
    Transaction* slow = head;
    Transaction* prev = nullptr;

    while (fast != nullptr && fast->next != nullptr) {
        prev = slow;
        slow = slow->next;
        fast = fast->next->next;
    }

    if (prev != nullptr) {
        prev->next = nullptr; // Split the list into two halves
    }

    return slow;
}

// Merge two sorted lists
Transaction* merge(Transaction* left, Transaction* right) {
    if (left == nullptr) return right;
    if (right == nullptr) return left;

    if (left->category < right->category) {
        left->next = merge(left->next, right);
        return left;
    } else {
        right->next = merge(left, right->next);
        return right;
    }
}

// Merge Sort implementation for linked list
void mergeSort(Transaction*& head) {
    if (head == nullptr || head->next == nullptr) return;

    Transaction* middle = split(head);
    mergeSort(head);
    mergeSort(middle);

    head = merge(head, middle);
}

// Algoritma pengurutan: Mengurutkan transaksi berdasarkan kategori
void sortTransactions(Transaction*& head) {
    mergeSort(head);
}


// Fungsi untuk menambahkan transaksi
void addTransaction(Transaction*& head, string type, string category, double amount) {
    Transaction* newNode = new Transaction;
    newNode->type = type;
    newNode->category = category;
    newNode->amount = amount;
    newNode->next = head;
    head = newNode;
}

// Algoritma pencarian: Mengembalikan total pengeluaran per kategori
double getTotalExpense(Transaction* head, string category) {
    double total = 0.0;
    Transaction* current = head;
    while (current != nullptr) {
        if (current->type == "expense" && current->category == category) {
            total += current->amount;
        }
        current = current->next;
    }
    return total;
}


// Fungsi untuk menyimpan transaksi ke file
void loadTransactionsFromFile(Transaction*& head, const string& filename) {
    ifstream file(filename);
    if (file.is_open()) {
        string type, category;
        double amount;
        while (file >> type >> category >> amount) {
            addTransaction(head, type, category, amount);
        }
        file.close();
    }
}

void saveTransactionsToFile(Transaction* head, const string& filename) {
    ofstream file(filename);
    if (file.is_open()) {
        Transaction* current = head;
        while (current != nullptr) {
            file << current->type << " " << current->category << " " << current->amount << endl;
            current = current->next;
        }
        file.close();
    }
}

// Fungsi untuk menghapus semua transaksi
void deleteTransactions(Transaction*& head) {
    while (head != nullptr) {
        Transaction* temp = head;
        head = head->next;
        delete temp;
    }
}
// Fungsi untuk mencetak transaksi dan total pengeluaran per kategori
// Fungsi untuk mencetak transaksi dan total pengeluaran per kategori

void tampilkan(){
    
}


int printTransactionsAndTotalExpense(Transaction*& head) {
    const string filename = "transactions.txt";

    // Muat transaksi dari file
    loadTransactionsFromFile(head, filename);

    int pilihan;
    do {
        string tipe; 
        string kategori;
        double jumlah;
        double totalIncome = 0.0;
        double totalExpense = 0.0;

        // Menampilkan pilihan transaksi
        cout << "\nAnda ingin melakukan transaksi apa? :" << endl;
        cout << "1. Pemasukan" << endl;
        cout << "2. Pengeluaran" << endl;
        cout << "3. Tampilkan Saldo" << endl; // Tambahkan pilihan untuk menampilkan saldo
        cout << "4. Hapus Semua Data" << endl;
        cout << "5. Keluar" << endl; // Tambahkan pilihan untuk keluar
        cout << "=>  ";
        cin >> pilihan;

        Transaction* current = head; 
        double balance = totalIncome - totalExpense;

        if (pilihan == 1 || pilihan == 2 || pilihan == 4 || pilihan == 5) {
            if (pilihan == 1) {
            tipe = "income"; // Ganti dari "Income" ke "income"
            kategori = "Salary";
            cout << "Masukkan jumlah: " << endl;
            cin >> jumlah;
            addTransaction(head, tipe, kategori, jumlah);
        } else if (pilihan == 2) {
            tipe = "expense"; // Ganti dari "Expense" ke "expense"
            cout << "Masukkan kategori transaksi: " << endl;
            cin >> kategori;
            cout << "Masukkan jumlah transaksi: " << endl;
            cin >> jumlah;

            // Hitung total saldo saat ini
            double currentBalance = 0.0;
            Transaction* current = head;
            while (current != nullptr) {
                if (current->type == "income") {
                    currentBalance += current->amount;
                } else if (current->type == "expense") {
                    currentBalance -= current->amount;
                }
                current = current->next;
            }

            // Cek apakah jumlah pengeluaran melebihi saldo saat ini
            if (jumlah > currentBalance) {
                cout << "Transaksi gagal. Jumlah pengeluaran melebihi saldo saat ini.\n";
            } else {
                addTransaction(head, tipe, kategori, jumlah);
            }
        }else if (pilihan == 4) {
            // Meminta konfirmasi sebelum menghapus semua data
            char confirm;
            cout << "Apakah Anda yakin ingin menghapus semua data? (y/n): ";
            cin >> confirm;

            if (confirm == 'y' || confirm == 'Y') {
                // Hapus linked list dan bebaskan memori
                deleteTransactions(head);
                cout << "Semua data telah dihapus.\n";
            } else {
                cout << "Penghapusan data dibatalkan.\n";
            }
        } else if (pilihan == 5) {
            // Keluar dari loop
            cout << "\nTerimakasih sudah menggunakan program kami!" << endl;
            cout << "Mengeluarkan..." << endl;
            break;
        }

        // Mencetak header tabel
        cout << "-------------------------------------------" << endl;
        cout << "| " << left << setw(10) << "Type" << "| " << setw(15) << "Category" << "| " << right << setw(10) << "Amount" << " |" << endl;
        cout << "-------------------------------------------" << endl;

        // Menggunakan ulang variabel current untuk mencetak transaksi
        Transaction* current = head; 

        // Mencetak transaksi dan menghitung total pengeluaran per kategori
        while (current != nullptr) {
            cout << "| " << left << setw(10) << current->type
                << "| " << setw(15) << current->category
                << "| " << right << setw(10) << current->amount << " |" << endl;
            if (current->type == "income") {
                totalIncome += current->amount;
            } else if (current->type == "expense") {
                totalExpense += current->amount;
            }
            current = current->next;
        }
        cout << "-----------------------------------------------------" << endl;

        // Menghitung saldo saat ini
        balance = totalIncome - totalExpense;
        cout << "Total Income: " << totalIncome << endl;
        cout << "Total Expense: " << totalExpense << endl;
        cout << "Current Balance: " << balance << endl;

        // Simpan transaksi ke file
        saveTransactionsToFile(head, filename);
        } else if (pilihan == 3) { // Tambahkan kondisi untuk menampilkan saldo;
            Transaction* current = head;
        while (current != nullptr) {
            if (current->type == "income") {
                totalIncome += current->amount;
            } else if (current->type == "expense") {
                totalExpense += current->amount;
            }
            current = current->next;
        }
        double balance = totalIncome - totalExpense;
        cout << "Saldo Anda saat ini: " << balance << endl;
        }  else {
            cout << "Pilihan anda tidak valid" << endl;
        }
    } while (true); // Loop terus menerus sampai pengguna memilih keluar (pilihan == 5)
    return 0;
}
int main() {
    Transaction* head = nullptr; // Inisialisasi linked list transaksi

    printTransactionsAndTotalExpense(head); // Panggil fungsi untuk mengelola transaksi

    // Bebaskan memori yang dialokasikan untuk linked list
    deleteTransactions(head);

    return 0;
}
