#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "headers.h"

#define telegramBotToken "xxxxxxxx:xxxxxxxxxxxxxxxxxxxxx"

WiFiClientSecure wiFiClientSecure;
UniversalTelegramBot bot(telegramBotToken, wiFiClientSecure);

String GeneralInfoMessage()
{
    char temp[24];
    char umid[24];
    char rssi[24];
    dtostrf(temperatura, 4, 1, temp);
    dtostrf(umidade, 2, 0, umid);
    dtostrf(RSSI, 3, 0, rssi);

    String mensagem;
    mensagem += "*Aviário - Painel de Controle*\n";
    mensagem += "Temperatura: " + (String)temp + " ºC\n";
    mensagem += "Umidade: " + (String)umid + " %\n";
    mensagem += "Sinal Wifi: " + (String)rssi + " dBi\n";
    mensagem += "\n";
    mensagem += "Atualizado em " + (String)returnLocalTime().c_str();
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
            bot.sendMessage(chat_id, GeneralInfoMessage(), "Markdown");
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
        if (WiFi.isConnected())
        {
            int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

            while (numNewMessages)
            {
                Serial.println("got telegram response");
                ProcessarNovasMensagens(numNewMessages);
                numNewMessages = bot.getUpdates(bot.last_message_received + 1);
            }
        }
        vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
}