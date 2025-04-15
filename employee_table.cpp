#include "employee_table.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QDialog>
#include <QLineEdit>
#include <QHeaderView>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QComboBox>
#include <QDate>
#include <algorithm>

EmployeeTable::EmployeeTable(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(960, 540);  // Фиксированный размер окна

    this->setStyleSheet(
        "QMainWindow { background-color:rgb(255, 240, 130); }"  // Цвет фона окна
        "QTableWidget::item { background-color: white; color: black; }"  // Цвет фона ячеек и текста
        "QHeaderView::section { background-color: #0078D7; color: black; font-weight: bold; }"  // Заголовки таблицы
        "QPushButton { background-color:rgb(0, 225, 255); color: black; border-radius: 5px; padding: 5px; }"  // Кнопки
        "QPushButton:hover { background-color: #0078D7; }"  // Цвет при наведении
        "QPushButton:pressed { background-color: #0078D7; }"  // Цвет при нажатии
    );

    // Создание главного виджета и компоновки
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout(centralWidget);

    // Панель сортировки
    QWidget *sortPanel = new QWidget(this);
    QHBoxLayout *sortLayout = new QHBoxLayout(sortPanel);

    QLabel *sortLabel = new QLabel("Сортировка:", this);
    QComboBox *sortCombo = new QComboBox(this);
    sortCombo->addItems({"Без сортировки", "По ФИО (А-Я)", "По ФИО (Я-А)", "По ЗП (возр.)", "По ЗП (убыв.)", "По дате рождения"});

    QPushButton *sortButton = new QPushButton("Применить", this);
    sortLayout->addWidget(sortLabel); // Текст "Сортировка"
    sortLayout->addWidget(sortCombo); // Выбадающий список (комбобокс)
    sortLayout->addWidget(sortButton); // Кнопка "Применить" (запуск сортироки)
    sortLayout->addStretch(); // Расположение элементов ближе к левому краю

    // Добавляем готовую панель сортировки в главное окно
    layout->addWidget(sortPanel);

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

    // Подключение сигналов
    connect(sortButton, &QPushButton::clicked, [this, sortCombo]() {
        applySorting(sortCombo->currentIndex());
    });

    setCentralWidget(centralWidget);

    loadDataFromFile("/home/ugryum/project/dataBase.txt");
}

// Деструктор класса EmployeeTable
EmployeeTable::~EmployeeTable() {}

// Добавление новой строки в таблицу
void EmployeeTable::addRow() {
    int row = table->rowCount();  // Получаем текущее количество строк
    table->insertRow(row);  // Добавляем новую строку

    // Создаём кнопку "Удалить"
    QPushButton *deleteButton = new QPushButton("Удалить");
    table->setCellWidget(row, 5, deleteButton);

    connect(deleteButton, &QPushButton::clicked, [this, deleteButton]() {
        int rowToDelete = table->indexAt(deleteButton->pos()).row();
        deleteRow(rowToDelete);
    });
}

void EmployeeTable::deleteRow(int row) {
    table->removeRow(row);
}

void EmployeeTable::customSort(int column, bool ascending) {
    QList<QList<QVariant>> rows; // Список строк, каждая строка — список значений ячеек (и индекс)

    // Извлекаем данные из таблицы в список
    for (int i = 0; i < table->rowCount(); ++i) {
        QList<QVariant> rowData;

        // Сохраняем значения из первых 5 колонок (ФИО, Телефон, Должность, ЗП, Дата рождения)
        for (int j = 0; j < 5; ++j) {
            rowData.append(table->item(i, j)->text());
        }

        rowData.append(QVariant::fromValue(i)); // Добавляем оригинальный индекс строки (если вдруг понадобится)
        rows.append(rowData); // Добавляем строку в общий список
    }

    // Сортируем список строк по выбранному столбцу
    std::sort(rows.begin(), rows.end(), [column, ascending](const QList<QVariant> &a, const QList<QVariant> &b) {
        // Если колонка — "Зарплата" (index 3)
        if (column == 3) {
            bool ok1, ok2;
            // Преобразуем зарплату в double (заменяя запятые на точки для корректности)
            double val1 = a[column].toString().replace(",", ".").toDouble(&ok1);
            double val2 = b[column].toString().replace(",", ".").toDouble(&ok2);

            // Сравниваем по возрастанию или убыванию
            return ascending ? val1 < val2 : val1 > val2;
        }
        // Если колонка — "Дата рождения" (index 4)
        else if (column == 4) {
            // Преобразуем строки в QDate
            QDate d1 = QDate::fromString(a[column].toString(), "dd.MM.yyyy");
            QDate d2 = QDate::fromString(b[column].toString(), "dd.MM.yyyy");

            // Сравниваем по дате
            return ascending ? d1 < d2 : d1 > d2;
        }
        else {
            // Сравнение строк в других колонках (например, ФИО, Телефон, Должность)
            return ascending ? a[column].toString() < b[column].toString()
                             : a[column].toString() > b[column].toString();
        }
    });

    // Очищаем таблицу перед повторным добавлением отсортированных данных
    table->setRowCount(0);

    // Добавляем отсортированные строки обратно в таблицу
    for (const auto &rowData : rows) {
        int row = table->rowCount(); // Получаем текущий индекс для новой строки
        table->insertRow(row);       // Вставляем строку

        // Вставляем текстовые данные в первые 5 колонок
        for (int j = 0; j < 5; ++j) {
            table->setItem(row, j, new QTableWidgetItem(rowData[j].toString()));
        }

        // Создаём кнопку "Удалить" для строки
        QPushButton *deleteButton = new QPushButton("Удалить");
        table->setCellWidget(row, 5, deleteButton); // Устанавливаем кнопку в 6-ю колонку

        // Подключаем сигнал нажатия на кнопку к функции удаления строки
        connect(deleteButton, &QPushButton::clicked, [this, deleteButton]() {
            int rowToDelete = table->indexAt(deleteButton->pos()).row();
            deleteRow(rowToDelete); // Удаляем строку по найденному индексу
        });
    }
}

void EmployeeTable::applySorting(int sortType)
{
    switch(sortType) {
        case 0: // Без сортировки — не делаем ничего
            break;
        case 1: // ФИО (А-Я)
            customSort(0, true);
            break;
        case 2: // ФИО (Я-А)
            customSort(0, false);
            break;
        case 3: // ЗП (возр.)
            customSort(3, true);
            break;
        case 4: // ЗП (убыв.)
            customSort(3, false);
            break;
        case 5: // Дата рождения
            customSort(4, true);
            break;
    }
}

void EmployeeTable::loadDataFromFile(const QString &filename) {
    QFile file(filename);

    // Открываем файл только для чтения в текстовом режиме
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл " + filename);
        return;
    }

    QTextStream in(&file);
    bool ok;
    // Читаем количество сотрудников из первой строки файла
    int employeeCount = in.readLine().toInt(&ok);

    // Проверка на корректность прочитанного числа сотрудников
    if (!ok || employeeCount <= 0) {
        QMessageBox::warning(this, "Ошибка", "Файл повреждён или содержит неверные данные.");
        file.close();
        return;
    }

    // Цикл для чтения информации о каждом сотруднике
    for (int i = 0; i < employeeCount; ++i) {
        // Считываем поля сотрудника из файла
        QString name = in.readLine();
        QString phone = in.readLine();
        QString post = in.readLine();
        QString salary = in.readLine();
        QString birthdate = in.readLine();

        // Проверка на наличие пустых строк в данных о сотруднике
        if (name.isEmpty() || phone.isEmpty() || post.isEmpty() || salary.isEmpty() || birthdate.isEmpty()) {
            QMessageBox::warning(this, "Ошибка", "Некорректные данные о сотруднике в файле.");
            file.close();
            return;
        }

        // Добавляем новую строку в таблицу
        int row = table->rowCount();
        table->insertRow(row);

        // Заполняем ячейки таблицы данными о сотруднике
        table->setItem(row, 0, new QTableWidgetItem(name));
        table->setItem(row, 1, new QTableWidgetItem(phone));
        table->setItem(row, 2, new QTableWidgetItem(post));
        table->setItem(row, 3, new QTableWidgetItem(salary));
        table->setItem(row, 4, new QTableWidgetItem(birthdate));

        // Создаём кнопку "Удалить" для удаления строки
        QPushButton *deleteButton = new QPushButton("Удалить");
        table->setCellWidget(row, 5, deleteButton);

        // Подключаем сигнал нажатия на кнопку к методу удаления строки
        connect(deleteButton, &QPushButton::clicked, [this, deleteButton]() {
            int rowToDelete = table->indexAt(deleteButton->pos()).row();
            deleteRow(rowToDelete); // Удаление строки по индексу
        });
    }

    // Закрываем файл после успешного чтения данных
    file.close();
}

void EmployeeTable::saveDataToFile(const QString &filename) {
    QFile file(filename);

    // Открываем файл для записи в текстовом режиме, предварительно очищая его содержимое
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл для записи: " + filename);
        return;
    }

    QTextStream out(&file);
    out << table->rowCount() << "\n"; // Записываем количество сотрудников в первую строку

    // Сохранение данных каждого сотрудника в файл
    for (int i = 0; i < table->rowCount(); ++i) {
        // Обходим все 5 полей сотрудника: ФИО, телефон, должность, зарплата, дата рождения
        for (int j = 0; j < 5; ++j) {
            // Проверка наличия данных в ячейке таблицы, если пусто — добавляем "-"
            QString value = table->item(i, j) ? table->item(i, j)->text() : "-";
            if (value.isEmpty()) {
                value = "-";
            }
            out << value << "\n"; // Записываем данные в файл
        }
    }

    // Закрываем файл после завершения записи данных
    file.close();
    qDebug() << "Данные успешно сохранены в файл: " << filename;
}

void EmployeeTable::closeEvent(QCloseEvent *event) {
    // Сохраняем данные в файл при закрытии окна
    saveDataToFile("/home/ugryum/project/dataBase.txt");

    // Подтверждаем завершение работы приложения
    event->accept();
}
