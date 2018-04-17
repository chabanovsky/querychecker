#ifndef QARUSITETESTCASE_H
#define QARUSITETESTCASE_H

#include <QList>
#include <QtWidgets>
#include <QString>
#include <QVariant>
#include <QObject>

QT_BEGIN_NAMESPACE
class QWebEngineView;
QT_END_NAMESPACE

#include "testcase.h"
#include "tsv_parser.h"
#include "utils.h"

class QaruSiteTestCase : public QObject, public TestCase {
    Q_OBJECT
public:
    QaruSiteTestCase (QWebEngineView * initView);
    virtual ~QaruSiteTestCase ();
    QaruSiteTestCase (QaruSiteTestCase  const &);
    QaruSiteTestCase & operator=(QaruSiteTestCase  const &);

public:
    virtual void Execute();
    virtual bool CanExecute();
    virtual bool Cancel();
    virtual QString DumpResults();

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


#endif // QARUSITETESTCASE_H
