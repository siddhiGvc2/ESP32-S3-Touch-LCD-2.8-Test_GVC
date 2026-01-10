#include "defs.h"

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
    else if(strncmp(rx_buffer, "*CC", 3) == 0){
        if(strcmp(InputVia, "TCP") == 0 || strcmp(InputVia, "MQTT") == 0)
        {
            sscanf(rx_buffer, "*CC:%[^:]:%[^:]:%[^#]#",CCuserName,CCdateTime,UniqueTimeStamp); // changed on 20-12-24 as per EC10
          
            sprintf(payload, "*CC-OK,%s,%s,%s#",CCuserName,CCdateTime,UniqueTimeStamp);  // changed on 20-12-24 as per EC10
            SendReply(payload,InputVia);
        }
        else if(strcmp(InputVia, "UART") == 0){
            strcpy(CCuserName,"LOCAL");
            strcpy(CCdateTime,"00/00/00");
            sprintf(payload, "*CC-OK,%s,%s#",CCuserName,CCdateTime);
            SendReply(payload,InputVia);
        }

            SaveString(NVS_CC_USERNAME, CCuserName);
            SaveString(NVS_CC_DATETIME, CCdateTime);  // added on 20-12-24 as per EC10
            SaveString(NVS_UNIX_TS, UniqueTimeStamp);
            // sendSocketData(sock, payload, strlen(payload), 0);
            for (int i = 0 ; i < 7 ; i++)
            {
                Totals[i] = 0;
                CashTotals[i] = 0;
            } 
            SaveInteger(NVS_CASH1_KEY, CashTotals[0]);
            SaveInteger(NVS_CASH2_KEY, CashTotals[1]);
            SaveInteger(NVS_CASH3_KEY, CashTotals[2]);
            SaveInteger(NVS_CASH4_KEY, CashTotals[3]);
            SaveInteger(NVS_CASH5_KEY, CashTotals[4]);
            SaveInteger(NVS_CASH6_KEY, CashTotals[5]);
            SaveInteger(NVS_CASH7_KEY, CashTotals[6]);

    }
    else if(strncmp(rx_buffer,"*CHENA:",7)==0)
    {
        int temp[NUM_CHANNELS] = {0};
        int parsed = sscanf(rx_buffer, "*CHENA:%d:%d:%d:%d:%d:%d:%d#",
                            &temp[0], &temp[1], &temp[2],
                            &temp[3], &temp[4], &temp[5], &temp[6]);

        if (parsed == NUM_CHANNELS) {
            for (int i = 0; i < NUM_CHANNELS; i++) {
                EnabledChannel[i] = (temp[i] ? 1 : 0); // force 0/1
            }
              SaveInteger(NVS_CHNL1_KEY, EnabledChannel[0]);
            SaveInteger(NVS_CHNL2_KEY, EnabledChannel[1]);
            SaveInteger(NVS_CHNL3_KEY, EnabledChannel[2]);
            SaveInteger(NVS_CHNL4_KEY, EnabledChannel[3]);
            SaveInteger(NVS_CHNL5_KEY, EnabledChannel[4]);
            SaveInteger(NVS_CHNL6_KEY, EnabledChannel[5]);
            SaveInteger(NVS_CHNL7_KEY, EnabledChannel[6]);
            sprintf(payload,"*CHENA:%d:%d:%d:%d:%d:%d:%d#",
                 EnabledChannel[0], EnabledChannel[1], EnabledChannel[2],
                 EnabledChannel[3], EnabledChannel[4], EnabledChannel[5],
                 EnabledChannel[6]);

            SendReply(InputVia,payload);
        }
    }
    //030925
    else if (strncmp(rx_buffer, "*PULSES?", 8) == 0) {
  
        
        sprintf(payload,"*CHENA:%d:%d:%d:%d:%d:%d:%d#",
                 EnabledChannel[0], EnabledChannel[1], EnabledChannel[2],
                 EnabledChannel[3], EnabledChannel[4], EnabledChannel[5],
                 EnabledChannel[6]);

        SendReply(InputVia,payload);
    }
   
    
    
     else if(strncmp(rx_buffer, "*TC?#", 5) == 0){
      
        sprintf(payload, "*TC,%s,%d,%d,%d,%d,%d,%d,%d#", 
        UniqueTimeStamp,CashTotals[0],CashTotals[1],CashTotals[2],CashTotals[3],CashTotals[4],CashTotals[5],CashTotals[6]);
        SendReply(payload,InputVia);
       
    }
    else if(strncmp(rx_buffer, "*TV?#", 5) == 0){
       
        sprintf(payload, "*TV,%d,%d,%d,%d,%d,%d,%d#", 
        Totals[0], Totals[1], Totals[2], Totals[3], Totals[4], Totals[5], Totals[6]);
        SendReply(payload,InputVia);
    }
      else if(strncmp(rx_buffer, "*CC?#", 5) == 0){
        sprintf(payload,"*CC,%s,%s,%s#",CCuserName,CCdateTime,UniqueTimeStamp);
        SendReply(payload,InputVia);
       
    }
    else  if(strncmp(rx_buffer, "*CA?#", 5) == 0){
        sprintf(payload,"*CA-OK,%s,%s,%d,%d#",CAuserName,CAdateTime,pulseWidth,SignalPolarity);
        SendReply(payload,InputVia);
    }




}
