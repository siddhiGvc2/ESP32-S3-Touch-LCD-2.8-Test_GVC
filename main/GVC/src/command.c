#include "defs.h"

static const char *TAG = "COMMAND";





void SendReply(char* InputVia, char* data)
{
   sendData(data);
}




void AnalyseGeneralCommands (char* InputVia,char* pkt) {
    char payload[800];
    int track_id;
     if(strncmp(pkt, "*SN?#", 5) == 0){
        sprintf(payload, "*SN,%s,%s,%s#",SNuserName,SNdateTime,SerialNumber); //actual when in production
        ESP_LOGI(TAG,"%s",payload);
        SendReply(InputVia,payload);
    }
    else if(strncmp(pkt, "*URL?#", 6) == 0){
        sprintf(payload,"*URL,%s,%s,%s#",URLuserName,URLdateTime,FOTA_URL);
        ESP_LOGI(TAG,"%s",payload);
        SendReply(InputVia,payload);
    }
    else if(strncmp(pkt,"*RSSI?#",7)==0)
    {
    sprintf(payload,"*RSSI,%d",RSSI);
    ESP_LOGI(TAG,"%s",payload);
    SendReply(InputVia,payload);
    }
    else if(strncmp(pkt, "*SSID?#", 7) == 0){
        sprintf(payload, "*SSID,%s,%s,%d,%s,%s,%s#",SSuserName,SSdateTime,WiFiNumber,WIFI_SSID_1,WIFI_SSID_2,WIFI_SSID_3); 
        ESP_LOGI(TAG,"%s",payload);
        SendReply(InputVia,payload);
       
    }
     else if(strncmp(pkt, "*FW?#", 5) == 0){
        ESP_LOGI(TAG,"*%s#",FWVersion);
        sprintf(payload, "*FW:%s#",FWVersion);
        SendReply(InputVia,payload);
     }
     else if(strncmp(pkt, "*SIP?#", 6) == 0){
        sprintf(payload,"*SIP,%s,%s,%s,%d#",SIPuserName,SIPdateTime,server_ip_addr,
                                         sp_port );
        SendReply(InputVia,payload);
       
    }
      else if(strncmp(pkt, "*MIP?#", 6) == 0){
        sprintf(payload,"*MIP,%s,%s,%s,%d#",MIPuserName,MIPdateTime,mqtt_uri,MipNumber);
        SendReply(InputVia,payload);
       
    }
    else if (strncmp(pkt, "*ERASE?", 7) == 0){
        sprintf(payload,"*ERASE,%s,%s,%s#",ERASEuserName,ERASEdateTime,ErasedSerialNumber); 
        SendReply(InputVia, payload);
    }
     else if(strncmp(rx_buffer, "*RST", 4) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            sscanf(rx_buffer, "*RST:%[^:]:%[^#]#",RSTuserName,RSTdateTime);
        }
        else if(strcmp(InputVia,"UART")==0)
        {
            strcpy(RSTuserName,"LOCAL");
            strcpy(RSTdateTime,"00/00/00");
        }
       
       
        ESP_LOGI(InputVia, "**************Restarting after 3 second*******");
        SaveString(NVS_RST_USERNAME, RSTuserName);
        SaveString(NVS_RST_DATETIME, RSTdateTime);
        sprintf(payload, "*RST-OK,%s,%s#",RSTuserName,RSTdateTime);
        SendReply(payload,InputVia);
        ESP_LOGI(InputVia, "*RST-OK#");
        SendReply(InputVia,"*Resetting device#");
        RestartDevice();

    }
    else if(strncmp(rx_buffer, "*SN:", 4) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            if (strstr(SerialNumber,"999999"))
            {
                sscanf(rx_buffer, "*SN:%[^:]:%[^:]:%[^#]#",SNuserName,SNdateTime,SerialNumber);
                SaveString(NVS_SERIAL_NUMBER, SerialNumber);
                SaveString(NVS_SN_USERNAME, SNuserName);
                SaveString(NVS_SN_DATETIME, SNdateTime);
                sendSocketData(sock, "*SN-OK#", strlen("*SN-OK#"), 0);
            }
            else
            {
               sendSocketData(sock, "*SN CAN NOT BE SET#", strlen("*SN CAN NOT BE SET#"), 0);

            }
                // tx_event_pending = 1; 
        }
        else if(strcmp(InputVia,"UART")==0)
        {
            sscanf(rx_buffer, "*SN:%[^#]#",SerialNumber);
            strcpy(SNuserName, "LOCAL");
            strcpy(SNdateTime, "00/00/00");
            
            SaveString(NVS_SERIAL_NUMBER, SerialNumber);
            SaveString(NVS_SN_USERNAME,SNuserName);
            SaveString(NVS_SN_DATETIME,SNdateTime);
            
        
            SendReply("*SN-OK#",InputVia);
            // tx_event_pending = 1;
        }
      
  
    }
    else if(strncmp(rx_buffer, "*URL:", 5) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64];

            // Parse the input buffer
            if (sscanf(rx_buffer, "*URL:%[^:]:%[^:]:%[^#]#", tempUserName, tempDateTime, tempBuf) == 3) {
                // Check if any of the parsed values are empty
                if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "*Error: Missing or invalid parameters#";
                    SendReply(errorMsg,InputVia);
                } else {
                        strcpy(URLuserName, tempUserName);
                        strcpy(URLdateTime, tempDateTime);
                        strcpy(FOTA_URL, tempBuf);
                }
           
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "Error: Invalid format";
                SendReply(errorMsg,InputVia);
            }
        }
        else if(strcmp(InputVia, "UART") == 0)
        {
            sscanf(rx_buffer, "*URL:%[^#]#", FOTA_URL);
            strcpy(URLuserName,"LOCAL");
            strcpy(URLdateTime,"00/00/00");
        }
        
        SaveString(NVS_OTA_URL_KEY, FOTA_URL);
        sprintf(payload, "*URL-OK,%s,%s#",URLuserName,URLdateTime);
        SaveString(NVS_URL_USERNAME, URLuserName);
        SaveString(NVS_URL_DATETIME, URLdateTime);
        SendReply(payload,InputVia);
        // tx_event_pending = 1;

    }
     else if(strncmp(rx_buffer, "*MIP:", 5) == 0){

        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64] ,tempBuf2[64];

            if (sscanf(rx_buffer, "*MIP:%[^:]:%[^:]:%[^:]#", tempUserName, tempDateTime, tempBuf) == 3) { 
                  if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0  ) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "*Error: Missing or invalid parameters#";
                    SendReply(errorMsg,InputVia);
                }
                else{

                        strcpy(MIPuserName, tempUserName);
                        strcpy(MIPdateTime, tempDateTime);
                        MipNumber=atoi(tempBuf);
                }
           
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "*Error: Invalid format#";
                SendReply(errorMsg,InputVia);
            }
        }
        else if(strcmp(InputVia, "UART") == 0)
        {
            sscanf(rx_buffer, "*MIP:%d#",&MipNumber);
            strcpy(MIPuserName,"LOCAL");
            strcpy(MIPdateTime,"00/00/00");
        }
      

        if ((MipNumber == 0) || (MipNumber >MAXMIPNUMBER))  
        {  
            sprintf(payload, "*MIP-Error#");
            ESP_LOGI(InputVia,"*MIP-ERROR#");
        }else 
        {
            sprintf(payload, "*MIP-OK,%s,%s#",MIPuserName,MIPdateTime);                                                   
            SaveInteger(NVS_MIP_NUMBER,  MipNumber);
            SaveString(NVS_MIP_USERNAME, MIPuserName);
            SaveString(NVS_MIP_DATETIME, MIPdateTime);
            ESP_LOGI(InputVia,"*MIP-OK,%s,%s#",MIPuserName,MIPdateTime);
        }    
        SendReply(payload,InputVia);
        SaveString(payload);
       // tx_event_pending = 1;

    }
      else if(strncmp(rx_buffer, "*SIP:", 5) == 0){

        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64] ,tempBuf2[64];

            if (sscanf(rx_buffer, "*SIP:%[^:]:%[^:]:%[^:]#", tempUserName, tempDateTime, tempBuf) == 3) { 
                  if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0  ) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "*Error: Missing or invalid parameters#";
                    SendReply(errorMsg,InputVia);
                }
                else{
                        strcpy(SIPuserName, tempUserName);
                        strcpy(SIPdateTime, tempDateTime);
                        SipNumber=atoi(tempBuf);
                }
           
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "*Error: Invalid format#";
                SendReply(errorMsg,InputVia);
            }
        }
        else if(strcmp(InputVia, "UART") == 0)
        {
            sscanf(rx_buffer, "*SIP:%d#",&SipNumber);
            strcpy(SIPuserName,"LOCAL");
            strcpy(SIPdateTime,"00/00/00");
        }
      

        if ((SipNumber == 0) || (SipNumber >MAXSIPNUMBER))  
        {  
            sprintf(payload, "*SIP-Error#");
            ESP_LOGI(InputVia,"*SIP-ERROR#");
        }else 
        {
            sprintf(payload, "*SIP-OK,%s,%s#",SIPuserName,SIPdateTime);                                                   
            SaveInteger(NVS_SIP_NUMBER,  SipNumber);
            SaveString(NVS_SIP_USERNAME, SIPuserName);
            SaveString(NVS_SIP_DATETIME, SIPdateTime);
            ESP_LOGI(InputVia,"*SIP-OK,%s,%s#",SIPuserName,SIPdateTime);
        }    
        SendReply(payload,InputVia);
      
       // tx_event_pending = 1;

    }
}

void AnalyseCommands (char* InputVia,char* pkt) {
    AnalyseGeneralCommands(InputVia,pkt);
    AnalyseKwikpayCommands(InputVia,pkt);
    AnalyseAACCommands(InputVia,pkt);
    AnalyseVendingCommands(InputVia,pkt);
    AnalyseWAMCommands(InputVia,pkt);
}



