
// vars for general

// vars for kwikpay 
extern char SNdateTime[100];
extern char SNuserName[100];
extern char URLdateTime[100];
extern char URLuserName[100];
extern char SSdateTime[100];
extern char SSuserName[100];
extern char SIPdateTime[100];
extern char SIPuserName[100];
extern char MIPdateTime[100];
extern char MIPuserName[100];
extern char SerialNumber[100];
extern char ERASEdateTime[100];
extern char ERASEuserName[100];
extern char FWVersion[100];
extern char FOTA_URL[200];
extern char WIFI_SSID_1[100];
extern char WIFI_SSID_2[100];
extern char WIFI_SSID_3[100];
extern int WiFiNumber;
extern int RSSI;
extern int SipNumber;
extern int MipNumber;
extern char ErasedSerialNumber[100];
extern int sp_port;
extern char server_ip_addr[100];
extern char mqtt_uri[100];
extern char mqtt_user[100];
extern char mqtt_pass[100];

// vars for kwikpay
// kwikpay
// edges , AckPulseReceived, TID, LastTID, pin, pulses, tx_event_pending, Totals[]
extern int edges; // number of edges (pulses * 2) for generating pulses on output pin
extern int AckPulseReceived;
extern char LastTID[100];
extern char TID[100];
extern int pin; // output pin for Generating Pulses
extern int pulses; // number of pulses for generating pulses on output pin
extern int Totals[7];
extern int CashTotals[7];

extern char INHuserName[100];
extern char INHdateTime[100];
extern int INHOutputValue;

extern char PTuserName[100];
extern char PTdateTime[100];   
extern char PassThruValue[2];

extern char CAuserName[100];
extern char CAdateTime[100];

extern char CCdateTime[100];
extern char CCuserName[100]; 
extern int pulseWidth;
extern int SignalPolarity;


// vars for AAC
extern int track_id,Total_Tracks,CurrentTrack;  


// vars for traffic

// vars for WAM

