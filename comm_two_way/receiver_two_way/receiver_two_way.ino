#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#define LED 2

int robot_id = 9;
int id;
int first_mark = 0, second_mark;
int i = 0;

float new_value = 0;
float v_l, v_a;
float kp, ki, kd;

const byte numChars = 64;
char commands[numChars];
char tempChars[numChars];

uint8_t broadcast_adr[] = {0xA4, 0xCF, 0x12, 0x72, 0xB7, 0x20};

typedef struct struct_message_send{
  float value;
  } struct_message_send;

struct_message_send send_command;

// ==================================================================================
typedef struct struct_message{
  char message[64];
  } struct_message;

struct_message rcv_commands;

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&rcv_commands, incomingData, sizeof(rcv_commands));
  // Update the structures with the new incoming data
  first_mark = millis();
  strcpy(commands, rcv_commands.message);
}
// ==================================================================================

esp_now_peer_info_t peerInfo;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
  ESP_ERROR_CHECK( esp_wifi_start());
  ESP_ERROR_CHECK( esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));
  
//=====================================================================
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);

  memcpy(peerInfo.peer_addr, broadcast_adr, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) 
  {
    Serial.println("Failed to add peer");
    return;
  }
}

void loop() {
  second_mark = millis();
  digitalWrite(LED, LOW);

  strcpy(tempChars, commands); // necessário para proteger a informação original
  parseData();


  Serial.print("kp");
  Serial.print(kp);
  Serial.print("ki");
  Serial.print(ki);
  Serial.print("kd");
  Serial.println(kd);
  
  send_command.value = kp + ki + kd;

  if (second_mark - first_mark > 500) {
    v_l = 0.00;
    v_a = 0.00;
    }

  if(new_value != send_command.value){
    sendData();
    new_value = send_command.value;
    }
    
}

void parseData(){
    char * strtokIndx;
  
    strtokIndx = strtok(tempChars, ",");
    
    while (strtokIndx != NULL){
        id = atoi(strtokIndx);
        
        if(id == robot_id){         
          strtokIndx = strtok(NULL, ",");  
          kp = atof(strtokIndx);       
          strtokIndx = strtok(NULL, ",");         
          ki = atof(strtokIndx);
          strtokIndx = strtok(NULL, ","); 
          kd = atof(strtokIndx);
          strtokIndx = strtok(NULL, ","); 
       }

       else{
          strtokIndx = strtok(NULL, ",");     
          strtokIndx = strtok(NULL, ",");         
          strtokIndx = strtok(NULL, ","); 
          strtokIndx = strtok(NULL, ","); 
       }
   } 
}

void sendData(){   
    digitalWrite(LED, HIGH);
    // esse delay é necessário para que os dados sejam enviados corretamente
    esp_err_t message = esp_now_send(broadcast_adr, (uint8_t *) &send_command, sizeof(send_command));
    delay(3);
}
