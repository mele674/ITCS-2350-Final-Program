//Marc Melendez
//ITCS 2530
//https://github.com/mele674/ITCS-2350-Final-Program
// This program will allow users to track car maintenance records, including adding, viewing, searching, editing, and deleting records. Along with file handling to save and load records.
// My posted
//AI Tool Used: ChatGPT


#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <algorithm>
#include <limits>

using namespace std;

const int MAX_RECORDS = 100;

struct MaintenanceRecord {
    string date;
    string part;
    string notes;
};

class MaintenanceManager {
private:
    MaintenanceRecord records[MAX_RECORDS];
    int recordCount;
    string filename;

public:
    MaintenanceManager(string file) : filename(file), recordCount(0) {
        loadFromFile();
    }

    void displayMenu();
    void addRecord();
    void searchRecord();
    void editRecord();
    void deleteRecord();
    void showAllRecords();
    void showUpcomingMaintenance();
    void showPartsToPurchase();
    void saveToFile();

private:
    void loadFromFile();
    string toLower(const string& str);
    string getCurrentTime();
    void clearInput();
};

// Converts string to lowercase for case-insensitive comparison
string MaintenanceManager::toLower(const string& str) {
    string lowerStr = str;
    transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(), ::tolower);
    return lowerStr;
}

// Gets current time using localtime_s
string MaintenanceManager::getCurrentTime() {
    time_t now = time(0);
    struct tm localTime;
    char buffer[80];
    localtime_s(&localTime, &now);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &localTime);
    return string(buffer);
}

void MaintenanceManager::clearInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void MaintenanceManager::loadFromFile() {
    ifstream infile(filename);
    recordCount = 0;
    while (infile && recordCount < MAX_RECORDS) {
        getline(infile, records[recordCount].date);
        getline(infile, records[recordCount].part);
        getline(infile, records[recordCount].notes);
        if (!records[recordCount].date.empty())
            recordCount++;
    }
    infile.close();
}

void MaintenanceManager::saveToFile() {
    ofstream outfile(filename);
    for (int i = 0; i < recordCount; ++i) {
        outfile << records[i].date << endl;
        outfile << records[i].part << endl;
        outfile << records[i].notes << endl;
    }
    outfile.close();
}

void MaintenanceManager::addRecord() {
    if (recordCount >= MAX_RECORDS) {
        cout << "Record limit reached. Cannot add more.\n";
        return;
    }

    cout << "Enter date (YYYY-MM-DD): ";
    cin.ignore();
    getline(cin, records[recordCount].date);
    cout << "Enter car part: ";
    getline(cin, records[recordCount].part);
    cout << "Enter notes: ";
    getline(cin, records[recordCount].notes);

    recordCount++;
    saveToFile();
    cout << "Record added successfully.\n";
}

void MaintenanceManager::searchRecord() {
    string keyword;
    cout << "Enter keyword to search (part or date): ";
    cin.ignore();
    getline(cin, keyword);
    keyword = toLower(keyword);
    bool found = false;
    for (int i = 0; i < recordCount; ++i) {
        if (toLower(records[i].part).find(keyword) != string::npos ||
            toLower(records[i].date).find(keyword) != string::npos) {
            cout << "\nRecord " << i + 1 << ":\n";
            cout << "Date: " << records[i].date << endl;
            cout << "Part: " << records[i].part << endl;
            cout << "Notes: " << records[i].notes << endl;
            found = true;
        }
    }
    if (!found)
        cout << "No matching record found.\n";
}

void MaintenanceManager::editRecord() {
    int index;
    cout << "Enter record number to edit (1 to " << recordCount << "): ";
    cin >> index;
    if (cin.fail()) {
        clearInput();
        cout << "Invalid input.\n";
        return;
    }
    if (index < 1 || index > recordCount) {
        cout << "Invalid entry.\n";
        return;
    }
    index--;

    string newDate, newPart, newNotes;
    cin.ignore();
    cout << "Enter new date (leave empty to keep current): ";
    getline(cin, newDate);
    cout << "Enter new part (leave empty to keep current): ";
    getline(cin, newPart);
    cout << "Enter new notes (leave empty to keep current): ";
    getline(cin, newNotes);

    if (!newDate.empty()) records[index].date = newDate;
    if (!newPart.empty()) records[index].part = newPart;
    if (!newNotes.empty()) records[index].notes = newNotes;

    saveToFile();
    cout << "Record updated.\n";
}

void MaintenanceManager::deleteRecord() {
    int index;
    cout << "Enter record number to delete (1 to " << recordCount << "): ";
    cin >> index;
    if (cin.fail()) {
        clearInput();
        cout << "Invalid input.\n";
        return;
    }
    if (index < 1 || index > recordCount) {
        cout << "Invalid entry.\n";
        return;
    }
    for (int i = index - 1; i < recordCount - 1; ++i) {
        records[i] = records[i + 1];
    }
    recordCount--;
    saveToFile();
    cout << "Record deleted.\n";
}

void MaintenanceManager::showAllRecords() {
    if (recordCount == 0) {
        cout << "No records found.\n";
        return;
    }
    for (int i = 0; i < recordCount; ++i) {
        cout << "\nRecord " << i + 1 << ":\n";
        cout << "Date: " << records[i].date << endl;
        cout << "Part: " << records[i].part << endl;
        cout << "Notes: " << records[i].notes << endl;
    }
}

void MaintenanceManager::showUpcomingMaintenance() {
    cout << "Upcoming maintenance is recommended every 3 months.\n";
    for (int i = 0; i < recordCount; ++i) {
        cout << records[i].date << " - " << records[i].part << "\n";
    }
}

void MaintenanceManager::showPartsToPurchase() {
    cout << "Parts/fluids mentioned in recent maintenance notes:\n";
    for (int i = 0; i < recordCount; ++i) {
        if (records[i].notes.find("replace") != string::npos || records[i].notes.find("buy") != string::npos)
            cout << "- " << records[i].part << ": " << records[i].notes << endl;
    }
}

void MaintenanceManager::displayMenu() {
    int choice;
    do {
        cout << "\n====== Car Maintenance Tracker ======\n";
        cout << "1. Add New Record\n";
        cout << "2. Search Records\n";
        cout << "3. Edit Record\n";
        cout << "4. Delete Record\n";
        cout << "5. Show All Records\n";
        cout << "6. Show Upcoming Maintenance\n";
        cout << "7. Show Parts/Fluids to Purchase\n";
        cout << "8. Exit\n";
        cout << "Choose an option (1-8): ";
        cin >> choice;

        if (cin.fail()) {
            clearInput();
            cout << "Invalid input.\n";
            continue;
        }

        switch (choice) {
        case 1: addRecord(); break;
        case 2: searchRecord(); break;
        case 3: editRecord(); break;
        case 4: deleteRecord(); break;
        case 5: showAllRecords(); break;
        case 6: showUpcomingMaintenance(); break;
        case 7: showPartsToPurchase(); break;
        case 8: cout << "Exiting...\n"; break;
        default: cout << "Invalid entry. Please choose a valid option.\n";
        }
    } while (choice != 8);
}

int main() {
    MaintenanceManager manager("maintenance_data.txt");
    manager.displayMenu();
    return 0;
}
