#include "app.h"

static const char *TAG = "COMMAND";





void SendReply(char* InputVia, char* data)
{
   sendData(data);
}




void AnalyseGeneralCommands (char* InputVia,char* rx_buffer) {
    char payload[800];
    int track_id;
     if(strncmp(rx_buffer, "*SN?#", 5) == 0){
        sprintf(payload, "*SN,%s,%s,%s#",SNuserName,SNdateTime,SerialNumber); //actual when in production
        ESP_LOGI(TAG,"%s",payload);
        SendReply(InputVia,payload);
    }
    else if(strncmp(rx_buffer, "*URL?#", 6) == 0){
        sprintf(payload,"*URL,%s,%s,%s#",URLuserName,URLdateTime,FOTA_URL);
        ESP_LOGI(TAG,"%s",payload);
        SendReply(InputVia,payload);
    }
    else if(strncmp(rx_buffer,"*RSSI?#",7)==0)
    {
    sprintf(payload,"*RSSI,%d",RSSI);
    ESP_LOGI(TAG,"%s",payload);
    SendReply(InputVia,payload);
    }
    else if(strncmp(rx_buffer, "*SSID?#", 7) == 0){
        sprintf(payload, "*SSID,%s,%s,%d,%s,%s,%s#",SSuserName,SSdateTime,WiFiNumber,WIFI_SSID_1,WIFI_SSID_2,WIFI_SSID_3); 
        ESP_LOGI(TAG,"%s",payload);
        SendReply(InputVia,payload);
       
    }
     else if(strncmp(rx_buffer, "*PW142857?#", 11) == 0){
        sprintf(payload, "*PW142857,%d,%s,%s,%s#",WiFiNumber,WIFI_PASS_1,WIFI_PASS_2,WIFI_PASS_3); 
        ESP_LOGI(TAG,"%s",payload);
        SendReply(InputVia,payload);
       
    }
     else if(strncmp(rx_buffer, "*FW?#", 5) == 0){
        ESP_LOGI(TAG,"*%s#",FWVersion);
        sprintf(payload, "*FW:%s#",FWVersion);
        SendReply(InputVia,payload);
     }
     else if(strncmp(rx_buffer, "*SIP?#", 6) == 0){
        sprintf(payload,"*SIP,%s,%s,%s,%d#",SIPuserName,SIPdateTime,server_ip_addr,
                                         sp_port );
        SendReply(InputVia,payload);
       
    }
    else if(strncmp(rx_buffer,"*TCP1:",6)==0)
    {
        sscanf(rx_buffer,"*TCP1:%s:%d#",TCP1_ip_addr,&TCP1_server_port);
        SaveString(NVS_TCP1_IP_ADDR, TCP1_ip_addr);
        SaveInteger(NVS_TCP1_SERVER_PORT, TCP1_server_port);
        ESP_LOGI(InputVia,"Set TCP1 IP Address to %s",TCP1_ip_addr);
        sprintf(payload,"*TCP1-OK,%s#",TCP1_ip_addr);
        SendReply(InputVia,payload);
    }
    else if(strncmp(rx_buffer,"*TCP2:",6)==0)
    {
        sscanf(rx_buffer,"*TCP2:%s:%d#",TCP2_ip_addr,&TCP2_server_port);
        SaveString(NVS_TCP2_IP_ADDR , TCP2_ip_addr);
        SaveInteger(NVS_TCP2_SERVER_PORT, TCP2_server_port);
        ESP_LOGI(InputVia,"Set TCP2 IP Address to %s",TCP2_ip_addr);
        sprintf(payload,"*TCP2-OK,%s#",TCP2_ip_addr);
        SendReply(InputVia,payload);
    }
    else if(strncmp(rx_buffer,"*TCP3:",6)==0)
    {
        sscanf(rx_buffer,"*TCP3:%s:%d#",TCP3_ip_addr,&TCP3_server_port);
        SaveString(NVS_TCP3_IP_ADDR, TCP3_ip_addr);
        SaveInteger(NVS_TCP3_SERVER_PORT, TCP3_server_port);
        ESP_LOGI(InputVia,"Set TCP3 IP Address to %s",TCP3_ip_addr);
        sprintf(payload,"*TCP3-OK,%s#",TCP3_ip_addr);
        SendReply(InputVia,payload);
    }
      else if(strncmp(rx_buffer, "*MIP?#", 6) == 0){
        sprintf(payload,"*MIP,%s,%s,%s,%d#",MIPuserName,MIPdateTime,mqtt_uri,MipNumber);
        SendReply(InputVia,payload);
       
    }
    else if(strncmp(rx_buffer,"*MQTT1:",7)==0)
    {
        sscanf(rx_buffer,"*MQTT1:%[^:]:%[^:]:%[^#]#",mqtt_uri1,mqtt_user1,mqtt_pass1);
        SaveString(NVS_MQTT1_URI, mqtt_uri1);
        SaveString(NVS_MQTT1_USER, mqtt_user1);
        SaveString(NVS_MQTT1_PASS, mqtt_pass1);
        ESP_LOGI(InputVia,"Set MQTT1 URI to %s",mqtt_uri1);
        sprintf(payload,"*MQTT1-OK,%s#",mqtt_uri1);
        SendReply(InputVia,payload);
    }
    else if(strncmp(rx_buffer,"*MQTT2:",7)==0)
    {
        sscanf(rx_buffer,"*MQTT2:%[^:]:%[^:]:%[^#]#",mqtt_uri2,mqtt_user2,mqtt_pass2);
        SaveString(NVS_MQTT2_URI, mqtt_uri2);
        SaveString(NVS_MQTT2_USER, mqtt_user2);
        SaveString(NVS_MQTT2_PASS, mqtt_pass2);
        ESP_LOGI(InputVia,"Set MQTT2 URI to %s",mqtt_uri2);
        sprintf(payload,"*MQTT2-OK,%s#",mqtt_uri2);
        SendReply(InputVia,payload);
    }
    else if(strncmp(rx_buffer,"*MQTT3:",7)==0)
    {
        sscanf(rx_buffer,"*MQTT3:%[^:]:%[^:]:%[^#]#",mqtt_uri3,mqtt_user3,mqtt_pass3);
        SaveString(NVS_MQTT3_URI, mqtt_uri3);   
        SaveString(NVS_MQTT3_USER, mqtt_user3);
        SaveString(NVS_MQTT3_PASS, mqtt_pass3);
        ESP_LOGI(InputVia,"Set MQTT3 URI to %s",mqtt_uri3);
        sprintf(payload,"*MQTT3-OK,%s#",mqtt_uri3);
        SendReply(InputVia,payload);
    }
    
    else if (strncmp(rx_buffer, "*ERASE?", 7) == 0){
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
        SendReply(InputVia,payload);
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
                    SendReply(InputVia,errorMsg);
                } else {
                        strcpy(URLuserName, tempUserName);
                        strcpy(URLdateTime, tempDateTime);
                        strcpy(FOTA_URL, tempBuf);
                }
           
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "Error: Invalid format";
                SendReply(InputVia,errorMsg);
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
        SendReply(InputVia,payload);
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
                    SendReply(InputVia,errorMsg);
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
                SendReply(InputVia,errorMsg);
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
        SendReply(InputVia,payload);
        
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
                    SendReply(InputVia,errorMsg);
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
                SendReply(InputVia,errorMsg);
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
        SendReply(InputVia,payload);
      
       // tx_event_pending = 1;

    }
    else if(strncmp(rx_buffer, "*SS:", 4) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64];

            // Parse the input buffer
            if (sscanf(rx_buffer, "*SS:%[^:]:%[^:]:%[^#]#", tempUserName, tempDateTime, tempBuf) == 3) {
                // Check if any of the parsed values are empty
                if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "*Error: Missing or invalid parameters#";
                    SendReply(InputVia,errorMsg);
                } else {
                    // Copy parsed values to the actual variables
                    strcpy(SSuserName, tempUserName);
                    strcpy(SSdateTime, tempDateTime);
                    strcpy(WIFI_SSID_1, tempBuf);
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
            sscanf(rx_buffer, "*SS:%[^#]#",WIFI_SSID_1);
            strcpy(SSuserName,"LOCAL");
            strcpy(SSdateTime,"00/00/00");
        }
    
           // Save the values to non-volatile storage
           SaveString(NVS_SSID_1_KEY, WIFI_SSID_1);
           SaveString(NVS_SS_USERNAME, SSuserName);
           SaveString(NVS_SS_DATETIME, SSdateTime);

           // Format the success message and send it
           sprintf(payload, "*SS-OK,%s,%s#", SSuserName, SSdateTime);
           SendReply(InputVia,payload);
          // tx_event_pending = 1;

    }
    else if(strncmp(rx_buffer, "*SS1:", 5) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64];

            // Parse the input buffer
            if (sscanf(rx_buffer, "*SS1:%[^:]:%[^:]:%[^#]#", tempUserName, tempDateTime, tempBuf) == 3) {
                // Check if any of the parsed values are empty
                if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "*Error: Missing or invalid parameters#";
                    SendReply(InputVia,errorMsg);
                } else {
                    // Copy parsed values to the actual variables
                    strcpy(SS1userName, tempUserName);
                    strcpy(SS1dateTime, tempDateTime);
                    strcpy(WIFI_SSID_2, tempBuf);
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
            sscanf(rx_buffer, "*SS1:%[^#]#",WIFI_SSID_2);
            strcpy(SS1userName,"LOCAL");
            strcpy(SS1dateTime,"00/00/00");
        }
       

           // Save the values to non-volatile storage
           SaveString(NVS_SSID_2_KEY, WIFI_SSID_2);
           SaveString(NVS_SS1_USERNAME, SS1userName);
           SaveString(NVS_SS1_DATETIME, SS1dateTime);

           // Format the success message and send it
           sprintf(payload, "*SS1-OK,%s,%s#", SS1userName, SS1dateTime);
           SendReply(InputVia,payload);
          // tx_event_pending = 1;

    }
    else if(strncmp(rx_buffer, "*SS2:", 5) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64];

            // Parse the input buffer
            if (sscanf(rx_buffer, "*SS2:%[^:]:%[^:]:%[^#]#", tempUserName, tempDateTime, tempBuf) == 3) {
                // Check if any of the parsed values are empty
                if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "*Error: Missing or invalid parameters#";
                    SendReply(InputVia,errorMsg);
                } else {
                    // Copy parsed values to the actual variables
                    strcpy(SS2userName, tempUserName);
                    strcpy(SS2dateTime, tempDateTime);
                    strcpy(WIFI_SSID_3, tempBuf);
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
            sscanf(rx_buffer, "*SS2:%[^#]#",WIFI_SSID_3);
            strcpy(SS2userName,"LOCAL");
            strcpy(SS2dateTime,"00/00/00");
        }
       

           // Save the values to non-volatile storage
           SaveString(NVS_SSID_3_KEY, WIFI_SSID_3);
           SaveString(NVS_SS2_USERNAME, SS2userName);
           SaveString(NVS_SS2_DATETIME, SS2dateTime);

           // Format the success message and send it
           sprintf(payload, "*SS2-OK,%s,%s#", SS2userName, SS2dateTime);
           SendReply(InputVia,payload);
          // tx_event_pending = 1;

    }
    else if(strncmp(rx_buffer, "*PW:", 4) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64];

            // Parse the input buffer
            if (sscanf(rx_buffer, "*PW:%[^:]:%[^:]:%[^#]#", tempUserName, tempDateTime, tempBuf) == 3) {
                // Check if any of the parsed values are empty
                if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "Error: Missing or invalid parameters";
                    SendReply(InputVia,errorMsg);
                } else {
                    // Copy parsed values to the actual variables
                    strcpy(PWuserName, tempUserName);
                    strcpy(PWdateTime, tempDateTime);
                    strcpy(WIFI_PASS_1, tempBuf);
                }
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "Error: Invalid format";
                SendReply(InputVia,errorMsg);
            }
        }
        else if(strcmp(InputVia, "UART") == 0)
        {
            sscanf(rx_buffer, "*PW:%[^#]#",WIFI_PASS_1);
            strcpy(PWuserName,"LOCAL");
            strcpy(PWdateTime,"00/00/00");
        }
      

           // Save the values to non-volatile storage
           SaveString(NVS_PASS_1_KEY, WIFI_PASS_1);
           SaveString(NVS_PW_USERNAME, PWuserName);
           SaveString(NVS_PW_DATETIME, PWdateTime);

           // Format the success message and send it
           sprintf(payload, "*PW-OK,%s,%s#", PWuserName, PWdateTime);
           SendReply(InputVia,payload);
          // tx_event_pending = 1;

    }
    else if(strncmp(rx_buffer, "*PW1:", 5) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64];

            // Parse the input buffer
            if (sscanf(rx_buffer, "*PW1:%[^:]:%[^:]:%[^#]#", tempUserName, tempDateTime, tempBuf) == 3) {
                // Check if any of the parsed values are empty
                if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "*Error: Missing or invalid parameters#";
                    SendReply(InputVia,errorMsg);
                } else {
                    // Copy parsed values to the actual variables
                    strcpy(PW1userName, tempUserName);
                    strcpy(PW1dateTime, tempDateTime);
                    strcpy(WIFI_PASS_2, tempBuf);
                }
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "Error: Invalid format";
                SendReply(InputVia,errorMsg);
            }
        }
        else if(strcmp(InputVia, "UART") == 0)
        {
            sscanf(rx_buffer, "*PW1:%[^#]#",WIFI_PASS_2);
            strcpy(PW1userName,"LOCAL");
            strcpy(PW1dateTime,"00/00/00");
        }
        
           // Save the values to non-volatile storage
           SaveString(NVS_PASS_2_KEY, WIFI_PASS_2);
           SaveString(NVS_PW1_USERNAME, PW1userName);
           SaveString(NVS_PW1_DATETIME, PW1dateTime);

           // Format the success message and send it
           sprintf(payload, "*PW1-OK,%s,%s#", PW1userName, PW1dateTime);
           SendReply(InputVia,payload);
          // tx_event_pending = 1;

    }
    else if(strncmp(rx_buffer, "*PW2:", 5) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            char tempUserName[64], tempDateTime[64], tempBuf[64];

            // Parse the input buffer
            if (sscanf(rx_buffer, "*PW2:%[^:]:%[^:]:%[^#]#", tempUserName, tempDateTime, tempBuf) == 3) {
                // Check if any of the parsed values are empty
                if (strlen(tempUserName) == 0 || strlen(tempDateTime) == 0 || strlen(tempBuf) == 0) {
                    // Send error message if any required parameters are missing or invalid
                    const char* errorMsg = "Error: Missing or invalid parameters";
                    SendReply(InputVia,errorMsg);
                } else {
                    // Copy parsed values to the actual variables
                    strcpy(PW2userName, tempUserName);
                    strcpy(PW2dateTime, tempDateTime);
                    strcpy(WIFI_PASS_3, tempBuf);
                }
            }
            else {
                // Send error message if parsing failed
                const char* errorMsg = "Error: Invalid format";
                SendReply(InputVia,errorMsg);
            }
        }
        else if(strcmp(InputVia, "UART") == 0)
        {
            sscanf(rx_buffer, "*PW2:%[^#]#",WIFI_PASS_3);
            strcpy(PW2userName,"LOCAL");
            strcpy(PW2dateTime,"00/00/00");
        }
    

           // Save the values to non-volatile storage
           SaveString(NVS_PASS_3_KEY, WIFI_PASS_3);
           SaveString(NVS_PW2_USERNAME, PW2userName);
           SaveString(NVS_PW2_DATETIME, PW2dateTime);

           // Format the success message and send it
           sprintf(payload, "*PW2-OK,%s,%s#", PW2userName, PW2dateTime);
           SendReply(InputVia,payload);
           //tx_event_pending = 1;

    }
}

void AnalyseCommands (char* InputVia,char* pkt) {
    AnalyseGeneralCommands(InputVia,pkt);
    AnalyseKwikpayCommands(InputVia,pkt);
    AnalyseAACCommands(InputVia,pkt);
    AnalyseVendingCommands(InputVia,pkt);
    AnalyseWAMCommands(InputVia,pkt);
}



