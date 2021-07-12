#include "uniformwidget.h"

#include <QHBoxLayout>
#include <QLineEdit>
#include <QEvent>
#include <QFileDialog>
#include <QLabel>
#define checkParse(uniform, str) if (!uniform->parse(str)) uniform->parse("")

map<string, string> UniformWidget::s_defaults = {
    {"mvp", "<mvp>"},
    {"modelviewprojection", "<mvp>"},
    {"modelviewproj", "<mvp>"},
    {"model", "<model>"},
    {"m", "<model>"},
    {"mod", "<mode>"},
    {"view", "<view>"},
    {"v", "<view>"},
    {"projection", "<projection>"},
    {"proj", "<projection>"},
    {"p", "<projection>"},
    {"pro", "<projection>"},
    {"skybox", "<skybox>"},
    {"sky", "<skybox>"},
    {"envmap", "<skybox>"},
    {"env", "<skybox>"},
    {"time", "<time>"},
    {"t", "<time>"},
    {"size", "<size>"},
    {"mouse", "<mouse>"}
};

using std::string;

UniformWidget::UniformWidget(QOpenGLContext *ctx, GLWidget *glwidget, UniformVariable::Type uniformType, const QString &uniformName, bool editable, int size) : QObject()
{
    uniform = new UniformVariable(ctx);


    uniform->setType(uniformType);
    uniform->setName(uniformName);
    uniform->setArraySize(size);

    string lower = uniformName.toLower().toStdString();
    if (s_defaults.find(lower) != s_defaults.end()) {
        checkParse(uniform, QString::fromStdString(s_defaults[lower]));
    }

    this->glwidget = glwidget;
    ownUniform = true;
    construct(editable);
}

UniformWidget::UniformWidget(UniformVariable *uniform, GLWidget *glwidget, bool editable) : QObject()
{
    this->uniform = uniform;
    this->glwidget = glwidget;
    ownUniform = false;
    construct(editable);
}

void UniformWidget::construct(bool editable)
{
    widget = new QVBoxLayout();
    QString labelText = UniformVariable::typeName(uniform->getType()).append(" ").append(uniform->getName());

    label = new QLabel(labelText);
    label->setFixedHeight(20);
    widget->addWidget(label);

    botRow = new QHBoxLayout();

    val = new QLineEdit();
    val->setFixedWidth(220);
    if (editable)
        val->installEventFilter(this);
    val->setEnabled(editable);
    val->setText(uniform->toString());
    val->setFixedHeight(30);
    normalValStyle = val->styleSheet();
    botRow->addWidget(val);

    widget->addLayout(botRow);

    QObject::connect(val, SIGNAL(textChanged(QString)), this, SLOT(valChanged(QString)));

}

UniformWidget::~UniformWidget()
{
    delete label;
    delete val;
    delete botRow;
    delete widget;
}

void UniformWidget::addTo(QBoxLayout *layout)
{
    layout->addLayout(widget);
}

void UniformWidget::removeFrom(QBoxLayout *layout)
{
    layout->removeItem(widget);
}

const UniformVariable *UniformWidget::getUniform() const
{
    return uniform;
}

void UniformWidget::valChanged(const QString &text)
{
    if (!uniform->parse(text)) {
        val->setStyleSheet("border: 2px solid red");
    } else {
        val->setStyleSheet(normalValStyle);
        emit changed(uniform);
    }
}

void UniformWidget::updateVal()
{
    val->setText(uniform->toString());
}

void UniformWidget::valClicked()
{
    if (this->uniform->getType() >= UniformVariable::TYPE_TEXTURE_START && this->uniform->getType() <= UniformVariable::TYPE_TEX2D) {
        glwidget->setPaused(true);
        QString fileName = QFileDialog::getOpenFileName(widget->parentWidget(), tr("Open File"), "/course/cs123/data", tr("Images (*.png *.jpg *.jpeg *.tga *.bmp)"));
        glwidget->setPaused(false);
        if (fileName != NULL)
            val->setText(fileName);
    }
}

void UniformWidget::deleteClicked()
{
    emit deleted(this);

    delete this;
}

void UniformWidget::deleteUniform()
{
    emit deleted(this);

    delete this;
}

void UniformWidget::updateUniform(const QString &text)
{
    val->setText(text);
}

bool UniformWidget::eventFilter(QObject *object, QEvent *event)
{
    if (object == val && event->type() == QEvent::MouseButtonRelease) {
        valClicked();
        return false;
    }
    return false;
}

void UniformWidget::createVecPicker()
{

}
