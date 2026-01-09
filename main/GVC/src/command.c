#include "ST7789.h"
#include "PCF85063.h"
#include "QMI8658.h"
#include "SD_MMC.h"
#include "Wireless.h"
#include "LVGL_Example.h"
#include "BAT_Driver.h"
#include "PWR_Key.h"
#include "PCM5101.h"
#include "LVGL_Music.h"
#include "driver/uart.h"
#include "esp_log.h"
#include "externVars.h"
#include "calls.h"
#include "vars.h"

static const char *TAG = "COMMAND";


void SendReply(char* data)
{
   sendData(data);
}

void AnalyseKwikpayCommands (char* InputVia, char* rx_buffer)
{
    char payload[600];
     if(strncmp(rx_buffer, "*V:", 3) == 0){
        if (edges == 0) 
        {
            AckPulseReceived = 0;
            sscanf(rx_buffer, "*V:%[^:]:%d:%d#",TID,&pin,&pulses);
    
            if (memcmp(TID, LastTID, 100) != 0)
            {
                edges = pulses*2;  // doubled edges
                ESP_LOGI(InputVia, "*V-OK,%s,%d,%d#",TID,pin,pulses);
                sprintf(payload, "*V-OK,%s,%d,%d#", TID,pin,pulses); //actual when in production
                SendReply(payload);
                vTaskDelay(1000/portTICK_PERIOD_MS);
                sprintf(payload, "*T-OK,%s,%d,%d#",TID,pin,pulses); //actual when in production
                ESP_LOGI(InputVia, "*T-OK,%s,%d,%d#",TID,pin,pulses);
                SendReply(payload);
                Totals[pin-1] += pulses;
                strcpy(LastTID,TID);
            }
            else
            {
              ESP_LOGI(InputVia,"Duplicate TID");
              SendReply("*DUP TID#");
              
            }  

        }
    }
}


void AnalyseAACCommands (char* InputVia,char* pkt) {
    char payload[800];
    int track_id;
     if(strncmp(pkt, "*PLAY:", 6) == 0){
       
        sscanf(pkt, "*PLAY:%d#", &track_id);
        CurrentTrack = track_id;
        PlayCurrentTrack();
    }
    else if(strncmp(pkt, "*PAUSE#", 7) == 0){
        Music_pause();
        SendReply("*PAUSE-OK#");
    }
    else if(strncmp(pkt, "*RESUME#", 8) == 0){
        Music_resume();
        SendReply("*RESUME-OK#");
    }
    else if(strncmp(pkt, "*NEXT#", 6) == 0){
        
        SendReply("*NEXT-OK#");
        CurrentTrack++;

        if(CurrentTrack >= Total_Tracks){
            CurrentTrack = 0;
        }
        PlayCurrentTrack();
    }
    else if(strncmp(pkt, "*PREV#", 6) == 0){
        
        if(CurrentTrack == 0){
            CurrentTrack = Total_Tracks-1;
        }
        else{
            CurrentTrack--;
        }
        PlayCurrentTrack();    
        SendReply("*PREV-OK#");
    }
    else if(strncmp(pkt, "*VOLUME:", 8) == 0){
        int vol;
        sscanf(pkt, "*VOLUME:%d#", &vol);
        if(vol <= Volume_MAX){
            Volume = vol;
            sprintf(payload,"*VOLUME-OK,%d#",vol);
            SendReply(payload);
        }
        else{
            SendReply("*VOLUME-ERR#");
        }
    }
   

}

void AnalyseGeneralCommands (char* InputVia,char* pkt) {
    char payload[800];
    int track_id;
     if(strncmp(pkt, "*SN?#", 5) == 0){
        sprintf(payload, "*SN,%s,%s,%s#",SNuserName,SNdateTime,SerialNumber); //actual when in production
        sendData(payload);
    }
    else if(strncmp(pkt, "*URL?#", 6) == 0){
        sprintf(payload,"*URL,%s,%s,%s#",URLuserName,URLdateTime,FOTA_URL);
        sendData(payload);
    }
    else if(strncmp(pkt,"*RSSI?#",7)==0)
    {
    sprintf(payload,"*RSSI,%d",RSSI);
    sendData(payload);
    }
    else if(strncmp(pkt, "*SSID?#", 7) == 0){
        sprintf(payload, "*SSID,%s,%s,%d,%s,%s,%s#",SSuserName,SSdateTime,WiFiNumber,WIFI_SSID_1,WIFI_SSID_2,WIFI_SSID_3); 
        sendData(payload);
       
    }
     else if(strncmp(pkt, "*FW?#", 5) == 0){
        ESP_LOGI(TAG,"*%s#",FWVersion);
        sendData(FWVersion);
     }
     else if(strncmp(pkt, "*SIP?#", 6) == 0){
        sprintf(payload,"*SIP,%s,%s,%s,%d#",SIPuserName,SIPdateTime,server_ip_addr,
                                         sp_port );
        sendData(payload);
       
    }
      else if(strncmp(pkt, "*MIP?#", 6) == 0){
        sprintf(payload,"*MIP,%s,%s,%s,%d#",MIPuserName,MIPdateTime,mqtt_uri,MipNumber);
        sendData(payload);
       
    }
    else if (strncmp(pkt, "*ERASE?", 7) == 0){
        sprintf(payload,"*ERASE,%s,%s,%s#",ERASEuserName,ERASEdateTime,ErasedSerialNumber); 
        sendData(payload);
    }
}

void AnalyseVendingCommands (char* InputVia,char* pkt) {
    char payload[200];
    int track_id;
}

void AnalyseWAMCommands (char* InputVia,char* pkt) {
    char payload[200];
    int track_id;
}


