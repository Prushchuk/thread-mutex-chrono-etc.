#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
using namespace std;

class Student
{
private:
    string name;
    string group;
    int age;
public:
    Student(const string& name, const string& group, const int& age) : name{name}, group{group}, age{age} {}

    string getName() const {
        return name;
    }

    string getGroup() const {
        return group;
    }

    int getAge() const {
        return age;
    }
};

class Journal
{
private:
    vector<Student> students;
    mutex mtx;
public:
    void addStudent(const Student& student)
    {
        lock_guard<mutex> lock(mtx);
        students.push_back(student);
    }

    void writeToFile(const string& filename)
    {
        lock_guard<mutex> lock(mtx);

        ofstream file(filename);
        if (file.is_open()) {
            for (size_t i = 0; i < students.size(); i++) {
                const Student& student = students[i];
                file << "Name: " << student.getName() << ", Group: " << student.getGroup() << ", Age: " << student.getAge() << "." << endl;

            }
            file.close();
            cout << "Data had been written to " << filename << endl;
        }
        else {
            cerr << "Unable  to open file " << filename << endl;
        }
    }
};

int main()
{
    Journal journal1, journal2;

    journal1.addStudent(Student("Max", "BD321", 17));
    journal1.addStudent(Student("Nastya", "BV123", 15));
    journal2.addStudent(Student("Bob", "AB135", 22));
    journal2.addStudent(Student("Eve", "CD531", 19));

    thread th1(&Journal::writeToFile, &journal1, "journal1.txt");
    thread th2(&Journal::writeToFile, &journal1, "journal2.txt");

    th1.join();
    th2.join();
}