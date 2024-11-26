#include <web_server.h>
#include <routes/routes.h>

AsyncWebServer WebServer::server(80);

void WebServer::WebListener()
{
  server.on("/", HTTP_GET, handleRoot);
  server.on("/mac", HTTP_GET, handleGetSavedMac);
  server.on("/changeControlState", HTTP_POST, handleChangeState);
  server.onRequestBody(handleOnBodyPost);
  server.onNotFound(handleNotFound);
}
