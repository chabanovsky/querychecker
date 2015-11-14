#ifndef SEARCHQUERYTESTCASE
#define SEARCHQUERYTESTCASE

#include <QtWidgets>
#include <QList>
#include <QString>
#include <QVariant>
#include <QObject>

QT_BEGIN_NAMESPACE
class QWebEngineView;
QT_END_NAMESPACE

#include "testcase.h"
#include "parser.h"
#include "utils.h"

class SearchQueryTestCase : public QObject, public TestCase {
    Q_OBJECT
public:
    SearchQueryTestCase(QWebEngineView * initView);
    virtual ~SearchQueryTestCase();
    SearchQueryTestCase(SearchQueryTestCase const &);
    SearchQueryTestCase&operator=(SearchQueryTestCase const &);

public:
    virtual void Execute();
    virtual bool CanExecute();
    virtual bool Cancel();
    virtual void DumpResults();

signals:
    void TestSuiteFinished();

protected slots:
    void finishLoading(bool);

private:
    void runNextQuery();
    SearchQuery& query();
    void onTestSuiteFinished();
    void onResultFoundCallback(const QVariant& returnValue);

private:
    QWebEngineView * view;
    QString requestUrl;
    int currentPage;
    int currentQueryIndex;
    QList<SearchQuery> searchQueries;
};

#endif // SEARCHQUERYTESTCASE

