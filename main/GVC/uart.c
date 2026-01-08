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
#include "defs.h" 
static QueueHandle_t uart0_queue;


static const char *TAG = "uart_events";
extern char SD_Name[100][100] ;  
int track_id,Total_Tracks,CurrentTrack;  




void process_uart_packet(const char *);
void sendData(char* data);
char payload[200];
 

void CalculateTotalTracks (void){
    Total_Tracks = 0;
    for(int i=0;i<100;i++){
        if(SD_Name[i][0] != '\0'){
            Total_Tracks++;
        }
        else{
            break;
        }
    }
}   

void PlayCurrentTrack(){
        CalculateTotalTracks ();
        sprintf(payload,"*PLAY-OK,%s#",SD_Name[CurrentTrack]);
        sendData(payload);
        Play_Music("/sdcard",SD_Name[CurrentTrack]);
 }

void sendData(char* data)
{
    const int len = strlen(data);
    const int txBytes = uart_write_bytes(EX_UART_NUM, data, len);
}

void process_uart_packet(const char *pkt){
     if(strncmp(pkt, "*PLAY:", 6) == 0){
       
        sscanf(pkt, "*PLAY:%d#", &track_id);
        CurrentTrack = track_id;
        PlayCurrentTrack();
    }
    else if(strncmp(pkt, "*PAUSE#", 7) == 0){
        Music_pause();
        sendData("*PAUSE-OK#");
    }
    else if(strncmp(pkt, "*RESUME#", 8) == 0){
        Music_resume();
        sendData("*RESUME-OK#");
    }
    else if(strncmp(pkt, "*NEXT#", 6) == 0){
        
        sendData("*NEXT-OK#");
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
        sendData("*PREV-OK#");
    }
    else if(strncmp(pkt, "*VOLUME:", 8) == 0){
        int vol;
        sscanf(pkt, "*VOLUME:%d#", &vol);
        if(vol <= Volume_MAX){
            Volume = vol;
            sprintf(payload,"*VOLUME-OK,%d#",vol);
            sendData(payload);
        }
        else{
            sendData("*VOLUME-ERR#");
        }
    }

}






static void uart_event_task(void *pvParameters)
{
    uart_event_t event;
    for(;;) {
        //Waiting for UART event.
        if(xQueueReceive(uart0_queue, (void * )&event, (TickType_t)portMAX_DELAY)) {
            switch(event.type) {
                //Event of UART receving data
                /*We'd better handler data event fast, there would be much more data events than
                other types of events. If we take too much time on data event, the queue might
                be full.*/
                case UART_DATA:{
                    char arr[event.size + 1];
                    uart_read_bytes(EX_UART_NUM, arr, event.size, portMAX_DELAY);
                    arr[event.size] = '\0';
                    process_uart_packet(arr);
                    break;
                }
                    
                //Event of HW FIFO overflow detected
                case UART_FIFO_OVF:
                    ESP_LOGI(TAG, "hw fifo overflow");
                    // If fifo overflow happened, you should consider adding flow control for your application.
                    // The ISR has already reset the rx FIFO,
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;
                //Event of UART ring buffer full
                case UART_BUFFER_FULL:
                    ESP_LOGI(TAG, "ring buffer full");
                    // If buffer full happened, you should consider encreasing your buffer size
                    // As an example, we directly flush the rx buffer here in order to read more data.
                    uart_flush_input(EX_UART_NUM);
                    xQueueReset(uart0_queue);
                    break;
                //Event of UART RX break detected
                case UART_BREAK:
                    ESP_LOGI(TAG, "uart rx break");
                    break;
                //Event of UART parity check error
                case UART_PARITY_ERR:
                    ESP_LOGI(TAG, "uart parity error");
                    break;
                //Event of UART frame error
                case UART_FRAME_ERR:
                    ESP_LOGI(TAG, "uart frame error");
                    break;
                //UART_PATTERN_DET
                case UART_PATTERN_DET:
                    ESP_LOGI(TAG, "[UART PATTERN DETECTED] ");
                    break;
                //Others
                default:
                    ESP_LOGI(TAG, "uart event type: %d", event.type);
                    break;
            }
        }
    }
    vTaskDelete(NULL);
}


void console_uart_init(void){
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    uart_driver_install(EX_UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 20, &uart0_queue, 0);
    uart_param_config(EX_UART_NUM, &uart_config);
    uart_set_pin(MKM_IC_UART, MKM_IC_UART_TX, MKM_IC_UART_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
//    uart_set_pin(EX_UART_NUM, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    xTaskCreate(uart_event_task, "uart_event_task", 4096, NULL, 6, NULL);
}
