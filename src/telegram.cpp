#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <ArduinoJson.h>
#include "headers.h"

#define telegramBotToken "xxxxxxxx:xxxxxxxxxxxxxxxxxxxxx"

WiFiClientSecure wiFiClientSecure;
UniversalTelegramBot bot(telegramBotToken, wiFiClientSecure);

String GeneralInfoMessage()
{
    char temp[6];
    char umid[6];
    char rssi[4];
    dtostrf(temperatura, 6, 1, temp);
    dtostrf(umidade, 6, 0, umid);
    dtostrf(RSSI, 4, 0, rssi);

    String mensagem;
    mensagem +="Aviário - Painel de Controle\n";
    mensagem += "Temperatura: " + (String)temp + " oC\n";
    mensagem += "Umidade: " + (String)umid + " %\n";
    mensagem += "Sinal Wifi: " + (String)rssi + " %\n";
    mensagem += "\n";
    mensagem += "\nAtualizado em " + (String)returnLocalTime().c_str();
    return mensagem;
}

void ProcessarNovasMensagens(int numNewMessages)
{
    Serial.println("Novas mensagens para processar: " + String(numNewMessages));

    for (int i = 0; i < numNewMessages; i++)
    {
        String chat_id = String(bot.messages[i].chat_id);

        String mensagem = bot.messages[i].text;
        Serial.println("Mensagem recebida: " + mensagem);

        String from_name = bot.messages[i].from_name;

        if (mensagem == "/start")
        {
            String mensagem = "Bem-vindo, " + from_name + ".\n";
            mensagem += "Para ler as informações básicas dos sensores use o comando abaixo.\n\n";
            mensagem += "/info \n";
            bot.sendMessage(chat_id, mensagem, "");
        }

        if (mensagem == "/info")
        {
            bot.sendMessage(chat_id, GeneralInfoMessage(), "");
        }
    }
}

void setupTelegramBot()
{
    wiFiClientSecure.setCACert(TELEGRAM_CERTIFICATE_ROOT); // Add root certificate for api.telegram.org
}

void VerificarMensagensTelegram(void *parameter)
{
    while (true)
    {
        int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

        while (numNewMessages)
        {
            Serial.println("got response");
            ProcessarNovasMensagens(numNewMessages);
            numNewMessages = bot.getUpdates(bot.last_message_received + 1);
        }

        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
}