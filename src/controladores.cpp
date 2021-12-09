#include "Arduino.h"
#include "Wire.h"
#include "headers.h"
#include "FreeRTOS.h"

const int ventiladorPin = 16; // Output socket

int VentilacaoMinimaEnabled;

int VentilacaoMinimaTimeOn = 0;
int VentilacaoMinimaTimeOff = 0;
int segundosVentilacaoMinimaActualState = 0;
int estadoAtualVentiacaoMinima = 0;
int ventiladorState = 0;

void VentiladorOnOf(int state)
{
    digitalWrite(ventiladorPin, !state);
    ventiladorState = state;
}

void DoVentilacaoMinima(void *parameter)
{
    while (true)
    {
        if (VentilacaoMinimaEnabled == 1)
        {
            if (estadoAtualVentiacaoMinima == 1)
            {
                segundosVentilacaoMinimaActualState++;

                if (segundosVentilacaoMinimaActualState >= VentilacaoMinimaTimeOn)
                {
                    segundosVentilacaoMinimaActualState = 0;
                    estadoAtualVentiacaoMinima = 0;
                }
                else
                {
                    if (ventiladorState == 0)
                    {
                        VentiladorOnOf(1);
                    }
                }
            }
            if (estadoAtualVentiacaoMinima == 0)
            {
                segundosVentilacaoMinimaActualState++;

                if (segundosVentilacaoMinimaActualState >= VentilacaoMinimaTimeOff)
                {
                    segundosVentilacaoMinimaActualState = 0;
                    estadoAtualVentiacaoMinima = 1;
                }
                else
                {
                    if (ventiladorState == 1)
                    {
                        VentiladorOnOf(0);
                    }
                }
            }
            else
            {
                //if (estadoAtualVentiacaoMinima == 1)
                {
                    //   VentiladorOnOf(0);
                }
            }
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}