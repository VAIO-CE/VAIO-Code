#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "WString.h"
#include "ESPAsyncWebServer.h"
#include "master_control.h"
#include "Update.h"

class WebServer
{
public:
    static AsyncWebServer server;

    static void WebListener();
};
#endif