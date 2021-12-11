#include <string>

void CreateRoutineTasks();
void UpdateClima(void *parameter);
void CheckNetworkConnection(void *parameter);
void UpdateRSSI(void *parameter);
void sendEvent(const char* data, const char* name);
char *itob(int val, char *buf, size_t n);
const char* outputState(int gpio);
void VentiladorOnOf(int state);
void NewDHT11Info(float temp, float hum);
void definePins();
std::string ReturnConstString(const float &var);
std::string returnLocalTime();
void StartNetWorkConnection();
void StartSensores();
void setupWebServer();
void DoVentilacaoMinima(void *parameter);
void definePins();
void TransmitInfoToWebServerClients();
void setupTelegramBot();
void VerificarMensagensTelegram(void *parameter);

void configReset();
bool configRead(void);
bool configSave(void);

extern const int ventiladorPin;
extern float temperatura;
extern float umidade;
extern float RSSI;

extern int VentilacaoMinimaEnabled;

extern int VentilacaoMinimaTimeOn;
extern int VentilacaoMinimaTimeOff;
extern int segundosVentilacaoMinimaActualState;
extern int estadoAtualVentiacaoMinima;
extern int ventiladorState;