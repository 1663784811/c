#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "esp_event.h"
#include "nvs_flash.h"

// ===========    项目头文件 START
#include "myWifi.h"


// ===========    项目头文件 END




void nvs_init(void){
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK( ret );
}



void app_main(void)
{
    // ========================================= 初始化 START
    printf("Init ...\n");
    // 初始化
    nvs_init();
   
    my_wifi_init_sta();

    my_wifi_init_ap();
    

    // 扫描wifi
    my_wifi_scan();

    // ========================================= 初始化 END


    while(true) {
        printf("..............心跳..........\n");
        vTaskDelay(portTICK_PERIOD_MS * 100);
    }

}
