#ifndef EMPLOYEE_TABLE_H
#define EMPLOYEE_TABLE_H

#include <QMainWindow>
#include <QTableWidget>

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
};

#endif // EMPLOYEE_TABLE_H
