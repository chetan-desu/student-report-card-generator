#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

class Student {
private:
    int rollNo;
    string name;
    float marks1, marks2, marks3;
    float total, percentage;
    char grade;

    void calculate() {
        total = marks1 + marks2 + marks3;
        // percentage on a 0-100 scale
        percentage = (total / 300.0f) * 100.0f;
        if (percentage >= 90.0f) grade = 'A';
        else if (percentage >= 75.0f) grade = 'B';
        else if (percentage >= 60.0f) grade = 'C';
        else if (percentage >= 40.0f) grade = 'D';
        else grade = 'F';
    }

public:
    Student() : rollNo(0), marks1(0), marks2(0), marks3(0), total(0), percentage(0), grade('F') {}

    void getDataFromUser() {
        cout << "\nEnter Roll Number: ";
        cin >> rollNo;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter Name: ";
        getline(cin, name);
        cout << "Enter Marks in 3 Subjects:\n";
        cout << "Subject 1: "; cin >> marks1;
        cout << "Subject 2: "; cin >> marks2;
        cout << "Subject 3: "; cin >> marks3;
        calculate();
    }

    string toRecordLine() const {
        ostringstream oss;
        oss << rollNo << '|' << name << '|'
            << fixed << setprecision(2)
            << marks1 << '|' << marks2 << '|' << marks3 << '|'
            << total << '|' << percentage << '|' << grade;
        return oss.str();
    }

    bool fromRecordLine(const string &line) {
        string token;
        istringstream iss(line);
        if (!getline(iss, token, '|')) return false;
        try { rollNo = stoi(token); } catch (...) { return false; }

        if (!getline(iss, name, '|')) return false;
        if (!getline(iss, token, '|')) return false; marks1 = stof(token);
        if (!getline(iss, token, '|')) return false; marks2 = stof(token);
        if (!getline(iss, token, '|')) return false; marks3 = stof(token);

        // Recompute correct values from marks (ignore stored total/percentage/grade)
        calculate();
        return true;
    }

    void showDataRow() const {
        cout << left << setw(8) << rollNo
             << setw(20) << (name.size() > 18 ? name.substr(0, 17) + "…" : name)
             << setw(8) << fixed << setprecision(2) << marks1
             << setw(8) << fixed << setprecision(2) << marks2
             << setw(8) << fixed << setprecision(2) << marks3
             << setw(10) << fixed << setprecision(2) << total
             << setw(8) << fixed << setprecision(2) << percentage
             << setw(4) << grade << '\n';
    }

    int getRollNo() const { return rollNo; }
};

const string FILENAME = "students.txt";

void addStudent() {
    Student s;
    s.getDataFromUser();
    ofstream ofs(FILENAME, ios::app);
    if (!ofs) {
        cerr << "Error: cannot open file for writing.\n";
        return;
    }
    ofs << s.toRecordLine() << '\n';
    ofs.close();
    cout << "\nRecord Added Successfully!\n";
}

void displayAll() {
    ifstream ifs(FILENAME);
    if (!ifs) {
        cout << "\nNo data found! (file " << FILENAME << " missing)\n";
        return;
    }

    cout << "\n--------------------------------------------------------------------------------\n";
    cout << left << setw(8) << "Roll"
         << setw(20) << "Name"
         << setw(8) << "Sub1"
         << setw(8) << "Sub2"
         << setw(8) << "Sub3"
         << setw(10) << "Total"
         << setw(8) << "Percent"
         << setw(4) << "Gr" << '\n';
    cout << "--------------------------------------------------------------------------------\n";

    string line;
    Student s;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        if (s.fromRecordLine(line)) {
            s.showDataRow();
        } else {
            cerr << "Warning: skipped malformed record.\n";
        }
    }
    ifs.close();
}

void searchStudent(int rollNo) {
    ifstream ifs(FILENAME);
    if (!ifs) {
        cout << "\nNo data found! (file " << FILENAME << " missing)\n";
        return;
    }

    string line;
    Student s;
    bool found = false;
    while (getline(ifs, line)) {
        if (line.empty()) continue;
        if (s.fromRecordLine(line)) {
            if (s.getRollNo() == rollNo) {
                cout << "\nRecord Found:\n";
                cout << left << setw(8) << "Roll"
                     << setw(20) << "Name"
                     << setw(8) << "Sub1"
                     << setw(8) << "Sub2"
                     << setw(8) << "Sub3"
                     << setw(10) << "Total"
                     << setw(8) << "Percent"
                     << setw(4) << "Gr" << '\n';
                cout << "--------------------------------------------------------------------------------\n";
                s.showDataRow();
                found = true;
                break;
            }
        }
    }
    ifs.close();
    if (!found) cout << "\nRecord Not Found!\n";
}

int main() {
    int choice;
    do {
        cout << "\n===== STUDENT REPORT CARD SYSTEM =====";
        cout << "\n1. Add Student Record";
        cout << "\n2. Display All Records";
        cout << "\n3. Search Student by Roll No";
        cout << "\n4. Exit";
        cout << "\nEnter your choice: ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: displayAll(); break;
            case 3: {
                int r;
                cout << "Enter Roll Number to Search: ";
                cin >> r;
                searchStudent(r);
                break;
            }
            case 4:
                cout << "\nExiting Program... Goodbye!\n";
                break;
            default:
                cout << "\nInvalid Choice. Try again.\n";
        }
    } while (choice != 4);

    return 0;
}
