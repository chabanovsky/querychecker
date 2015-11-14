#include <QtWidgets>
#include <QtWebEngineWidgets>
#include <QDebug>
#include <stdexcept>

#include "searchquerytestcase.h"
#include "jquery.h"

namespace {
const QString DEFAULT_SEARCH_URL = "https://yandex.ru/search/?lr=2&text=";
const QString FIND_SORU_IN_YA_JS = " \
        function findUrl(){ \
            var search_url = 'ru.stackoverflow.com'; \
            var results = $('.serp-item'); \
            for (var index = 0; index < results.length; index++) { \
                var item = results[index]; \
                var anchor = $(item).find('.serp-url__link'); \
                var link = $(anchor).attr('href'); \
                if (link == undefined) { \
                    continue; \
                } \
                if (link.indexOf(search_url) > 1){ \
                    return parseInt($(item).attr('aria-posinset')); \
                } \
                return -1; \
            } \
        }; findUrl();";
}

SearchQueryTestCase::SearchQueryTestCase(QWebEngineView * initView)
    : TestCase(),
    view(initView),
    requestUrl(DEFAULT_SEARCH_URL),
    currentPage(0),
    currentQueryIndex(-1),
    searchQueries(){

    Parser parser(":/stats.json");
    searchQueries = parser.Parse();

    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
}

SearchQueryTestCase::~SearchQueryTestCase() {
}

SearchQueryTestCase::SearchQueryTestCase(SearchQueryTestCase const & other)
    : TestCase(other),
      view(other.view),
      requestUrl(other.requestUrl),
      currentPage(other.currentPage),
      currentQueryIndex(other.currentQueryIndex),
      searchQueries(other.searchQueries) {
}

SearchQueryTestCase& SearchQueryTestCase::operator=(SearchQueryTestCase const & other) {
    TestCase::operator=(other);
    return *this;
}

void SearchQueryTestCase::Execute() {
    runNextQuery();
}

bool SearchQueryTestCase::CanExecute() {
    return true;
}

bool SearchQueryTestCase::Cancel() {
    return false;
}

SearchQuery& SearchQueryTestCase::query() {
    if (currentQueryIndex < 0 || currentQueryIndex > searchQueries.count()) {
        throw std::invalid_argument("Invalid query index");
    }
    return searchQueries[currentQueryIndex];
}

void SearchQueryTestCase::DumpResults() {
    for (int index = 0; index < searchQueries.count(); ++index) {
        SearchQuery & query = searchQueries[index];
        qDebug() << "Query: " << query.Query() << ", in google: " << query.GoogleRank() << ", in yandex: " << query.YandexRank();
    }
}

void SearchQueryTestCase::runNextQuery() {
    currentPage = 0;
    ++currentQueryIndex;
    if (currentQueryIndex >= searchQueries.count()) {
        onTestSuiteFinished();
        return;
    }

    SearchQuery & nextQuery = query();
    view->load(QUrl(requestUrl + nextQuery.EncodingQuery()));
}

void SearchQueryTestCase::onResultFoundCallback(const QVariant& returnValue) {
    int result = -1;
    bool isOk = false;

    result = returnValue.toInt(&isOk);

    if (!isOk) {
        runNextQuery();
        return;
    }
    if (result > -1) {
        query().SetYandexRank(result);
        runNextQuery();
        return;
    }
    ++currentPage;
    if (currentPage >= SearchQuery::MaxPageNum()) {
        runNextQuery();
        return;
    }
    QUrl url = QUrl(requestUrl + query().EncodingQuery() + "&p=" + QString::number(currentPage));
    view->load(url);
}

void SearchQueryTestCase::finishLoading(bool) {
    view->page()->runJavaScript(JQuery::Instance()->Code());
    view->page()->runJavaScript(FIND_SORU_IN_YA_JS, invoke(this, &SearchQueryTestCase::onResultFoundCallback));
}

void SearchQueryTestCase::onTestSuiteFinished() {
    emit TestSuiteFinished();

    DumpResults();
}

