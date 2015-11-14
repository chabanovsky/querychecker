#ifndef TESTSUITECASE
#define TESTSUITECASE

class TestCase {
public:
    TestCase();
    virtual ~TestCase();
    TestCase(TestCase const &);
    TestCase &operator=(TestCase const &);

public:
    virtual void Execute() = 0;
    virtual bool CanExecute() = 0;
    virtual bool Cancel() = 0;
    virtual void DumpResults() = 0;
};

#endif // TESTSUITECASE

