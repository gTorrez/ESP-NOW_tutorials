#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>


// MAC Adress genérico para enviar os dados no canal selecionado
// uint8_t broadcast_adr[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast_adr[] = {0x94, 0xE6, 0x86, 0x3C, 0xDD, 0x58};

//==============

typedef struct struct_message{
  float v_l;
  float v_a;
  } struct_message;

struct_message commands;

//==============

esp_now_peer_info_t peerInfo;

void setup() {
  Serial.begin(115200);

  ESP_ERROR_CHECK(esp_netif_init());
  ESP_ERROR_CHECK(esp_event_loop_create_default());
  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  ESP_ERROR_CHECK(esp_wifi_init(&cfg));
  ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
  ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
  ESP_ERROR_CHECK(esp_wifi_start());
  ESP_ERROR_CHECK(esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE));
  esp_wifi_set_max_tx_power(84);


  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, broadcast_adr, 6);
  if (esp_now_add_peer(&peerInfo) != ESP_OK) 
  {
    Serial.println("Failed to add peer");
    return;
  }
 
}

//=============

void loop() {
    commands.v_l = 19;
    commands.v_a = 10;
    sendData();

}

//==============


void sendData(){   
    // esse delay é necessário para que os dados sejam enviados corretamente
    esp_err_t message = esp_now_send(broadcast_adr, (uint8_t *) &commands, sizeof(commands));
    delay(3);
    
}
