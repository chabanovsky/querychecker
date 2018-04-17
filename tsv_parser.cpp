#include "tsv_parser.h"

TSVParser::TSVParser(const QString & name) : Parser(name) {
}

QList<SearchQuery> TSVParser::Parse() {
    QList<SearchQuery> searchQueries;

    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject obj =  document.object();
    QJsonValue val = obj.value("items");
    QJsonArray array = val.toArray();
    for (int index = 0; index < array.count(); ++index) {
        QJsonValue item = array.at(index);
        QJsonObject query = item.toObject();
        QJsonValue queryPhrase = query.value("term");
        searchQueries.append(SearchQuery(QString(queryPhrase.toString()), index));
    }

    return searchQueries;
}
