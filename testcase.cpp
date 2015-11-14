#include <QObject>
#include "testcase.h"

TestCase::TestCase() {
}
TestCase::~TestCase() {
}
TestCase::TestCase(TestCase const &) {
}
TestCase & TestCase::operator=(TestCase const &) {
    return *this;
}
