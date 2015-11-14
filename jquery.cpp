#include <QFile>
#include "jquery.h"

JQuery * JQuery::instance = NULL;

JQuery::JQuery() {
    QFile file;
    file.setFileName(":/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    jQuery.append("\nvar qt = { 'jQuery': jQuery.noConflict(true) };");
    file.close();
}

QString JQuery::Code() {
    return jQuery;
}

JQuery * JQuery::Instance() {
    if (instance == NULL)
        instance = new JQuery();
    return instance;
}

