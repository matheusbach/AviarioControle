#include "ESPAsyncWebServer.h"
#include "headers.h"
#include "webpage/pagina.cpp"
#include "Arduino.h"
#include "Wire.h"
#include "string"
#include <AsyncElegantOTA.h>;

const char *http_username = "";
const char *http_password = "bach";

const char *PARAM_INPUT_1 = "state";

AsyncWebServer server(80);
AsyncEventSource events("/events");

void sendEvent(const char *data, const char *name)
{
    events.send(data, String(name).c_str(), millis());
}

void TransmitInfoToWebServerClients()
{
    char result[7]; //buffer o suficiente pra float de 6 char
    sendEvent(dtostrf(temperatura, 6, 1, result), "temperature");
    sendEvent(dtostrf(umidade, 6, 0, result), "humidity");
    if (VentilacaoMinimaEnabled)
    {
        sendEvent("true", "ventMinCkeckBox");
    }
    else
    {
        sendEvent("false", "ventMinCkeckBox");
    }
    sendEvent(dtostrf(RSSI, 4, 0, result), "rssi");
    sendEvent(returnLocalTime().c_str(), "lastUpdatedTime");
}

String processor(const String &var)
{
    //Serial.println(var);
    if (var == "BUTTONPLACEHOLDER")
    {
        String buttons;
        String outputStateValue = outputState(ventiladorPin);
        buttons += "<div class=\"card card-switch\"><h4><i class=\"fas fa-lightbulb\"></i> Ventilação Mínima</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"controlVentMin(this)\" id=\"ventMinCkeckBox\" ><span class=\"slider\"></span></label></div>";
        return buttons;
    }
    return String();
}

void setupWebServer()
{
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    //  if (!request->authenticate(http_username, http_password))
    //     return request->requestAuthentication();
        request->send_P(200, "text/html", index_html, processor);
    });
    server.on("/logged-out", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", logout_html, processor);
    });
    server.on("/logout", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(401);
    });
    server.on("/get-data", HTTP_GET, [](AsyncWebServerRequest *request) 
    {
        TransmitInfoToWebServerClients();
    });
    // Send a GET request to control output socket <ESP_IP>/output?state=<inputMessage>
    server.on("/ventMin", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->authenticate(http_username, http_password))
            return request->requestAuthentication();
        String inputMessage;
        // GET gpio and state value
        if (request->hasParam(PARAM_INPUT_1))
        {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
            int tempHolderVentilacaoMinimaEnabled = VentilacaoMinimaEnabled;

            if (inputMessage == "1")
            {
                VentilacaoMinimaEnabled = 1;
                estadoAtualVentiacaoMinima = 1;
            }
            else
            {
                VentilacaoMinimaEnabled = 0;
                estadoAtualVentiacaoMinima = 0;
                VentiladorOnOf(0);
            }
            request->send(200, "text/plain", inputMessage);

            if (inputMessage.toInt() != tempHolderVentilacaoMinimaEnabled)
            {
                configSave();
            }
        }
        request->send(200, "text/plain", "Failed");
    });
    server.on("/ventMinTimeOn", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->authenticate(http_username, http_password))
            return request->requestAuthentication();
        String inputMessage;
        // GET gpio and state value
        if (request->hasParam(PARAM_INPUT_1))
        {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
        //    int tempHolderVentilacaoMinimaTimeOn = VentilacaoMinimaTimeOn;

            request->send(200, "text/plain", inputMessage);
            if (inputMessage.toInt() != VentilacaoMinimaTimeOn)
            {
                VentilacaoMinimaTimeOn = inputMessage.toInt();
                configSave();
            }
        }
        request->send(200, "text/plain", "Failed");
    });
    server.on("/ventMinTimeOff", HTTP_GET, [](AsyncWebServerRequest *request) {
        if (!request->authenticate(http_username, http_password))
            return request->requestAuthentication();
        String inputMessage;
        // GET gpio and state value
        if (request->hasParam(PARAM_INPUT_1))
        {
            inputMessage = request->getParam(PARAM_INPUT_1)->value();
        //    int tempHolderVentilacaoMinimaTimeOff = VentilacaoMinimaTimeOff;
            request->send(200, "text/plain", inputMessage);
            if (inputMessage.toInt() != VentilacaoMinimaTimeOff)
            {
                VentilacaoMinimaTimeOff = inputMessage.toInt();
                configSave();
            }
        }
        request->send(200, "text/plain", "Failed");
    });

    events.onConnect([](AsyncEventSourceClient *client) {
        if (client->lastId())
        {
            Serial.printf("Client reconnected! Last message ID that it got is: %u\n", client->lastId());
        }
        // send event with message "hello!", id current millis and set reconnect delay to 1 second
        client->send("hello!", NULL, millis(), 1000);
    });
    server.addHandler(&events);

    // Route for root / web page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send_P(200, "text/html", index_html, processor);
    });
    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", (String)temperatura);
    });
    server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", (String)umidade);
    });
    server.on("/rssi", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", (String)RSSI);
    });
    server.on("/lastUpdatedTime", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", String(returnLocalTime().c_str()));
    });

    AsyncElegantOTA.begin(&server);
    server.begin();
}
