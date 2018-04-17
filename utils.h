#ifndef UTILS
#define UTILS

template<typename Arg, typename R, typename C>
struct InvokeWrapper {
    R *receiver;
    void (C::*memberFun)(Arg);
    void operator()(Arg result) {
        (receiver->*memberFun)(result);
    }
};

template<typename Arg, typename R, typename C>
InvokeWrapper<Arg, R, C> invoke(R *receiver, void (C::*memberFun)(Arg)) {
    InvokeWrapper<Arg, R, C> wrapper = {receiver, memberFun};
    return wrapper;
}

const QString DEFAULT_YANDEX_SEARCH_URL = "https://yandex.ru/search/?lr=2&text=";
const QString DEFAULT_GOOGLE_SEARCH_URL = "https://www.google.ru/search?q=";
const QString DEFAULT_BASE_URL = "http://ru.stackoverflow.com";
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
const QString FIND_QARU_GOOGLE_JS = " \
        function findUrl(){ \
            var search_url = 'qaru.site'; \
            var results = document.getElementsByClassName('r') ; \
            for (var index = 0; index < results.length; index++) { \
                var item = results[index]; \
                var link = item.getElementsByTagName('a')[0]; \
                if (link == undefined) { \
                    continue; \
                } \
                if (link.hostname.includes(search_url)) { \
                    return link.href; \
                } \
            } \
            return ''; \
        }; findUrl();";
const QString DEFAULT_SORU_QUESTION_URL = "http://ru.stackoverflow.com/questions?sort=newest";
const QString QUESTION_LIST_JS = " \
    function questionsList() { \
        var result = []; \
        var questions = $('.question-summary'); \
        for (var index = 0; index < questions.length; index++) { \
            var question = questions[index]; \
            var item = {text: $(question).find('h3').text(), link: $(question).find('h3 a').attr('href')}; \
            result.push(item); \
        } \
        return result; \
    }; questionsList();";
const QString QUESTION_WITH_ANSWER_LIST_JS = " \
    function questionsList() { \
        var result = []; \
        var questions = $('.question-summary'); \
        for (var index = 0; index < questions.length; index++) { \
            var question = questions[index]; \
            var status = $(question).find('.status strong')[0]; \
            var hasAnswers = parseInt($(status).text()) > 0; \
            if (!hasAnswers) \
                continue; \
            var item = {text: $(question).find('h3').text(), link: $(question).find('h3 a').attr('href')};  \
            result.push(item); \
        } \
        return result; \
    }; questionsList();";
const QString ANSWERED_QUESTION_LIST_JS = "\
    function questionsList() { \
        var result = []; \
        var questions = $('.question-summary'); \
        for (var index = 0; index < questions.length; index++) { \
            var question = questions[index]; \
            var status = $(question).find('.status')[0]; \
            if (!$(status).hasClass('answered')) \
                continue; \
            var item = {text: $(question).find('h3').text(), link: $(question).find('h3 a').attr('href')}; \
            result.push(item); \
        } \
        return result; \
    }; questionsList();";
const int QUESTION_TO_SEARCH_LIMIT = 200; // Read 200 newest questions on Stack Overflow in Russia
const int DEFAULT_MAX_PAGE_NUM = 3; // How many pages with search results on yandex.ru we shold check for a question

#endif // UTILS

