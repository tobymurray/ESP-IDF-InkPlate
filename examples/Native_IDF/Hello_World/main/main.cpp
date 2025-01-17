/*
   The purpose of this example is to show how to use the ESP-IDF-InkPlate library without using Platformio SDK
   It will display a message "Hello World" in the center of the screen and then exit the program
   NOTE: Partial update is only available on 1 Bit mode (BW) and it is not recommended to use it on first refresh after
   power up. It is recommended to do a full refresh every 5-10 partial refresh to maintain good picture quality.

   Want to learn more about Inkplate? Visit www.inkplate.io
   Looking to get support? Write on our forums: http://forum.e-radionica.com/en/
   20 July 2022 by tajnymag
*/

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include <cstring>

#include "inkplate.hpp"            //Include Inkplate library to the sketch

Inkplate display(DisplayMode::INKPLATE_1BIT); // Create an object on Inkplate library and also set library into 1-bit mode (BW)

static const char * TAG = "Main";

void mainTask(void * param)
{
  ESP_LOGI(TAG, "Main task has started, configured as %s", INKPLATE_VARIANT);

  display.begin();                    // Init Inkplate library (you should call this function ONLY ONCE)
  display.clearDisplay();             // Clear frame buffer of display

  display.setTextColor(BLACK, WHITE); // Set text color to be black and background color to be white
  display.setTextSize(4);             // Set text to be 4 times bigger than classic 5x7 px text

  // Prepare message text and position
  char message[] = "Hello World";
  int w = display.width();
  int h = display.height();
  int cursor_x = (w / 2) - (strlen(message) / 2);
  int cursor_y = h / 2;

  ESP_LOGI(TAG, "Display size: width: %d, height: %d", w, h);

  // Write text to the center of the screen
  display.setCursor(cursor_x, cursor_y);
  display.print(message);

  // Do a partial update and print the text to the screen
  display.partialUpdate();

  ESP_LOGI(TAG, "Main task has ended");

  // Close the task and exit
  // (not neccessary for tasks with an inifinite loop)
  vTaskDelete(NULL);
}


#define STACK_SIZE 10000

extern "C" {

  void app_main()
  {
    TaskHandle_t xHandle = NULL;

    xTaskCreate(mainTask, "mainTask", STACK_SIZE, (void *) 1, tskIDLE_PRIORITY, &xHandle);
    configASSERT(xHandle);
  }

} // extern "C"