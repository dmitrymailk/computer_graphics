#include "databinding.h"
#include <QVariant>


////////////////////////////////////////////////////////////////////////////////
// class ChoiceBinding
////////////////////////////////////////////////////////////////////////////////
ChoiceBinding* ChoiceBinding::bindRadioButtons(int numRadioButtons, int &value, ...) {
    // Create a button group from the variable argument list following initialValue
    QButtonGroup *buttonGroup = new QButtonGroup;
    va_list args;
    va_start(args, value);
    for (int id = 0; id < numRadioButtons; id++)
        buttonGroup->addButton(va_arg(args, QRadioButton *), id);
    va_end(args);

    // Bind the button group and the value together
    ChoiceBinding *binding = new ChoiceBinding(value);
    connect(buttonGroup, SIGNAL(buttonClicked(int)), binding, SLOT(intChanged(int)));

    // Set the initial value
    value = qMax(0, qMin(numRadioButtons - 1, value));
    buttonGroup->button(value)->click();

    return binding;
}

ChoiceBinding* ChoiceBinding::bindTabs(QTabWidget *tabs, int &value) {
    // Bind the tabs and the value together
    ChoiceBinding *binding = new ChoiceBinding(value);
    connect(tabs, SIGNAL(currentChanged(int)), binding, SLOT(intChanged(int)));

    // Set the initial value
    value = qMax(0, qMin(tabs->count() - 1, value));
    tabs->setCurrentIndex(value);

    return binding;
}

void ChoiceBinding::intChanged(int newValue) {
    m_value = newValue;
    emit dataChanged();
}
