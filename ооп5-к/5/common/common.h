#ifndef COMMON_H
#define COMMON_H
#include <QString>
enum messages
{
    PRINT_CLASSIC_REQUEST = 1,
    PRINT_CLASSIC_ANSWER = 2,
    VALUE_REQUEST = 3,
    VALUE_ANSWER = 4,
};
extern const QChar separator;
QString& operator<< (QString&, const QString&);
#endif // COMMON_H