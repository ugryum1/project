#include "employee_table.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>

EmployeeTable::EmployeeTable(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(960, 540);  // Фиксированный размер окна

    this->setStyleSheet(
        "QMainWindow { background-color: #f0f0f0; }"  // Цвет фона окна
        "QTableWidget::item { background-color: white; color: black; }"  // Цвет фона ячеек и текста
        "QHeaderView::section { background-color: #0078D7; color: black; font-weight: bold; }"  // Заголовки таблицы
        "QPushButton { background-color:rgb(0, 225, 255); color: black; border-radius: 5px; padding: 5px; }"  // Кнопки
        "QPushButton:hover { background-color: #0078D7; }"  // Цвет при наведении
        "QPushButton:pressed { background-color: #0078D7; }"  // Цвет при нажатии
    );

    // Создание главного виджета и компоновки
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Создание таблицы с 6 колонками
    table = new QTableWidget(0, 6, this);
    table->setHorizontalHeaderLabels({"ФИО", "Номер телефона", "Должность", "ЗП", "Дата рождения", "Удалить"});

    // Устанавливаем ширину колонок
    table->setColumnWidth(0, 235);
    table->setColumnWidth(1, 150);
    table->setColumnWidth(2, 200);
    table->setColumnWidth(3, 120);
    table->setColumnWidth(4, 120);
    table->setColumnWidth(5, 100);  // Кнопка "Удалить"

    layout->addWidget(table);

    // Кнопка "Добавить" для создания новых строк
    QPushButton *addButton = new QPushButton("Добавить", this);
    layout->addWidget(addButton);
    connect(addButton, &QPushButton::clicked, this, &EmployeeTable::addRow);

    setCentralWidget(centralWidget);
}

// Деструктор класса EmployeeTable
EmployeeTable::~EmployeeTable() {}

// Добавление новой строки в таблицу
void EmployeeTable::addRow() {
    int row = table->rowCount();  // Получаем текущее количество строк
    table->insertRow(row);  // Добавляем новую строку

    // Создаём кнопку "Удалить"
    QPushButton *deleteButton = new QPushButton("Удалить");
    connect(deleteButton, &QPushButton::clicked, [this, row]() { deleteRow(row); });
    table->setCellWidget(row, 5, deleteButton);
}

void EmployeeTable::deleteRow(int row) {
    table->removeRow(row);
}
