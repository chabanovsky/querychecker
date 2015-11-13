#include "parser.h"

Parser::Parser(const QString & name) : filename(name) {
}

QList<SearchQuery> Parser::Parse() {
    QList<SearchQuery> test;
    test.append(SearchQuery(QString("Билд Natron под Windows"), 1));
    test.append(SearchQuery(QString("Stack Overflow на русском"), 1));
    return test;
}


