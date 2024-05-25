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
void saveTransactionsToFile(Transaction* head, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        Transaction* current = head;
        while (current != nullptr) {
            outFile << current->type << " " << current->category << " " << current->amount << "\n";
            current = current->next;
        }
        outFile.close();
    } else {
        cout << "Unable to open file for writing.\n";
    }
}

// Fungsi untuk membaca transaksi dari file
void loadTransactionsFromFile(Transaction*& head, const string& filename) {
    ifstream inFile(filename);
    if (inFile.is_open()) {
        string type, category;
        double amount;
        while (inFile >> type >> category >> amount) {
            addTransaction(head, type, category, amount);
        }
        inFile.close();
    } else {
        cout << "Unable to open file for reading.\n";
    }
}

// Fungsi untuk menghapus semua transaksi
void deleteTransactions(Transaction*& head) {
    Transaction* current = head;
    while (current != nullptr) {
        Transaction* next = current->next;
        delete current;
        current = next;
    }
    head = nullptr;
}

// Fungsi untuk mencetak transaksi dan total pengeluaran per kategori
// Fungsi untuk mencetak transaksi dan total pengeluaran per kategori
int printTransactionsAndTotalExpense(Transaction* head) {

    const string filename = "transactions.txt";

    // Muat transaksi dari file
    loadTransactionsFromFile(head, filename);

    // Mengurutkan transaksi berdasarkan kategori sebelum menghitung total pengeluaran
    sortTransactions(head);

    // Menyimpan total pengeluaran per kategori dalam array
    const int numCategories = 3; // Misalnya ada 3 kategori: Food, Transportation, Entertainment
    string categories[numCategories] = {"Food", "Transportation", "Entertainment"};
    double expenses[numCategories] = {0.0};
    int pilihan;
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
    cout << "=>  ";
    cin >> pilihan;
    if (pilihan == 1) {
        tipe = "income"; // Ganti dari "Income" ke "income"
        kategori = "Salary";
        cout << "Masukkan jumlah: " << endl;
        cin >> jumlah;
        addTransaction(head, tipe, kategori, jumlah);
    } if (pilihan == 2) {
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
}
        else if (pilihan == 3) { // Tambahkan kondisi untuk menampilkan saldo
        // Hitung saldo
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
        return 0; // Kembali tanpa mencetak transaksi
    } else if (pilihan == 4) {
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
    } else {
        cout << "Pilihan anda tidak valid" << endl;
        return 0;
    }

    // Mencetak header tabel
    cout << "-------------------------------------------" << endl;
    cout << "| " << left << setw(10) << "Type" << "| " << setw(15) << "Category" << "| " << right << setw(10) << "Amount" << " |" << endl;
    cout << "-------------------------------------------" << endl;

    // Mencetak transaksi dan menghitung total pengeluaran per kategori
    Transaction* current = head;
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
    cout << "-------------------------------------------" << endl;

    double balance = totalIncome - totalExpense;
    cout << "Total Income: " << totalIncome << endl;
    cout << "Total Expense: " << totalExpense << endl;
    cout << "Balance: " << balance << endl;

    // Simpan transaksi ke file sebelum keluar
    saveTransactionsToFile(head, filename);

    return 0;
}

int main() {
    Transaction* head = nullptr; // Head dari linked list transaksi

    // Menjalankan dan mencetak transaksi dan total pengeluaran per kategori
    printTransactionsAndTotalExpense(head);

    // Hapus linked list dan bebaskan memori
    deleteTransactions(head);

    return 0;
}