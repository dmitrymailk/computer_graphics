#include "varsfile.h"

#include <QFileInfo>
#include <QDomDocument>

bool VarsFile::load(const QString &p, QList<const UniformVariable *> *dest, QOpenGLContext *ctx)
{
    if (!dest)
        return false;
    QString path = QFileInfo(p).absolutePath() + '/' + QFileInfo(p).baseName() + ".vars";

    if (!QFileInfo(path).exists())
        return false;

    QFile file(path);
    if (!file.open(QIODevice::ReadOnly))
        return false;

    QDomDocument doc;
    if (!doc.setContent(&file)) {
        file.close();
        return false;
    }
    file.close();

    QDomElement uniforms = doc.firstChildElement("uniforms");
    if (uniforms.isNull()) return false;

    QDomElement var = uniforms.firstChildElement("var");
    while (!var.isNull()) {
        QString name = var.attribute("name");
        QString type = var.attribute("type");
        QString value = var.text();
        QString perm = var.attribute("perm", "false");
        QString size = var.attribute("size", "1");

        if (!name.isEmpty() && !type.isEmpty()) {
            UniformVariable *v = new UniformVariable(ctx);
            v->setName(name);
            v->setType(UniformVariable::typeFromName(type));
            bool isInt;
            int sizei = size.toInt(&isInt);
            if (isInt) {
                v->setArraySize(sizei);
            }
            v->parse(value);
            if (qstrcmpl(perm, "true")) {
                v->setPermanent(true);
            } else {
                v->setPermanent(false);
            }
            dest->append(v);
        }

        var = var.nextSiblingElement("var");
    }
    return true;
}

bool VarsFile::save(const QString &p, const QList<const UniformVariable *> *dest)
{
    if (!dest)
        return false;

    QDomDocument doc;
    QDomElement uniforms = doc.createElement("uniforms");
    doc.appendChild(uniforms);

    foreach (const UniformVariable *v, *dest) {
        QDomElement var = doc.createElement("var");
        var.setAttribute("name", v->getName());
        var.setAttribute("type", UniformVariable::typeName(v->getType()));
        var.setAttribute("perm", v->getPermanent() ? "true" : "false");
        var.setAttribute("size", v->getArrySize());

        QDomText text = doc.createTextNode(v->toString());
        var.appendChild(text);
        uniforms.appendChild(var);
    }

    QString path = QFileInfo(p).absolutePath() + '/' + QFileInfo(p).baseName() + ".vars";

    QFile file(path);
    if (!file.open(QIODevice::WriteOnly))
        return false;

    file.write(doc.toString().toUtf8());

    file.close();
    return true;
}

VarsFile::VarsFile()
{
}
