#include "employee_table.h"  // Собственный заголовочный файл класса
#include "input_validator.h" // Собственный класс для валидации ввода

// Основные модули Qt
#include <QVBoxLayout>      // Вертикальное размещение виджетов
#include <QPushButton>      // Кнопки интерфейса
#include <QLineEdit>        // Текстовые поля ввода
#include <QHeaderView>      // Заголовки таблицы
#include <QFile>            // Работа с файлами
#include <QTextStream>      // Текстовый поток ввода/вывода
#include <QMessageBox>      // Диалоговые окна сообщений
#include <QLabel>           // Текстовые метки
#include <QComboBox>        // Выпадающие списки
#include <QDate>            // Работа с датами

// Дополнительные модули
#include <QDebug>          // Для отладочного вывода
#include <QCloseEvent>     // Обработка события закрытия окна
#include <algorithm>       // Стандартные алгоритмы (sort) - необходимо для сортировки

EmployeeTable::EmployeeTable(QWidget *parent) : QMainWindow(parent) {
    setFixedSize(960, 540); // Фиксированный размер окна

    this->setStyleSheet(
        "QMainWindow { background-color: #f5f7fa; }" // Светлый серо-голубой фон окна
        "QTableWidget { background-color: white; gridline-color: #e0e0e0; }" // Белый фон таблицы с серыми линиями
        "QTableWidget::item { background-color: white; color: #333333; padding: 5px; }" // Темно-серый текст в ячейках
        "QTableWidget::item:selected { background-color: #4a90e2; color: white; }" // Синий фон для выделенных ячеек
        "QHeaderView::section { background-color: #4a90e2; color: white; font-weight: bold; "
        "padding: 5px; border: none; }" // Синие заголовки с белым текстом
        "QPushButton { background-color: #4a90e2; color: white; border: none; "
        "border-radius: 4px; padding: 6px 12px; font-weight: 500; }" // Синие кнопки с белым текстом
        "QPushButton:hover { background-color: #357abd; }" // Темно-синий при наведении
        "QPushButton:pressed { background-color: #2a5f8f; }" // Еще темнее при нажатии
        "QComboBox { padding: 5px; border: 1px solid #d0d0d0; border-radius: 4px; }" // Стиль для комбобокса
        "QLabel { color: #333333; }" // Темно-серый текст для меток
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
    table->setColumnWidth(4, 100);
    table->setColumnWidth(5, 100);  // Кнопка "Удалить"

    // Настройка валидаторов ввода
    QRegularExpressionValidator *phoneValidator = createPhoneValidator();
    QRegularExpressionValidator *salaryValidator = createSalaryValidator();
    QRegularExpressionValidator *dateValidator = createDateValidator();
    table->setItemDelegateForColumn(1, new InputValidator(phoneValidator, this));  // Телефон
    table->setItemDelegateForColumn(3, new InputValidator(salaryValidator, this)); // Зарплата
    table->setItemDelegateForColumn(4, new InputValidator(dateValidator, this));   // Дата

    layout->addWidget(table);

    // Кнопка "Добавить" для создания новых строк
    QPushButton *addButton = new QPushButton("Добавить", this);
    layout->addWidget(addButton);

    // Подключение сигналов
    connect(addButton, &QPushButton::clicked, this, &EmployeeTable::addRow);
    connect(sortButton, &QPushButton::clicked, [this, sortCombo]() {
        applySorting(sortCombo->currentIndex());
    });

    setCentralWidget(centralWidget);

    loadDataFromFile("/home/ugryum/project/dataBase.txt");
}

// Методы валидации ввода
QRegularExpressionValidator* EmployeeTable::createPhoneValidator() {
    QRegularExpression phoneRegex("^[+0-9\\-\\(\\)\\s]{6,20}$");
    return new QRegularExpressionValidator(phoneRegex, this);
}

QRegularExpressionValidator* EmployeeTable::createSalaryValidator() {
    QRegularExpression salaryRegex("^\\d{1,8}([.,]\\d{1,2})?$");
    return new QRegularExpressionValidator(salaryRegex, this);
}

QRegularExpressionValidator* EmployeeTable::createDateValidator() {
    QRegularExpression dateRegex("^(0[1-9]|[12][0-9]|3[01])\\.(0[1-9]|1[012])\\.(19\\d{2}|20(0[0-9]|10|11))$");
    return new QRegularExpressionValidator(dateRegex, this);
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
    QList<QList<QVariant>> emptyRows; // Пустые строки

    // Извлекаем данные из таблицы в список
    for (int i = 0; i < table->rowCount(); ++i) {
        QList<QVariant> rowData;
        bool isRowFilled = true;

        // Сохраняем значения из первых 5 колонок (ФИО, Телефон, Должность, ЗП, Дата рождения)
        // Только заполненные строки
        for (int j = 0; j < 5; ++j) {
            QTableWidgetItem* item = table->item(i, j);
            if (!item || item->text().isEmpty()) {
                isRowFilled = false;
                break;
            }
            rowData.append(table->item(i, j)->text());
        }

        if (isRowFilled) {
            rowData.append(i); // Сохраняем оригинальный индекс
            rows.append(rowData);
        } else {
            // Для пустых строк сохраняем все имеющиеся данные
            QList<QVariant> emptyRowData;
            for (int j = 0; j < 5; ++j) {
                QTableWidgetItem* item = table->item(i, j);
                emptyRowData.append(item ? item->text() : "");
            }
            emptyRows.append(emptyRowData);
        }
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

    // Добавляем отсортированные заполненные строки
    for (const auto &rowData : rows) {
        addRowToTable(rowData.mid(0, 5)); // Берем только данные (без индекса)
    }

    // Добавляем невалидные строки в конец
    for (const auto &rowData : emptyRows) {
        addRowToTable(rowData);
    }
}

void EmployeeTable::addRowToTable(const QList<QVariant> &rowData) {
    int row = table->rowCount();
    table->insertRow(row);

    // Заполняем данные
    for (int j = 0; j < 5 && j < rowData.size(); ++j) {
        if (!rowData[j].toString().isEmpty()) {
            table->setItem(row, j, new QTableWidgetItem(rowData[j].toString()));
        }
    }

    // Добавляем кнопку удаления
    QPushButton *deleteButton = new QPushButton("Удалить");
    table->setCellWidget(row, 5, deleteButton);
    connect(deleteButton, &QPushButton::clicked, [this, deleteButton]() {
        int rowToDelete = table->indexAt(deleteButton->pos()).row();
        if (rowToDelete >= 0) {
            deleteRow(rowToDelete);
        }
    });
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
