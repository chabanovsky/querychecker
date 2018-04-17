#ifndef SEARCHQUERY_H
#define SEARCHQUERY_H

#include <QtWidgets>

class SearchQuery {
public:
    SearchQuery();
    SearchQuery(QString initialQuery, int initialGoogleRank);

    int GoogleRank();
    int YandexRank();
    QString Query();
    QString EncodingQuery();
    void SetYandexRank(int rank);
    QString ExtraStr();
    void SetExtraStr(QString);

public:
    static void SetMaxPageNum(int maxPageNum);
    static int MaxPageNum();

private:
    QString query;
    QString extraStr;
    int extraInt;
    int googleRank;
    int yandexRank;

private:
    static int MAX_PAGE_NUM;
};

#endif // SEARCHQUERY_H

