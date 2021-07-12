#ifndef VARSFILE_H
#define VARSFILE_H

#include "uniformvariable.h"

#include <QList>
#include <QString>

class VarsFile
{
public:
    static bool load(const QString& path, QList<const UniformVariable*> *dest, QOpenGLContext *ctx);
    static bool save(const QString& path, const QList<const UniformVariable *> *dest);
private:
    VarsFile();
};

#endif // VARSFILE_H
