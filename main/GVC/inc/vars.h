
// vars for general

char SNdateTime[100];
char SNuserName[100];
char URLdateTime[100];
char URLuserName[100];
char SSdateTime[100];
char SSuserName[100];
char PWdateTime[100];
char PWuserName[100];

char SS1dateTime[100];
char SS1userName[100];

char PW1dateTime[100];
char PW1userName[100];

char SS2dateTime[100];
char SS2userName[100];

char PW2dateTime[100];
char PW2userName[100];

char SIPdateTime[100];
char SIPuserName[100];
char MIPdateTime[100];
char MIPuserName[100];
char SerialNumber[100] = "GVC/WS/0000";
char ERASEdateTime[100];
char ERASEuserName[100];
char FWVersion[100] = "FW GVC 09JAN25";
char FOTA_URL[200] = "http://gvcsystems.com/WS/firmware.bin";
char WIFI_SSID_1[64];
char WIFI_PASS_1[64];
char WIFI_SSID_2[64];
char WIFI_PASS_2[64];
char WIFI_SSID_3[64];
char WIFI_PASS_3[64];
int WiFiNumber;
int RSSI;
int SipNumber;
int MipNumber;
char ErasedSerialNumber[100];
int sp_port;
char server_ip_addr[100];
char mqtt_uri[100];
char mqtt_user[100];
char mqtt_pass[100];


// kwikpay
// edges , AckPulseReceived, TID, LastTID, pin, pulses, tx_event_pending, Totals[]
int edges = 0; // number of edges (pulses * 2) for generating pulses on output pin
int AckPulseReceived = 0;
char LastTID[100];
char TID[100];
int pin = 0; // output pin for Generating Pulses
int pulses = 0; // number of pulses for generating pulses on output pin
int Totals[7];
int CashTotals[7];


    //  INHUserName:INHdateTime:INHOutputValue 
char INHuserName[100];
char INHdateTime[100];
int INHOutputValue;

char PTuserName[100];
char PTdateTime[100];   
char PassThruValue[2];


char CAuserName[100];
char CAdateTime[100];   
int pulseWidth = 0;
int SignalPolarity = 0;

char CCdateTime[100];
char CCuserName[100];

char RSTdateTime[100];
char RSTuserName[100];


// vars for AAC
// extern int track_id,Total_Tracks,CurrentTrack;  


// vars for traffic

// vars for WAM

