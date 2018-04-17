#ifndef TSV_PARSER_H
#define TSV_PARSER_H

#include <QString>
#include <QList>

#include "searchquery.h"
#include "parser.h"

class TSVParser : public Parser {
public:
    TSVParser(const QString & name);
    virtual QList<SearchQuery> Parse() override;
};

#endif // TSV_PARSER_H

