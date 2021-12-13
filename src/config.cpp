#include "headers.h"

const size_t JSON_SIZE = JSON_OBJECT_SIZE(5) + 96;

int bootCount = 0;

// Funções de Configuração ------------------------------
void configReset()
{
    // Define configuração padrão
    // strlcpy(id, "IeC Device", sizeof(id));
    // strlcpy(ssid, "", sizeof(ssid));
    // strlcpy(pw, "", sizeof(pw));
    VentilacaoMinimaEnabled = false;
}

boolean configRead()
{
    // Lê configuração
    StaticJsonDocument<JSON_SIZE> jsonConfig;

    File file = SPIFFS.open(F("/Config.json"), "r");
    if (deserializeJson(jsonConfig, file))
    {
        // Falha na leitura, assume valores padrão
        configReset();

        Serial.println(F("Falha lendo CONFIG, assumindo valores padrão."));
        return false;
    }
    else
    {
        // Sucesso na leitura
        // strlcpy(id, jsonConfig["id"] | "", sizeof(id));
        // strlcpy(ssid, jsonConfig["ssid"] | "", sizeof(ssid));
        // strlcpy(pw, jsonConfig["pw"] | "", sizeof(pw));
        VentilacaoMinimaEnabled = jsonConfig["ventMinEnabled"] | 0;
        VentilacaoMinimaTimeOn = jsonConfig["ventMinTimeOn"] | 0;
        VentilacaoMinimaTimeOff = jsonConfig["ventMinTimeOff"] | 0;

        file.close();

        Serial.println(F("\nLendo config:"));
        serializeJsonPretty(jsonConfig, Serial);
        Serial.println("");

        return true;
    }
}

boolean configSave()
{
    // Grava configuração
    StaticJsonDocument<JSON_SIZE> jsonConfig;

    File file = SPIFFS.open(F("/Config.json"), "w+");
    if (file)
    {
        // Atribui valores ao JSON e grava
        //  jsonConfig["id"] = id;
        jsonConfig["ventMinEnabled"] = VentilacaoMinimaEnabled;
        jsonConfig["ventMinTimeOn"] = VentilacaoMinimaTimeOn;
        jsonConfig["ventMinTimeOff"] = VentilacaoMinimaTimeOff;
        //  jsonConfig["ssid"] = ssid;
        //  jsonConfig["pw"] = pw;

        serializeJsonPretty(jsonConfig, file);
        file.close();

        Serial.println(F("\nGravando config:"));
        serializeJsonPretty(jsonConfig, Serial);
        Serial.println("");

        return true;
    }
    return false;
}
