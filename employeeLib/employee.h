#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <iostream>
#include <string>

class Employee {
private:
	std::string name; // ФИО
    std::string telephoneNumber; // номер телефона
    std::string post; // должность
    std::string salary; // заработная плата
    std::string birthdate; // дата рождения
public:
	Employee();
	~Employee();

	std::string getName() const;
	std::string getTelephoneNumber() const;
	std::string getPost() const;
	std::string getSalary() const;
    std::string getBirthdate() const;

    void setName(const std::string& newName);
	void setTelephoneNumber(const std::string& newTelephoneNumber);
	void setPost(const std::string& newPost);
	void setSalary(const std::string& newSalary);
    void setBirthdate(const std::string& newBirthdate);
};

#endif //EMPLOYEE_H
