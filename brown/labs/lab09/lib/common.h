#ifndef COMMON_H
#define COMMON_H

#define qstrcmp(qstring1, qstring2) (!qstring1.compare(qstring2))
#define qstrcmpl(qstring1, qstring2) (!qstring1.toLower().compare(QString(qstring2).toLower()))

#define degreesToRadians(x) x*(3.141592f/180.0f)

#endif // COMMON_H
