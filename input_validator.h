#ifndef INPUT_VALIDATOR_H
#define INPUT_VALIDATOR_H

#include <QStyledItemDelegate> // Базовый класс для создания делегатов
#include <QRegularExpressionValidator> // Валидатор на основе регулярных выражений
#include <QLineEdit>  // Виджет для редактирования текста
#include <QObject>    // Базовый класс Qt
#include <QWidget>    // Базовый класс виджетов
#include <QString>    // Работа со строками

/*
Класс для валидации ввода в QTableWidget наследуется от QStyledItemDelegate и добавляет поддержку
валидации через QRegularExpressionValidator для редактируемых ячеек таблицы.
*/
class InputValidator : public QStyledItemDelegate {
    Q_OBJECT // Макрос для поддержки сигналов/слотов

public:
    explicit InputValidator(QValidator *validator, QObject *parent = nullptr);

    // Создание редактора для ячейки таблицы
    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;
private:
    QValidator *validator; // Указатель на валидатор, используемый этим делегатом
};

#endif // INPUT_VALIDATOR_H
