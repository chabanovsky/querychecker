#include "searchquery.h"
#include "utils.h"

int SearchQuery::MAX_PAGE_NUM = DEFAULT_MAX_PAGE_NUM;

SearchQuery::SearchQuery()
    : query(""),
    googleRank(-1),
    yandexRank(-1) {
}

SearchQuery::SearchQuery(QString initialQuery, int initialGoogleRank)
    : query(initialQuery),
    googleRank(initialGoogleRank),
    yandexRank(-1) {
}

int SearchQuery::GoogleRank() {
    return googleRank;
}

int SearchQuery::YandexRank() {
    return yandexRank;
}

QString SearchQuery::Query() {
    return query;
}

QString SearchQuery::EncodingQuery() {
    return QUrl::toPercentEncoding(Query());
}

void SearchQuery::SetYandexRank(int rank) {
    yandexRank = rank;
}

void SearchQuery::SetMaxPageNum(int maxPageNum) {
    MAX_PAGE_NUM = maxPageNum;
}

int SearchQuery::MaxPageNum() {
    return MAX_PAGE_NUM;
}

void SearchQuery::SetExtraStr(QString extra) {
    extraStr = extra;
}

QString SearchQuery::ExtraStr() {
    return extraStr;
}
