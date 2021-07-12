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
#include <QColor>
#include <QDial>
#include <QPushButton>

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

@class FloatBinding
@brief Binds a slider and a textbox to a float.

This class binds a slider and a textbox to a float, so when either the slider or the textbox
is changed, the other is also changed and the float is updated with the new value.  This does
not update either the slider or the textbox when the float is set to a new value.

**/
class FloatBinding : public DataBinding {
    Q_OBJECT
public:
    virtual ~FloatBinding() {}

    static FloatBinding* bindSliderAndTextbox(
            QSlider *slider, QLineEdit *textbox, float &value, float minValue, float maxValue);

private slots:
    void intChanged(int newValue);
    void stringChanged(QString newValue);

signals:
    void updateInt(int newValue);
    void updateString(QString newValue);

private:
    FloatBinding(float &value) :
        DataBinding(), m_value(value), m_maxValue(0), m_minValue(0), m_offset(0),
        m_wrappingExtendsRange(false) {}

    float &m_value, m_maxValue, m_minValue, m_offset;
    bool m_wrappingExtendsRange;
};

/*!

@class ColorBinding
@brief Binds a button and three textboxes to a QColor.

**/
class ColorBinding : public DataBinding {
    Q_OBJECT
public:
    virtual ~ColorBinding() {}

    static ColorBinding* bindButtonAndTextboxes(
            QPushButton *button, QLineEdit *rTextbox, QLineEdit *gTextbox, QLineEdit *bTextbox,
            QColor &value);

private slots:
    void buttonPushed();
    void rStringChanged(QString newValue);
    void gStringChanged(QString newValue);
    void bStringChanged(QString newValue);

signals:
    void updateRString(QString newValue);
    void updateGString(QString newValue);
    void updateBString(QString newValue);

private:
    void updateButtonColor();

    ColorBinding(QColor &value, QPushButton *button) :
        DataBinding(), m_value(value), m_button(button) {}
    QColor &m_value;
    QPushButton *m_button;
};

#endif // DATABINDING_H
