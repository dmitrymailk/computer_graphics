#include "databinding.h"
#include <QVariant>
#include <QColorDialog>
#include <math.h>

////////////////////////////////////////////////////////////////////////////////
// class FloatBinding
////////////////////////////////////////////////////////////////////////////////

// Sliders can only work in ints, so use 100 slider units for every value unit (two decimals of resolution)
inline float mapValue(int i) { return (float)i * 0.01f; }
inline int mapValue(float f) { return (int)ceilf(f * 100 - 0.5f); }

FloatBinding* FloatBinding::bindSliderAndTextbox(QSlider *slider, QLineEdit *textbox, float &value, float minValue, float maxValue) {
    // Bind the slider, the textbox, and the value together
    FloatBinding *binding = new FloatBinding(value);
    connect(slider, SIGNAL(sliderMoved(int)), binding, SLOT(intChanged(int)));
    connect(textbox, SIGNAL(textChanged(QString)), binding, SLOT(stringChanged(QString)));
    connect(binding, SIGNAL(updateInt(int)), slider, SLOT(setValue(int)));
    connect(binding, SIGNAL(updateString(QString)), textbox, SLOT(setText(QString)));

    // Set the range and initial value
    slider->setMinimum(mapValue(minValue));
    slider->setMaximum(mapValue(maxValue));
    slider->setValue(mapValue(value));
    textbox->setText(QString::number(value));

    return binding;
}

void FloatBinding::intChanged(int newValue) {
    float floatValue = mapValue(newValue);

    if (m_wrappingExtendsRange) {
        float lowerBound = m_minValue * 0.75f + m_maxValue * 0.25f;
        float upperBound = m_minValue * 0.25f + m_maxValue * 0.75f;

        // This is a hack to get dials to wrap around infinitely
        //
        //        A           B           C           D
        //  |-----------|-----------|-----------|-----------|
        // min     lowerBound              upperBound      max
        //
        // Any jump from A -> D or D -> A is considered a wrap around

        if (m_value - m_offset < lowerBound && floatValue > upperBound)
            m_offset -= m_maxValue - m_minValue;
        else if (m_value - m_offset > upperBound && floatValue < lowerBound)
            m_offset += m_maxValue - m_minValue;
    }

    if (m_value - m_offset != floatValue) {
        m_value = floatValue + m_offset;
        emit updateString(QString::number(m_value - m_offset));
        emit dataChanged();
    }
}

void FloatBinding::stringChanged(QString newValue) {
    float floatValue = newValue.toFloat();
    if (m_value - m_offset != floatValue) {
        m_value = floatValue + m_offset;
        emit updateInt(mapValue(m_value - m_offset));
        emit dataChanged();
    }
}


////////////////////////////////////////////////////////////////////////////////
// class ColorBinding
////////////////////////////////////////////////////////////////////////////////

ColorBinding* ColorBinding::bindButtonAndTextboxes(
        QPushButton *button, QLineEdit *rTextbox, QLineEdit *gTextbox, QLineEdit *bTextbox,
        QColor &value) {
    // Bind the slider, the textbox, and the value together
    ColorBinding *binding = new ColorBinding(value, button);
    connect(button, SIGNAL(pressed()), binding, SLOT(buttonPushed()));
    connect(rTextbox, SIGNAL(textChanged(QString)), binding, SLOT(rStringChanged(QString)));
    connect(gTextbox, SIGNAL(textChanged(QString)), binding, SLOT(gStringChanged(QString)));
    connect(bTextbox, SIGNAL(textChanged(QString)), binding, SLOT(bStringChanged(QString)));
    connect(binding, SIGNAL(updateRString(QString)), rTextbox, SLOT(setText(QString)));
    connect(binding, SIGNAL(updateGString(QString)), gTextbox, SLOT(setText(QString)));
    connect(binding, SIGNAL(updateBString(QString)), bTextbox, SLOT(setText(QString)));
    rTextbox->setText(QString::number(value.redF(), 'f', 2));
    gTextbox->setText(QString::number(value.greenF(), 'f', 2));
    bTextbox->setText(QString::number(value.blueF(), 'f', 2));

    return binding;
}

void ColorBinding::rStringChanged(QString newValue) {
    float floatValue = std::max(0.f, std::min(1.f, newValue.toFloat()));
    if (m_value.redF() != floatValue) {
        m_value.setRedF(floatValue);
        updateButtonColor();
        emit dataChanged();
    }
}

void ColorBinding::gStringChanged(QString newValue) {
    float floatValue = std::max(0.f, std::min(1.f, newValue.toFloat()));
    if (m_value.greenF() != floatValue) {
        m_value.setGreenF(floatValue);
        updateButtonColor();
        emit dataChanged();
    }
}

void ColorBinding::bStringChanged(QString newValue) {
    float floatValue = std::max(0.f, std::min(1.f, newValue.toFloat()));
    if (m_value.blueF() != floatValue) {
        m_value.setBlueF(floatValue);
        updateButtonColor();
        emit dataChanged();
    }
}

void ColorBinding::buttonPushed() {
    QColor color = QColorDialog::getColor(m_value);
    if (color.isValid()) {
        updateButtonColor();
        emit updateRString(QString::number(color.redF(), 'f', 2));
        emit updateGString(QString::number(color.greenF(), 'f', 2));
        emit updateBString(QString::number(color.blueF(), 'f', 2));
        emit dataChanged();
    }
}

void ColorBinding::updateButtonColor() {
    QString styleSheet("background: #"
          + QString(m_value.red() < 16? "0" : "") + QString::number(m_value.red(),16)
          + QString(m_value.green() < 16? "0" : "") + QString::number(m_value.green(),16)
          + QString(m_value.blue() < 16? "0" : "") + QString::number(m_value.blue(),16) + ";");
    QRect r = m_button->geometry();
    m_button->setStyleSheet(styleSheet);
    m_button->setGeometry(r);
}
