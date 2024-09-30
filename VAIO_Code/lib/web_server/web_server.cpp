#include <web_server.h>

AsyncWebServer WebServer::server(80);

static String style =
    "<style>#file-input,input{width:100%;height:44px;border-radius:4px;margin:10px auto;font-size:15px}"
    "input{background:#f1f1f1;border:0;padding:0 15px}body{background:#3498db;font-family:sans-serif;font-size:14px;color:#777}"
    "#file-input{padding:0;border:1px solid #ddd;line-height:44px;text-align:left;display:block;cursor:pointer}"
    "#bar,#prgbar{background-color:#f1f1f1;border-radius:10px}#bar{background-color:#3498db;width:0%;height:10px}"
    "form{background:#fff;max-width:258px;margin:75px auto;padding:30px;border-radius:5px;text-align:center}"
    ".btn{background:#3498db;color:#fff;cursor:pointer}</style>";

/* Login page */
String WebServer::loginIndex = 
"<form name=loginForm>"
"<h1>VAIO Login</h1>"
"<input name=userid placeholder='User ID'> "
"<input name=pwd placeholder=Password type=Password> "
"<input type=submit onclick=check(this.form) class=btn value=Login></form>"
"<script>"
"function check(form) {"
"if(form.userid.value=='admin' && form.pwd.value=='admin')"
"{window.open('/vaio')}"
"else"
"{alert('Error Password or Username')}"
"}"
"</script>" + style;

/* VAIO Index Page */
String WebServer::vaioIndex = 
"<html>"
"<head>"
"<title>VAIO Control Status</title>"
"<style>" + style + "</style>"
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<script>"
"function changeControlState(state) {"
"  fetch('/changeControlState', {"
"    method: 'POST',"
"    headers: {"
"      'Content-Type': 'application/json'"
"    },"
"    body: JSON.stringify({ state: state })"
"  });"
"}"
"</script>"
"<body>"
"<form>"
"<h1>VAIO Control Status</h1>"
"<p>Current Control State: <span id='controlState'></span></p>"
"<div style='display: flex; flex-direction: row; gap: 10px;'>"
"<button class='btn' style='width: 120px; height: 40px; border-radius: 10px;' onclick='changeControlState(\"AUTO_CONTROL\")'>Auto Control</button>"
"<button class='btn' style='width: 120px; height: 40px; border-radius: 10px;' onclick='changeControlState(\"GYRO_CONTROL\")'>Gyro Control</button>"
"<button class='btn' style='width: 120px; height: 40px; border-radius: 10px;' onclick='changeControlState(\"DS4_CONTROL\")'>DS4 Control</button>"
"</div>"
"</form>"
"<script>"
"fetch('/getControlState', {"
"  method: 'GET'"
"}).then(response => response.json())"
".then(data => {"
"  document.getElementById('controlState').textContent = data.state;"
"});"
"</script>"
"</body>"
"</html>";

/* Server Index Page */
String WebServer::updateIndex = 
"<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.2.1/jquery.min.js'></script>"
"<form method='POST' action='/updateFirmware' enctype='multipart/form-data' id='upload_form'>"
"<input type='file' name='update' id='file' onchange='sub(this)' style=display:none>"
"<label id='file-input' for='file'>   Choose file...</label>"
"<input type='submit' class=btn value='Update'>"
"<br><br>"
"<div id='prg'></div>"
"<br><div id='prgbar'><div id='bar'></div></div><br></form>"
"<script>"
"function sub(obj){"
"var fileName = obj.value.split('\\\\');"
"document.getElementById('file-input').innerHTML = '   '+ fileName[fileName.length-1];"
"};"
"$('form').submit(function(e){"
"e.preventDefault();"
"var form = $('#upload_form')[0];"
"var data = new FormData(form);"
"$.ajax({"
"url: '/updateFirmware',"
"type: 'POST',"
"data: data,"
"contentType: false,"
"processData:false,"
"xhr: function() {"
"var xhr = new window.XMLHttpRequest();"
"xhr.upload.addEventListener('progress', function(evt) {"
"if (evt.lengthComputable) {"
"var per = evt.loaded / evt.total;"
"$('#prg').html('progress: ' + Math.round(per*100) + '%');"
"$('#bar').css('width',Math.round(per*100) + '%');"
"}"
"}, false);"
"return xhr;"
"},"
"success:function(d, s) {"
"console.log('success!') "
"},"
"error: function (a, b, c) {"
"}"
"});"
"});"
"</script>" + style;


void WebServer::WebListener(){
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", loginIndex); 
    response->addHeader("Connection", "close");
    request->send(response);
  });

  server.on("/vaio", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", vaioIndex); 
    response->addHeader("Connection", "close");
    request->send(response);
  });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/html", updateIndex);
    response->addHeader("Connection", "close");
    request->send(response);
  });

  server.on("/updateFirmware", HTTP_POST, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse(200, "text/plain", (Update.hasError()) ? "FAIL" : "OK");
    response->addHeader("Connection", "close");
    request->send(response);
    ESP.restart();
  }, [](AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    if (!index) {
      Serial.printf("Update: %s\n", filename.c_str());
      if (!Update.begin(UPDATE_SIZE_UNKNOWN)) { //start with max available size
        Update.printError(Serial);
      }
    }
    if (!Update.write(data, len)) {
      Update.printError(Serial);
    }
    if (final) {
      if (Update.end(true)) { //true to set the size to the current progress
        Serial.printf("Update Success: %u\nRebooting...\n", index + len);
      } else {
        Update.printError(Serial);
      }
    }
  });

  server.on("/getControlState", HTTP_GET, [](AsyncWebServerRequest *request){
    String state;

    switch (MasterControl::currentControlMode) {
      case ControlState::AUTO_CONTROL:
        state = "AUTO_CONTROL";
        break;
      case ControlState::GYRO_CONTROL:
        state = "GYRO_CONTROL";
        break;
      case ControlState::DS4_CONTROL:
        state = "DS4_CONTROL";
        break;
    }
    request->send(200, "application/json", "{\"state\": \"" + state + "\"}");
  });

  server.on("/changeControlState", HTTP_POST, [](AsyncWebServerRequest *request){
    String state = request->arg("state");
    ControlState newControlState;

    if (state == "AUTO_CONTROL") newControlState = ControlState::AUTO_CONTROL;
    else if (state == "GYRO_CONTROL") newControlState = ControlState::GYRO_CONTROL;
    else if (state == "DS4_CONTROL") newControlState = ControlState::DS4_CONTROL;

    MasterControl::setControlMode(newControlState);
    request->send(200, "text/plain", "Control state changed");
  });
}


