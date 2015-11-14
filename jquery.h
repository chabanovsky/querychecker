#ifndef JQUERY
#define JQUERY

#include <QString>

class JQuery {
private:
    JQuery();

public:
    QString Code();

public:
    static JQuery * Instance();

private:
    QString jQuery;

private:
    static JQuery * instance;
};

#endif // JQUERY

