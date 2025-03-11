#include "employee_table.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>

EmployeeTable::EmployeeTable(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(960, 540);
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    table = new QTableWidget(0, 5, this);
    table->setHorizontalHeaderLabels({"Номер сотрудника", "ФИО", "Должность", "ЗП", "Дата рождения"});
    layout->addWidget(table);

    QPushButton *addButton = new QPushButton("Добавить", this);
    layout->addWidget(addButton);
    connect(addButton, &QPushButton::clicked, this, &EmployeeTable::addRow);

    setCentralWidget(centralWidget);
}

EmployeeTable::~EmployeeTable() {}

void EmployeeTable::addRow() {
    int row = table->rowCount();
    table->insertRow(row);
}

void EmployeeTable::editRow(int row) {
    // Реализация редактирования строки
}
