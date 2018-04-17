#include "qarusitetestcase.h"
#include "jquery.h"

#include <QtWidgets>
#include <QtWebEngineWidgets>
#include <QDebug>
#include <stdexcept>

QaruSiteTestCase::QaruSiteTestCase(QWebEngineView * initView)
    : TestCase(),
    view(initView),
    requestUrl(DEFAULT_GOOGLE_SEARCH_URL),
    currentPage(0),
    currentQueryIndex(-1),
    searchQueries(){

    TSVParser parser(":/output.json");
    searchQueries = parser.Parse();

    SearchQuery::SetMaxPageNum(1);

    connect(view, SIGNAL(loadFinished(bool)), SLOT(finishLoading(bool)));
}

QaruSiteTestCase::~QaruSiteTestCase() {
}

QaruSiteTestCase::QaruSiteTestCase(QaruSiteTestCase const & other)
    : TestCase(other),
      view(other.view),
      requestUrl(other.requestUrl),
      currentPage(other.currentPage),
      currentQueryIndex(other.currentQueryIndex),
      searchQueries(other.searchQueries) {
}

QaruSiteTestCase& QaruSiteTestCase::operator=(QaruSiteTestCase const & other) {
    TestCase::operator=(other);
    return *this;
}

void QaruSiteTestCase::Execute() {
    runNextQuery();
}

bool QaruSiteTestCase::CanExecute() {
    return true;
}

bool QaruSiteTestCase::Cancel() {
    return false;
}

SearchQuery& QaruSiteTestCase::query() {
    if (currentQueryIndex < 0 || currentQueryIndex > searchQueries.count()) {
        throw std::invalid_argument("Invalid query index");
    }
    return searchQueries[currentQueryIndex];
}

QString QaruSiteTestCase::DumpResults() {
    QString dump;
    for (int index = 0; index < searchQueries.count(); ++index) {
        SearchQuery & query = searchQueries[index];
        dump += QString("Query # ")
             + QString::number(query.GoogleRank())
             + QString(", target page: ")
             + query.ExtraStr()
             + QString(", query: ")
             + query.Query()
             + QString("\n");
    }
    return dump;
}

void QaruSiteTestCase::runNextQuery() {
    currentPage = 0;
    ++currentQueryIndex;
    if (currentQueryIndex >= searchQueries.count()) {
        onTestSuiteFinished();
        return;
    }

    SearchQuery & nextQuery = query();
    view->load(QUrl(requestUrl + nextQuery.EncodingQuery()));
}

void QaruSiteTestCase::onResultFoundCallback(const QVariant& returnValue) {
    QString result = "";
    bool isOk = false;

    result = returnValue.toString();

    if (result.length() > 0) {
        query().SetExtraStr(result);
    }

    QTime dieTime= QTime::currentTime().addSecs(3);
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    runNextQuery();
}

void QaruSiteTestCase::finishLoading(bool) {
    view->page()->runJavaScript(JQuery::Instance()->Code());
    view->page()->runJavaScript(FIND_QARU_GOOGLE_JS, invoke(this, &QaruSiteTestCase::onResultFoundCallback));
}

void QaruSiteTestCase::onTestSuiteFinished() {
    disconnect(view, SIGNAL(loadFinished(bool)));
    emit TestSuiteFinished();
}

