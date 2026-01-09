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

void SetINHLow(void)
{

}
void SetINHHigh(void)
{

}

void SaveInteger(const char* key, int value)
{

}

void SaveString(const char* key, char* value)
{

}


void SendReply(char* InputVia, char* data)
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
                SendReply(InputVia, payload);
                vTaskDelay(1000/portTICK_PERIOD_MS);
                sprintf(payload, "*T-OK,%s,%d,%d#",TID,pin,pulses); //actual when in production
                ESP_LOGI(InputVia, "*T-OK,%s,%d,%d#",TID,pin,pulses);
                SendReply(InputVia, payload);
                Totals[pin-1] += pulses;
                strcpy(LastTID,TID);
            }
            else
            {
              ESP_LOGI(InputVia,"Duplicate TID");
              SendReply(InputVia, "*DUP TID#");
              
            }  
        }
    }

    else if(strncmp(rx_buffer, "*INH:", 5) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            sscanf(rx_buffer, "*INH:%[^:]:%[^:]:%d#",INHuserName,INHdateTime, &INHOutputValue);
          
        }
        else if(strcmp(InputVia, "UART") == 0)
        {
            sscanf(rx_buffer, "*INH:%d#",&INHOutputValue);
            strcpy(INHuserName,"LOCAL");
            strcpy(INHdateTime,"00/00/00");
        }

        if (INHOutputValue != 0)
        {
            INHOutputValue = 1;
            SetINHLow();
        }
        else
        {
              SetINHHigh();
        }
        ESP_LOGI (InputVia, "Set INH Output as %d",INHOutputValue);
        sprintf(payload, "*INH-DONE,%s,%s,%d#",INHuserName,INHdateTime,INHOutputValue);
        // utils_nvs_set_str(NVS_INH_USERNAME, INHuserName);
        SaveString(NVS_INH_USERNAME, INHuserName);
        SaveString(NVS_INH_DATETIME, INHdateTime);
        SaveInteger(NVS_INH_KEY, INHOutputValue);
        SendReply(InputVia, payload);
     }  

     // PTUserName, PTdateTime, PassThruValue
        else if(strncmp(rx_buffer, "*PT:", 4) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64] ;
            if (sscanf(rx_buffer, "*PT:%[^:]:%[^:]:%[^:#]#", tempUserName, tempDateTime, tempBuf) == 3) {
            // Check if any of the parsed values are empty
            if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0 ) {
                // send error message if any required parameters are missing or invalid
                const char* errorMsg = "*Error: Missing or invalid parameters#";
                SendReply(InputVia,errorMsg);
            }
            else{
        
            strcpy(PTuserName, tempUserName);
            strcpy(PTdateTime, tempDateTime);
            strcpy(PassThruValue, tempBuf);
             }
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "*Error: Invalid format#";
              SendReply(InputVia,errorMsg);
            }
        }
        else if(strcmp(InputVia,"UART") == 0)
        {
            sscanf(rx_buffer, "*PT:%[^#]#",PassThruValue);
            strcpy(PTuserName, "LOCAL");
            strcpy(PTdateTime, "00/00/00");
        }
    
        if (strstr(PassThruValue, "Y") == NULL && strstr(PassThruValue, "N") == NULL) {
            strcpy(PassThruValue, "Y");
        }

        ESP_LOGI (InputVia, "Pass Thru %s",PassThruValue);
        sprintf(payload, "*PT-OK,%s,%s,%s#",PTuserName,PTdateTime,PassThruValue);
        SaveString(NVS_PT_USERNAME, PTuserName);
        SaveString(NVS_PT_DATETIME, PTdateTime);
        SendReply(InputVia,payload);
        SaveString(NVS_PASS_THRU, PassThruValue);
     }

     // CAuserName, CAdateTime, numValue, polarity
     // pulseWidth, SignalPolarity

        else if(strncmp(rx_buffer, "*CA:", 4) == 0){
        int numValue = 0;
        int polarity = 0;
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64],tempBuf2[64];
            if (sscanf(rx_buffer, "*CA:%[^:]:%[^:]:%[^:]:%[^:#]#", tempUserName, tempDateTime, tempBuf,tempBuf2) == 4) {
                // Check if any of the parsed values are empty
                if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0 || strlen(tempBuf2) == 0) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "*Error: Missing or invalid parameters#";
                    SendReply(InputVia,errorMsg);
                }
                else{
                    strcpy(CAuserName, tempUserName);
                    strcpy(CAdateTime, tempDateTime);
                    numValue = atoi(tempBuf);
                    polarity = atoi(tempBuf2);
                }
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "*Error: Invalid format#";
                SendReply(InputVia,errorMsg);
            }
        }
        else if(strcmp(InputVia, "UART") == 0)
        {
            sscanf(rx_buffer, "*CA:%d:%d#",&numValue,&polarity);
            strcpy(CAuserName,"LOCAL");
            strcpy(CAdateTime,"00/00/00"); 
        }
       
      
     
       ESP_LOGI(InputVia, "Generate @ numValue %d polarity %d",numValue,polarity);
       sprintf(payload, "*CA-OK,%s,%s,%d,%d#",CAuserName,CAdateTime,numValue,polarity);
       SaveString(NVS_CA_USERNAME, CAuserName);
       SaveString(NVS_CA_DATETIME, CAdateTime);
       ESP_LOGI(InputVia,"CA Values Saved %s,%s",CAuserName,CAdateTime);
       SendReply(InputVia,payload);
       // valid values are between 25 and 100
       if (numValue<10)
           numValue = 25;
       if (numValue>100)
           numValue = 100;
       // possible values are 0 and 1        
       if (polarity>0)
           polarity = 1;   
       polarity = 0;     
       pulseWidth=numValue;
       SignalPolarity=polarity;
//       Out4094(0x00);
       SaveInteger(NVS_CA_KEY, numValue*2+polarity);
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
        SendReply(InputVia, "*PAUSE-OK#");
    }
    else if(strncmp(pkt, "*RESUME#", 8) == 0){
        Music_resume();
        SendReply(InputVia, "*RESUME-OK#");
    }
    else if(strncmp(pkt, "*NEXT#", 6) == 0){
        
        SendReply(InputVia, "*NEXT-OK#");
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
        SendReply(InputVia, "*PREV-OK#");
    }
    else if(strncmp(pkt, "*VOLUME:", 8) == 0){
        int vol;
        sscanf(pkt, "*VOLUME:%d#", &vol);
        if(vol <= Volume_MAX){
            Volume = vol;
            sprintf(payload,"*VOLUME-OK,%d#",vol);
            SendReply(InputVia, payload);
        }
        else{
            SendReply(InputVia, "*VOLUME-ERR#");
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


