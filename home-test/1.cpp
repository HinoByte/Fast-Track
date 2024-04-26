#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <algorithm>

struct Student {
    int id;
    std::string name;
    int age;
};

class StudentManagement {
private:
    std::vector<std::shared_ptr<Student>> students;

public:
    void addStudent(int id, const std::string& name, int age) {
        auto newStudent = std::make_shared<Student>();
        newStudent->id = id;
        newStudent->name = name;
        newStudent->age = age;
        students.push_back(newStudent);
        std::cout << "Student added successfully!" << std::endl;
    }

    void removeStudent(int id) {
        auto it = std::find_if(students.begin(), students.end(), [id](const std::shared_ptr<Student>& s) {
            return s->id == id;
        });

        if (it != students.end()) {
            students.erase(it);
            std::cout << "Student removed successfully!" << std::endl;
        } else {
            std::cout << "Student with ID " << id << " not found." << std::endl;
        }
    }

    void getStudentInfo(int id) {
        auto it = std::find_if(students.begin(), students.end(), [id](const std::shared_ptr<Student>& s) {
            return s->id == id;
        });

        if (it != students.end()) {
            std::cout << "Student ID: " << (*it)->id << std::endl;
            std::cout << "Student Name: " << (*it)->name << std::endl;
            std::cout << "Student Age: " << (*it)->age << std::endl;
        } else {
            std::cout << "Student with ID " << id << " not found." << std::endl;
        }
    }
};

int main() {
    StudentManagement sms;

    sms.addStudent(1, "Vladislav", 20);
    sms.addStudent(2, "Vladimir", 22);
    sms.addStudent(3, "Gennady", 21);
    sms.getStudentInfo(2);
    sms.removeStudent(1);
    return 0;
}