// #include <QApplication>
// #include <QPushButton>

// int main(int argc, char *argv[]) {
//     QApplication app(argc, argv);

//     QPushButton button("Hello, Qt!");
//     button.resize(200, 100);
//     button.show();

//     return app.exec();
// }

#include <QApplication>
#include "employee_table.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    EmployeeTable window;
    window.show();
    return app.exec();
}
