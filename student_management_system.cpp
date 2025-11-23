#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

class Student {
private:
    int id;
    string name;
    int age;
    vector<string> subjects = {"Math", "English", "Science", "Computer", "Economics"};
    vector<float> marks;
    float average;

public:
    Student() : id(0), age(0), average(0.0) {
        marks.resize(subjects.size(), 0.0f);
    }

    // For creating from file data
    Student(int _id, const string& _name, int _age, const vector<float>& _marks)
        : id(_id), name(_name), age(_age), marks(_marks), average(0.0f) {
        if (!marks.empty()) {
            float sum = 0;
            for (float m : marks) sum += m;
            average = sum / marks.size();
        }
    }

    void inputStudentData(int studentID) {
        id = studentID;
        cout << "\nEnter student name: ";
        cin.ignore();
        getline(cin, name);
        cout << "Enter age: ";
        cin >> age;

        marks.resize(subjects.size());
        float sum = 0;

        cout << "\nEnter marks for the following subjects:\n";
        for (size_t i = 0; i < subjects.size(); ++i) {
            cout << subjects[i] << ": ";
            cin >> marks[i];
            sum += marks[i];
        }

        average = sum / subjects.size();
        cout << "\n? Student record added successfully!\n";
    }

    void displayStudentData() const {
        cout << "\n----------------------------------\n";
        cout << "ID: " << id << endl;
        cout << "Name: " << name << endl;
        cout << "Age: " << age << endl;

        cout << "Marks:\n";
        for (size_t i = 0; i < subjects.size(); ++i)
            cout << "  " << subjects[i] << ": " << marks[i] << endl;

        cout << "Average Score: " << average << endl;
    }
        void displayStudentRow() const {
        cout << left << setw(5) << id
             << setw(15) << name
             << setw(6) << age;

        for (float m : marks) {
            cout << setw(10) << m;
        }
        cout << setw(8) << average << endl;
    }


    void updateStudentData() {
        cout << "\nUpdating record for " << name << " (ID: " << id << ")\n";
        cout << "Enter new name (press enter to keep unchanged): ";
        cin.ignore();
        string newName;
        getline(cin, newName);
        if (!newName.empty()) name = newName;

        cout << "Enter new age (0 to keep unchanged): ";
        int newAge;
        cin >> newAge;
        if (newAge > 0) age = newAge;

        cout << "\nUpdate marks? (Y/N): ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y') {
            float sum = 0;
            for (size_t i = 0; i < subjects.size(); ++i) {
                cout << "New mark for " << subjects[i] << ": ";
                cin >> marks[i];
                sum += marks[i];
            }
            average = sum / subjects.size();
        }

        cout << "\n? Record updated successfully!\n";
    }

    int getID() const { return id; }

    // Expose serialized line for future saving (we'll use it in save step)
    string serializeToLine() const {
        // Format: ID,Name,Age,m1,m2,m3,m4,m5
        ostringstream oss;
        oss << id << "," << name << "," << age;
        for (float m : marks) {
            oss << "," << m;
        }
        return oss.str();
    }
};

class StudentManagementSystem {
private:
    vector<Student> students;
    int nextID;
    const string DATA_FILE = "students.txt";

public:
    StudentManagementSystem() : nextID(1) {
        loadDataFromFile(); // load existing records at startup
        // set nextID to max existing ID + 1 to avoid duplicates
        int maxID = 0;
        for (const auto& s : students) {
            // We don't have a getID method returning const here, so temporarily use reflection via serialize (quick parse)
        }
        // Determine nextID correctly:
        for (const auto& line : getAllSerializedLines()) {
            // Not used; this loop is just placeholder - we'll compute nextID below properly
            (void)line;
        }
        // compute nextID by checking highest ID present
        int highest = 0;
        for (const Student& s : students) {
            // unfortunately getID is available; use it
            if (s.getID() > highest) highest = s.getID();
        }
        nextID = highest + 1;
    }

    // Load students from students.txt
    void loadDataFromFile() {
        ifstream fin(DATA_FILE);
        if (!fin.is_open()) {
            // No file yet — not an error
            cout << "(Info) No data file found. Starting with empty records.\n";
            return;
        }

        string line;
        int loaded = 0;
        while (getline(fin, line)) {
            if (line.empty()) continue;

            // Parse line: ID,Name,Age,m1,m2,m3,m4,m5
            // We'll split by comma. Name must not contain commas for this simple format.
            vector<string> tokens;
            string token;
            stringstream ss(line);
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            // Need at least ID,Name,Age + marks(5) => total 8 tokens
            if (tokens.size() < 8) {
                cout << "(Warning) Skipping malformed line in data file: " << line << "\n";
                continue;
            }

            int id = stoi(tokens[0]);
            string name = tokens[1];
            int age = stoi(tokens[2]);

            vector<float> marks;
            for (size_t i = 3; i < 3 + 5 && i < tokens.size(); ++i) {
                marks.push_back(static_cast<float>(stof(tokens[i])));
            }
            // If fewer than 5 marks present, pad with 0
            while (marks.size() < 5) marks.push_back(0.0f);

            Student s(id, name, age, marks);
            students.push_back(s);
            ++loaded;
        }

        fin.close();
        cout << "(Info) Loaded " << loaded << " student(s) from " << DATA_FILE << ".\n";
    }

    // helper to get serialized lines if needed (not used now)
    vector<string> getAllSerializedLines() const {
        vector<string> lines;
        for (const auto& s : students) {
            lines.push_back(s.serializeToLine());
        }
        return lines;
    }

    void addStudent() {
        Student s;
        s.inputStudentData(nextID++);
        students.push_back(s);
    }

       void viewStudents() {
        if (students.empty()) {
            cout << "\nNo student records available.\n";
            return;
        }

        cout << "\n==================================================================================================================\n";
        cout << "ID   " << left << setw(15) << "Name"
             << setw(6) << "Age"
             << setw(8) << "Math"
             << setw(10) << "English"
             << setw(10) << "Science"
             << setw(10) << "Computer"
             << setw(12) << "Economics"
             << setw(8) << "Avg" << endl;
        cout << "==================================================================================================================\n";

        for (const auto& s : students) {
            s.displayStudentRow();
        }

        cout << "==================================================================================================================\n";
    }


    Student* findStudentByID(int id) {
        for (auto& s : students)
            if (s.getID() == id)
                return &s;
        return nullptr;
    }

    void updateStudent() {
        if (students.empty()) {
            cout << "\nNo records to update.\n";
            return;
        }

        int id;
        cout << "\nEnter student ID to update: ";
        cin >> id;

        Student* s = findStudentByID(id);
        if (s != nullptr)
            s->updateStudentData();
        else
            cout << "\n? Student not found!\n";
    }

    void deleteStudent() {
        if (students.empty()) {
            cout << "\nNo records to delete.\n";
            return;
        }

        int id;
        cout << "\nEnter student ID to delete: ";
        cin >> id;

        for (size_t i = 0; i < students.size(); ++i) {
            if (students[i].getID() == id) {
                cout << "\nAre you sure you want to delete this record? (Y/N): ";
                char confirm;
                cin >> confirm;
                if (confirm == 'Y' || confirm == 'y') {
                    students.erase(students.begin() + i);
                    cout << "\n? Record deleted successfully!\n";
                } else {
                    cout << "\nDeletion canceled.\n";
                }
                return;
            }
        }
        cout << "\n? Student not found!\n";
    }
    
    void saveDataToFile() {
        ofstream fout(DATA_FILE);
        if (!fout.is_open()) {
            cout << "? Error saving data to file!" << endl;
            return;
        }

        for (const auto& s : students) {
            fout << s.serializeToLine() << "\n";
        }

        fout.close();
        cout << "(Info) All records saved to " << DATA_FILE << ".\n";
    }
        void searchByID() {
        if (students.empty()) {
            cout << "\nNo records available to search.\n";
            return;
        }

        int id;
        cout << "\nEnter student ID to search: ";
        cin >> id;

        Student* s = findStudentByID(id);
        if (s != nullptr) {
            cout << "\n? Student Found:\n";
            s->displayStudentData();
        } else {
            cout << "\n? No student found with ID " << id << ".\n";
        }
    }



       void displayMenu() {
        int choice;
        do {
            cout << "\n===== Student Management System =====\n";
            cout << "1. Add Student Record\n";
            cout << "2. View All Records\n";
            cout << "3. Update Record\n";
            cout << "4. Delete Record\n";
            cout << "5. Search by ID\n";   // ?? new option
            cout << "6. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1: addStudent(); break;
                case 2: viewStudents(); break;
                case 3: updateStudent(); break;
                case 4: deleteStudent(); break;
                case 5: searchByID(); break;   // ?? handle new option
                case 6:
                    cout << "\nSaving and exiting program...\n";
                    saveDataToFile();
                    break;
                default:
                    cout << "\nInvalid choice! Please try again.\n";
            }

        } while (choice != 6);
    }
};

int main() {
    StudentManagementSystem sms;
    sms.displayMenu();
    return 0;
}

