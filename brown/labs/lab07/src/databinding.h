/*!
   @file   Databinding.h
   @author Evan Wallace (evan.exe@gmail.com)
   @author Ben Herila (ben@herila.net)
   @date   Fall 2010
*/

#ifndef DATABINDING_H
#define DATABINDING_H

#include <QObject>
#include <QVariant>
#include <QSlider>
#include <QLineEdit>
#include <QCheckBox>
#include <QButtonGroup>
#include <QRadioButton>
#include <QDockWidget>
#include <QTabWidget>
#include <QDial>

/*!

@class DataBinding
@brief Binds a slider and a textbox to an int.

This class binds a slider and a textbox to an int, so when either the slider or the textbox
is changed, the other is also changed and the int is updated with the new value.  This does
not update either the slider or the textbox when the int is set to a new value.

**/
class DataBinding : public QObject {
    Q_OBJECT

public:
    DataBinding(QObject *parent = 0) : QObject(parent) {}
    virtual ~DataBinding() {}

signals:
    void dataChanged();

};

/*!

@class ChoiceBinding
@brief Binds a group of radio buttons to an int (enum) value.

This class binds a group of radio buttons to an int, so when a radio button is clicked the
int is updated with the index of the currently checked button in the group.  This does not
update the radio buttons when the int is set to a new value.

**/
class ChoiceBinding : public DataBinding {
    Q_OBJECT
public:
    virtual ~ChoiceBinding() {}

    static ChoiceBinding* bindRadioButtons(int numRadioButtons, int &value, ...);
    static ChoiceBinding* bindTabs(QTabWidget *tabs, int &value);

private slots:
    void intChanged(int newValue);

private:
    ChoiceBinding(int &value) : DataBinding(), m_value(value) {}

    int &m_value;
};

#endif // DATABINDING_H
