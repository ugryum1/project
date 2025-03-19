#include "employee.h"

Employee::Employee() {
    name = "";
    telephoneNumber = "";
    post = "";
    salary = "";
    birthdate = "";
}

Employee::~Employee() {
    name = "";
    telephoneNumber = "";
    post = "";
    salary = "";
    birthdate = "";
}

std::string Employee::getName() const {
    return name;
}

std::string Employee::getTelephoneNumber() const {
    return telephoneNumber;
}

std::string Employee::getPost() const {
    return post;
}

std::string Employee::getSalary() const {
    return salary;
}

std::string Employee::getBirthdate() const {
    return birthdate;
}

void Employee::setName(const std::string& newName) {
    name = newName;
}

void Employee::setTelephoneNumber(const std::string& newTelephoneNumber) {
    telephoneNumber = newTelephoneNumber;
}

void Employee::setPost(const std::string& newPost) {
    post = newPost;
}

void Employee::setSalary(const std::string& newSalary) {
    salary = newSalary;
}

void Employee::setBirthdate(const std::string& newBirthdate) {
    birthdate = newBirthdate;
}
