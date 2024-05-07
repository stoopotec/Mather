#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include "list.h"




enum METHOD_E {
    UNDEFINED_METHOD = 0,
    GET  = 1,
    POST = 2,
    CALC = 3,
};

enum METHOD_E get_method(char* http_header);

const char* define_content_type(const char* filename);


typedef struct url_arg {
    const char* key;
    size_t key_len;
    const char* val;
    size_t val_len;
} url_arg_t;


typedef LIST(url_arg_t) list_url_arg_t;
char* get_url_alloc(char* http);

list_url_arg_t get_url_args(const char* url);

char* get_path_alloc(char* url, const char* add_to_start);

















struct header {
    LIST_ANON(struct {const char* key; const char* value}) headers;
};




enum HTTP_CODES_E {

// Информационные ответы

    // Промежуточный ответ, он указывает, что клиент должен продолжить запрос или игнорировать этот ответ, если запрос уже завершен.
    CODE_100_CONTINUE = 100,

    // Этот код отправляется в ответ на заголовок запроса Upgrade от клиента и указывает протокол, на который переключается сервер.
    CODE_101_SWITCHING_PROTOCOLS = 101,

    // Сервер получил и обрабатывает запрос, но ответа пока нет.
    CODE_102_PROCESSING = 102,

    // Этот код в первую очередь предназначен для использования с заголовком Link, позволяя пользовательскому агенту начать предварительную загрузку ресурсов или осуществить предварительное соединение к источнику ресурсов, пока сервер готовит ответ.
    CODE_103_EARLY_HINTS = 103,


// Успешные ответы

    // Запрос успешно выполнен. Значение результата «успех» зависит от метода HTTP:
    //     GET: Ресурс был получен и передан в теле сообщения.
    //     HEAD: Ответ содержит заголовки, но тела сообщения нет.
    //     PUT или POST: Ресурс, описывающий результат действия, передан в теле сообщения.
    //     TRACE: Тело сообщения содержит сообщение запроса, полученное сервером.
    CODE_200_OK = 200,

    // Запрос выполнен успешно, и в результате был создан новый ресурс. Обычно это ответ, отправляемый на запросы POST или PUT.
    CODE_201_CREATED = 201,

    // Запрос получен, но еще не обработан. Это «уклончивый» ответ, поскольку в HTTP нет возможности позже отправить асинхронный ответ с результатом обработки запроса. Этот код предназначен для случаев, когда запрос обрабатывается другим процессом или сервером, а также для пакетной обработки.
    CODE_202_ACCEPTED = 202,

    // Возвращенные метаданные не полностью совпадают с теми, которые доступны на исходном сервере, а получены из другого источника. Чаще всего это используется для зеркал или резервных копий ресурсов. За исключением таких случаев предпочтительнее использовать ответ 200 OK.
    CODE_203_NON_AUTHORITATIVE_INFORMATION = 203,

    // Для этого запроса нет содержимого для отправки, но заголовки ответа могут быть полезны. Пользовательский агент может использовать их для обновления закешированных заголовков, полученных ранее для этого ресурса.
    CODE_204_NO_CONTENT = 204,

    // Сообщает пользовательскому агенту, что необходимо сбросить отображение документа, который отправил этот запрос.
    CODE_205_RESET_CONTENT = 205,

    // Этот код ответа используется, когда от клиента отправляется заголовок Range для запроса только части ресурса.
    CODE_206_PARTIAL_CONTENT = 206,

    // Передаёт информацию о нескольких ресурсах в случаях, когда могут быть уместны несколько кодов состояния.
    CODE_207_MULTI_STATUS = 207,

    // Используется внутри элемента ответа <dav:propstat>, чтобы избежать повторного перечисления «привязок» и дублирования данных.
    CODE_208_ALREADY_REPORTED = 208,

    // Используется для ответа на запросы GET в тех случаях, когда сервер хочет отправить только изменённую часть ресурса (то есть «дельту»).
    CODE_226_IM_USED = 226,


// Сообщения о перенаправлении

    // У запроса более одного возможного ответа. Пользовательский агент или пользователь должен выбрать один из них. Не существует стандартизированного способа выбора одного из ответов, но рекомендуется использовать HTML-ссылки на возможные варианты, чтобы у пользователя была возможность выбора.
    CODE_300_MULTIPLE_CHOICES = 300,

    // URL-адрес запрошенного ресурса был изменен навсегда. Новый URL-адрес указан в ответе.
    CODE_301_MOVED_PERMANENTLY = 301,

    // URI запрошенного ресурса был временно изменен. В будущем могут быть внесены дальнейшие изменения в URI. Следовательно, этот же URI должен использоваться клиентом в будущих запросах.
    CODE_302_FOUND = 302,

    // Клиенту необходимо получить запрошенный ресурс по другому URI с помощью запроса GET.
    CODE_303_SEE_OTHER = 303,

    // Этот код используется для целей кэширования. Он сообщает клиенту, что ответ не был изменен, поэтому клиент может продолжать использовать кэшированную версию ответа.
    CODE_304_NOT_MODIFIED = 304,

    // Запрошенный ответ должен быть доступен через прокси. Этот код определен в предыдущей версии спецификации HTTP и в настоящее время устарел из-за проблем безопасности, связанных с внутренней настройкой прокси-сервера.
    CODE_305_USE_PROXY = 305, // depricated

    // Этот код ответа зарезервирован. Использовался в предыдущей версии спецификации HTTP/1.1.
    CODE_306_UNUSED = 306,

    // Клиенту необходимо получить запрошенный ресурс по другому URI тем же методом, который использовался в предыдущем запросе. Он имеет ту же семантику, что и код ответа 302 Found, за исключением того, что пользовательский агент не должен изменять используемый метод: если в первом запросе использовался POST, то POST должен использоваться и во втором запросе.
    CODE_307_TEMPORARY_REDIRECT = 307,

    // Ресурс теперь находится по другому URI, указанному в заголовке ответа Location. Он имеет ту же семантику, что и код ответа 301 Moved Permanently, за исключением того, что пользовательский агент не должен изменять используемый метод: если в первом запросе использовался POST, то POST должен использоваться и во втором запросе.
    CODE_308_PERMANENT_REDIRECT = 308,


// Ошибки клиента

    // Сервер не может или не будет обрабатывать запрос из-за чего-то, что воспринимается как ошибка клиента (например, неправильный синтаксис, формат или маршрутизация запроса).
    CODE_400_BAD_REQUEST = 400,

    // Хотя стандарт HTTP определяет этот ответ как «неавторизованный», семантически он означает «неаутентифицированный». Это значит, что клиент должен аутентифицировать себя, чтобы получить запрошенный ответ.
    CODE_401_UNAUTHORIZED = 401,

    // Этот код ответа зарезервирован для использования в будущем. Первоначальной целью создания этого кода было использование его для цифровых платежных систем, однако он используется очень редко и стандартного соглашения не существует.
    CODE_402_PAYMENT_REQUIRED = 402, // experemental

    // Клиент не имеет прав доступа к контенту, то есть он неавторизован, поэтому сервер отказывается предоставить запрошенный ресурс. В отличие от 401 Unauthorized, личность клиента известна серверу.
    CODE_403_FORBIDDEN = 403,

    // Сервер не может найти запрошенный ресурс. В браузере это означает, что URL-адрес не распознан. В API это также может означать, что адрес правильный, но ресурс не существует. Сервер также может отправить этот код ответа вместо 403 Forbidden, чтобы скрыть существование ресурса от неавторизованного клиента. Это самый известный код ответа из-за его частого появления в сети.
    CODE_404_NOT_FOUND = 404,

    // Метод запроса известен серверу, но не поддерживается целевым ресурсом. Например, API может не разрешать вызов DELETE для удаления ресурса.
    CODE_405_METHOD_NOT_ALLOWED = 405,

    // Сервер после выполнения согласования контента не нашёл содержимого, соответствующего критериям, заданным пользовательским агентом.
    CODE_406_NOT_ACCEPTABLE = 406,

    // Этот код ответа похож на 401 Unauthorized, но аутентификация должна выполняться через прокси-сервер.
    CODE_407_PROXY_AUTHENTICATION_REQUIRED = 407,

    // Сервер может отправить этот код ответа при неиспользовании соединения, даже без предварительного запроса со стороны клиента. Он означает, что сервер хотел бы закрыть это соединение. Этот ответ используется сравнительно часто, поскольку некоторые браузеры (такие как Chrome, Firefox 27+ или IE9) для ускорения используют механизмы предварительного подключения HTTP. Некоторые серверы просто закрывают соединение, не отправляя это сообщение.
    CODE_408_REQUEST_TIMEOUT = 408,

    // Запрос конфликтует с текущим состоянием сервера.
    CODE_409_CONFLICT = 409,

    // Запрошенное содержимое было удалено с сервера, и отсутствует возможность переадресации. Ожидается, что клиенты удалят свои кеши и ссылки на этот ресурс. Спецификация HTTP предполагает, что этот код ответа будет использоваться для «ограниченных по времени или рекламных услуг». API не обязаны указывать ресурсы, которые были удалены, с помощью этого кода.
    CODE_410_GONE = 410,

    // Запрос отклонён, потому что сервер требует указание поля заголовка Content-Length, но оно не определено.
    CODE_411_LENGTH_REQUIRED = 411,

    // Клиент указал в заголовках запроса условия, которым сервер не соответствует.
    CODE_412_PRECONDITION_FAILED = 412,

    // Размер объекта запроса превышает ограничения, определенные сервером. Сервер может закрыть соединение или вернуть поле заголовка Retry-After.
    CODE_413_PAYLOAD_TOO_LARGE = 413,

    // Запрошенный клиентом URI слишком длинный для того, чтобы сервер смог его обработать.
    CODE_414_URI_TOO_LONG = 414,

    // Запрос отклонён, потому что медиа формат запрашиваемых данных не поддерживается сервером.
    CODE_415_UNSUPPORTED_MEDIA_TYPE = 415,

    // Сервер не может корректно обработать запрос с учётом диапазона, указанного в поле заголовка Range.
    CODE_416_RANGE_NOT_SATISFIABLE = 416,

    // Сервер не может выполнить ожидание, указанное в поле заголовка запроса Expect.
    CODE_417_EXPECTATION_FAILED = 417,

    // «Шуточный» ответ: сервер отклоняет попытку заварить кофе в чайнике.
    CODE_418_IM_A_TEAPOT = 418,

    // Сервер не может дать ответ. Возможно сервер не настроен для комбинации схемы и полномочий, указанных в URI запроса.
    CODE_421_MISDIRECTED_REQUEST = 421,

    // Запрос сформирован правильно, но его невозможно выполнить из-за семантических ошибок.
    CODE_422_UNPROCESSABLE_CONTENT = 422,

    // Запрашиваемый ресурс заблокирован.
    CODE_423_LOCKED = 423,

    // Запрос не выполнен из-за проблем в предыдущем запросе.
    CODE_424_FAILED_DEPENDENCY = 424,

    // Клиент отправил порцию данных слишком рано, до того как сервер установил безопасное соединение.
    CODE_425_TOO_EARLY = 425, // experemental

    // Сервер отказывается выполнять запрос с использованием текущего протокола, но может быть готов сделать это после смены протокола на указанный в заголовке ответа Upgrade.
    CODE_426_UPGRADE_REQUIRED = 426,

    // Сервер требует, чтобы запрос содержал условия. Этот ответ предназначен для предотвращения проблемы одновременного изменения ресурса несколькими пользователями.
    CODE_428_PRECONDITION_REQUIRED = 428,

    // Пользователь отправил слишком много запросов в определённый промежуток времени.
    CODE_429_TOO_MANY_REQUESTS = 429,

    // Сервер не будет обрабатывать запрос, потому что поля заголовка слишком велики. Запрос может быть отправлен повторно после уменьшения размера полей заголовка.
    CODE_431_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,

    // Пользовательский агент запросил ресурс, который не может быть предоставлен по закону, например веб-страницу, подвергнутую цензуре со стороны правительства.
    CODE_451_UNAVAILABLE_FOR_LEGAL_REASONS = 451,


// Ошибки сервера

    // На сервере произошла ошибка, в результате которой он не может успешно обработать запрос.
    CODE_500_INTERNAL_SERVER_ERROR = 500,

    // Метод запроса не поддерживается сервером и поэтому он не может быть обработан. Методы GET и HEAD должны всегда поддерживаться сервером и для них не должен возвращаться этот код.
    CODE_501_NOT_IMPLEMENTED = 501,

    // Такой ответ может прислать сервер, который выступает в качестве шлюза, если в процессе обработки запроса он получил недопустимый ответ от целевого сервера.
    CODE_502_BAD_GATEWAY = 502,

    // Сервер не готов обработать запрос в данный момент. Распространёнными причинами являются техническое обслуживание или перегрузка сервера. Вместе с таким ответом следует отправлять удобную для пользователя страницу с объяснением проблемы, а также HTTP-заголовок Retry-After с расчётным временем решения проблемы. Кроме того, полезно отправлять заголовки с информацией о кешировании, поскольку эти временные ответы обычно не следует кэшировать.
    CODE_503_SERVICE_UNAVAILABLE = 503,

    // Такой ответ может прислать сервер, который выступает в качестве шлюза, если в процессе обработки запроса он не может вовремя получить ответ от целевого сервера.
    CODE_504_GATEWAY_TIMEOUT = 504,

    // Используемая в запросе версия HTTP не поддерживается сервером.
    CODE_505_HTTP_VERSION_NOT_SUPPORTED = 505,

    // На сервере произошла внутренняя ошибка конфигурации: выбранный в процессе согласования вариант ресурса не является подходящим.
    CODE_506_VARIANT_ALSO_NEGOTIATES = 506,

    // Запрос не выполнен, потому что серверу не удалось сохранить данные.
    CODE_507_INSUFFICIENT_STORAGE = 507,

    // Запрос не выполнен, потому что на сервере был обнаружен бесконечный цикл обработки данных.
    CODE_508_LOOP_DETECTED = 508,

    // Для выполнения запроса необходимо его расширить.
    CODE_510_NOT_EXTENDED = 510,

    // Клиенту необходимо пройти аутентификацию для получения доступа к сети.
    CODE_511_NETWORK_AUTHENTICATION_REQUIRED = 511,


};

const char* get_code_comment(enum HTTP_CODES_E code);