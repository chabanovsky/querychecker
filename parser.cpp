#include "parser.h"

Parser::Parser(const QString & name) : filename(name) {
}

QList<SearchQuery> Parser::Parse() {
    QList<SearchQuery> searchQueries;
    readFile();

    QJsonDocument document = QJsonDocument::fromJson(content.toUtf8());
    QJsonObject obj =  document.object();
    QJsonValue val = obj.value("data");
    QJsonArray array = val.toArray();
    for (int index = 0; index < array.count(); ++index) {
        QJsonValue item = array.at(index);
        QJsonObject query = item.toObject();
        QJsonValue queryPhrase = query.value("phrase");
        searchQueries.append(SearchQuery(QString(queryPhrase.toString()), index));
    }

    return searchQueries;
}

void Parser::readFile() {
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly);
    content = file.readAll();
    file.close();
}


