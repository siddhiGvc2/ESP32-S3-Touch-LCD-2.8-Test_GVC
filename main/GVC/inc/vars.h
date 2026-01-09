#include "defs.h"

// vars for general


// vars for kwikpay 
char SNdateTime[100];
char SNuserName[100];
char URLdateTime[100];
char URLuserName[100];
char SSdateTime[100];
char SSuserName[100];
char SIPdateTime[100];
char SIPuserName[100];
char MIPdateTime[100];
char MIPuserName[100];
char SerialNumber[100];
char ERASEdateTime[100];
char ERASEuserName[100];
char FWVersion[100];
char FOTA_URL[200];
char WIFI_SSID_1[100];
char WIFI_SSID_2[100];
char WIFI_SSID_3[100];
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
 

// vars for AAC
extern int track_id,Total_Tracks,CurrentTrack;  


// vars for traffic

// vars for WAM

