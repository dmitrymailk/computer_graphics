#ifndef UNIFORMWIDGET_H
#define UNIFORMWIDGET_H

#include "glwidget.h"

using std::map;
using std::string;

class QBoxLayout; class QComboBox; class QLineEdit; class QPushButton; class QLabel;
class QString;

class UniformWidget : public QObject
{
    Q_OBJECT

public:
    UniformWidget(QOpenGLContext *ctx, GLWidget *glwidget, UniformVariable::Type uniformType = UniformVariable::TYPE_INT, const QString &uniformName = QString(""), bool editable = true, int size = 1);
    UniformWidget(UniformVariable *uniform, GLWidget *glwidget, bool editable = false);
    void construct(bool editable);
    virtual ~UniformWidget();

    void addTo(QBoxLayout *layout);
    void removeFrom(QBoxLayout *layout);

    const UniformVariable *getUniform() const;

    void updateUniform(const QString &text);

signals:
    void deleted(UniformWidget *deleted);
    void changed(const UniformVariable *uniform);

private slots:
    void valChanged(const QString &text);
    void updateVal();
    void valClicked();
    void deleteClicked();

public slots:
    void deleteUniform();

private:

    QBoxLayout *widget, *botRow;
    QLabel *label;
    QLineEdit *val;
    QString normalValStyle;
    QPushButton *button;

    static map<string, string> s_defaults;

    UniformVariable *uniform;

    bool eventFilter(QObject *, QEvent *);
    void createVecPicker();
    GLWidget *glwidget;

    bool ownUniform;
};

#endif // UNIFORMWIDGET_H
