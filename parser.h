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
    const QString filename;
};


#endif // PARSER

