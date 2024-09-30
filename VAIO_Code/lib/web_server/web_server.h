#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include "WString.h"
#include "ESPAsyncWebServer.h"
#include "master_control.h"
#include "Update.h"

class WebServer{
public:
    static String loginIndex;
    static String updateIndex;
    static String vaioIndex;
    static AsyncWebServer server;

    static void WebListener();
};
#endif