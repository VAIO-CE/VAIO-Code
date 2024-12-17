#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "master_control.h"
#include "routes/routes.h"
#include <WString.h>
#include "ESPAsyncWebServer.h"
#include <Update.h>

class WebServer
{
public:
    static inline AsyncWebServer server{80};

    static void WebListener();
};
#endif
