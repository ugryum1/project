#include "input_validator.h"

InputValidator::InputValidator(QValidator *validator, QObject *parent)
    : QStyledItemDelegate(parent), validator(validator) {}

QWidget* InputValidator::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                                      const QModelIndex &index) const {
    QLineEdit *editor = new QLineEdit(parent);
    editor->setValidator(validator);
    return editor;
}
