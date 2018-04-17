#ifndef PARSER
#define PARSER

#include <QString>
#include <QList>

#include "searchquery.h"

class Parser {
public:
    Parser(const QString & name);
    virtual QList<SearchQuery> Parse();

protected:
    void readFile();

protected:
    const QString filename;
    QString content;
};


#endif // PARSER

