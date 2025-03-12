#include <QApplication>
#include "employee_table.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // Создание приложения Qt

    EmployeeTable window;  // Создание главного окна
    window.show();  // Отображение окна

    return app.exec();  // Запуск главного цикла обработки событий
}
