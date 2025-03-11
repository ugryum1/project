#ifndef EMPLOYEE_TABLE_H
#define EMPLOYEE_TABLE_H

#include <QMainWindow>
#include <QTableWidget>

class EmployeeTable : public QMainWindow {
    Q_OBJECT
public:
    explicit EmployeeTable(QWidget *parent = nullptr);
    ~EmployeeTable();

private slots:
    void addRow();
    void editRow(int row);

private:
    QTableWidget *table;
};

#endif // EMPLOYEE_TABLE_H
