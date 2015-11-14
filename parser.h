#ifndef PARSER
#define PARSER

#include <QString>
#include <QList>

#include "searchquery.h"

class Parser {
public:
    Parser(const QString & name);
    QList<SearchQuery> Parse();

private:
    void readFile();

private:
    const QString filename;
    QString content;
};


#endif // PARSER

