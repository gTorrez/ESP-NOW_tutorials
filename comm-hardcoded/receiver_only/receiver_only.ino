#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>


// This is de code for the board that is in robots
int first_mark = 0, second_mark;


const byte numChars = 64;
char commands[numChars];
char tempChars[numChars];

float v_l, v_a;

typedef struct struct_message{
  float v_a;
  float v_l;
  } struct_message;

struct_message rcv_commands;
struct_message robot;


void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
  memcpy(&rcv_commands, incomingData, sizeof(rcv_commands));
  // Update the structures with the new incoming data
  first_mark = millis();
  robot.v_l = rcv_commands.v_l;
  robot.v_a = rcv_commands.v_a;
  Serial.println();
}


void setup() {
  Serial.begin(115200);

  // configurações comunicação

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
  ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
  ESP_ERROR_CHECK( esp_wifi_set_mode(WIFI_MODE_STA) );
  ESP_ERROR_CHECK( esp_wifi_start());
  ESP_ERROR_CHECK( esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));

  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(OnDataRecv);
  

  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());

}


void loop() {
  second_mark = millis();
  
  strcpy(tempChars, commands); // necessário para proteger a informação original

  v_l = robot.v_l;
  v_a = robot.v_a;
  

  if (second_mark - first_mark > 500) {
    v_l = 0.00;
    v_a = 0.00;
  }

  Serial.println();
  Serial.print("vl: ");
  Serial.print(v_l);
  Serial.print(" | ");
  Serial.print("va: ");
  Serial.print(v_a);
  Serial.println();
}
