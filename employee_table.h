#ifndef EMPLOYEE_TABLE_H
#define EMPLOYEE_TABLE_H

#include <QMainWindow>
#include <QTableWidget>
#include <vector>
#include <string>

#include "employeeLib/employee.h"

// Класс EmployeeTable представляет главное окно приложения с таблицей сотрудников
class EmployeeTable : public QMainWindow {
    Q_OBJECT  // Макрос Qt для работы с сигналами и слотами

public:
    // Конструктор класса EmployeeTable.
    // parent - родительский виджет (по умолчанию nullptr)
    explicit EmployeeTable(QWidget *parent = nullptr);

    // Деструктор класса EmployeeTable
    ~EmployeeTable();

private slots:
    // Добавление новой строки в таблицу
    void addRow();

    // Удаление строки
    void deleteRow(int row);

private:
    QTableWidget *table;  // Таблица для отображения данных сотрудников
    std::vector <Employee> dataBase; // Список с данными о всех сотрудниках

    // Методы для загрузки и сохранения данных
    void loadDataFromFile(const QString &filename);
    void saveDataToFile(const QString &filename);


protected:
    // Переопределённое событие закрытия окна
    void closeEvent(QCloseEvent *event) override;
};

#endif // EMPLOYEE_TABLE_H
