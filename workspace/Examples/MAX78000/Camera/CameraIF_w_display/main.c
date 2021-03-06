/*******************************************************************************
* Copyright (C) Maxim Integrated Products, Inc., All Rights Reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
* IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
* OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
* ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
* OTHER DEALINGS IN THE SOFTWARE.
*
* Except as contained in this notice, the name of Maxim Integrated
* Products, Inc. shall not be used except as stated in the Maxim Integrated
* Products, Inc. Branding Policy.
*
* The mere transfer of this software does not imply any licenses
* of trade secrets, proprietary technology, copyrights, patents,
* trademarks, maskwork rights, or any other form of intellectual
* property whatsoever. Maxim Integrated Products, Inc. retains all
* ownership rights.
*
******************************************************************************/

/**
 * @file    main.c
 * @brief   Parallel camera example with the OV7692.
 *
 * @details This example uses the UART to stream out the image captured from the camera.
 *          The image is prepended with a header that is interpreted by the grab_image.py
 *          python script.  The data from this example through the UART is in a binary format.
 *          Instructions: 1) Load and execute this example. The example will initialize the camera
 *                        and start the repeating binary output of camera frame data.
 *                        2) Run 'sudo grab_image.py /dev/ttyUSB0 115200'
 *                           Substitute the /dev/ttyUSB0 string for the serial port on your system.
 *                           The python program will read in the binary data from this example and
 *                           output a png image.
 */

/***** Includes *****/
#include <stdio.h>
#include <stdint.h>
#include "mxc_device.h"
#include "uart.h"
#include "led.h"
#include "board.h"
#include "mxc_delay.h"
#include "camera.h"
#include "utils.h"
#include "dma.h"
#include "pb.h"
#include "gpio.h"

#include <stdlib.h>
#include <string.h>
#include <gpio.h>
#include <uart.h>
#include <mxc_delay.h>

#include "ff.h"
#include "sd.h"

#ifdef BOARD_EVKIT_V1
#include "bitmap.h"
#include "tft.h"
#endif
#ifdef BOARD_FTHR_REVA
#include "tft_fthr.h"
#endif

#define IMAGE_XRES  100
#define IMAGE_YRES  100
#define TFT_BUFFER_SIZE 2500
#define IMAGE_LENGTH IMAGE_XRES
#define CAMERA_FREQ (10 * 1000 * 1000)

#define TFT_BUFF_SIZE   30    // TFT buffer size

/***** GPIO ***************/
#define LORA_RESET_PORT_OUT               MXC_GPIO1
#define LORA_RESET_PIN_OUT                MXC_GPIO_PIN_6

#define TFT_SS_PORT_OUT               MXC_GPIO0
#define TFT_SS_PIN_OUT                MXC_GPIO_PIN_19


#define LORA_TFT_SPI_PINS MXC_GPIO_PIN_5 | MXC_GPIO_PIN_6 | MXC_GPIO_PIN_7 | MXC_GPIO_PIN_11

/********SPI******************/
#define SPI         MXC_SPI0

#ifdef BOARD_EVKIT_V1
int image_bitmap_1 = img_1_bmp;
int image_bitmap_2 = logo_white_bg_darkgrey_bmp;
int font_1 = urw_gothic_12_white_bg_grey;
int font_2 = urw_gothic_13_white_bg_grey;
#endif
#ifdef BOARD_FTHR_REVA
int image_bitmap_1 = (int)& img_1_rgb565[0];
int image_bitmap_2 = (int)& logo_rgb565[0];
int font_1 = (int)& SansSerif16x16[0];
int font_2 = (int)& SansSerif16x16[0];
#endif

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define MAXLEN 256

#define TFT_SPI0_PIN 	MXC_GPIO_PIN_5 | MXC_GPIO_PIN_6 | MXC_GPIO_PIN_7 | MXC_GPIO_PIN_11|MXC_GPIO_PIN_19

uint32_t imNum = 0;

extern FRESULT err;
extern TCHAR cwd[MAXLEN];
extern TCHAR* FF_ERRORS[20];

//imageLength = IMAGE_XRES * IMAGE_YRES;

uint32_t input_0_camera[TFT_BUFFER_SIZE];
uint32_t input_1_camera[TFT_BUFFER_SIZE];
uint32_t input_2_camera[TFT_BUFFER_SIZE];

int imgNum = 0;
//32768
//131072

//https://blog.fearcat.in/a?ID=00900-e289dd3c-202f-4105-8437-7de05cc65166
void RGB565ToRGB888Char(uint8_t* rgb565, uint8_t* rgb888)
{
    uint8_t byte1 = rgb565[2];
    uint8_t byte2 = rgb565[3];
    uint8_t byte3 = rgb565[0];
    uint8_t byte4 = rgb565[1];

    uint16_t pixel1 = byte1 * 0x100 + byte2;
    uint16_t pixel2 = byte3 * 0x100 + byte4;

    uint8_t r1 = (pixel1 >>11) & 0x1f;
    uint8_t g1 = (pixel1 >>5) & 0x3f;
    uint8_t b1 = (pixel1 >>0) & 0x1f;
    r1 = r1 * 255 / 31;
    g1 = g1 * 255 / 63;
    b1 = b1 * 255 / 31;
    rgb888[3] = (r1);
    rgb888[4] = (g1);
    rgb888[5] = (b1);

    uint8_t r2 = (pixel2 >>11) & 0x1f;
    uint8_t g2 = (pixel2 >>5) & 0x3f;
    uint8_t b2 = (pixel2 >>0) & 0x1f;
    r2 = r2 * 255 / 31;
    g2 = g2 * 255 / 63;
    b2 = b2 * 255 / 31;
    rgb888[0] = (r2);
    rgb888[1] = (g2);
    rgb888[2] = (b2);
}

void img565To888(uint8_t* img565, uint8_t* img888)
{
  int imgLen = 64*64*2;
  for (int i = 0; i < imgLen/4; i++)
  {
    RGB565ToRGB888Char(&img565[i*4], &img888[i*6]);
  }
}

void process_img(void)
{
    // uint8_t*   raw;
    // uint32_t  imgLen;
    // uint32_t  w, h;
    

    // // Get the details of the image from the camera driver.
    // camera_get_image(&raw, &imgLen, &w, &h);


    // uint8_t *imgSeg = (uint8_t*)malloc(64*64*4*sizeof(uint8_t));
    // uint32_t imgSegLen = 64*64*4;

    // img565To888(raw, imgSeg);

    // int err = createRawImage(raw, imgLen,imgNum);
    // err = createRawImage(imgSeg, imgSegLen,imgNum+1000);

    // if (err==0)
    // 	imgNum++;

}

// *****************************************************************************
void process_camera_img(uint8_t* imgBlock, uint32_t *data0, uint32_t *data1, uint32_t *data2)
{
  uint8_t *ptr0;
  uint8_t *ptr1;
  uint8_t *ptr2;
  uint8_t *buffer;
  ptr0 = (uint8_t *)data0;
  ptr1 = (uint8_t *)data1;
  ptr2 = (uint8_t *)data2;
  buffer = imgBlock;

  for (int y = 0; y < IMAGE_YRES; y++) {
    for (int x = 0; x < IMAGE_XRES; x++, ptr0++, ptr1++, ptr2++) {
            *ptr0 = (*buffer);
            buffer++;
            *ptr1 = (*buffer);
            buffer++;
            *ptr2 = (*buffer);
            buffer++;
    }
  }
}



/* **************************************************************************** */
void TFT_Print(char* str, int x, int y, int font, int length)
{
  // fonts id
  text_t text;
  text.data = str;
    text.len = length;
  MXC_TFT_PrintFont(x, y, font, &text, NULL);
}

#define X_OFFSET    15
#define Y_OFFSET    15
#define SCALE       2.2

/* **************************************************************************** */

void lcd_show_sampledata(uint32_t* data0, uint32_t* data1, uint32_t* data2, int length)
{
  int i;
  int j;
  int x;
  int y;
  int r;
  int g;
  int b;
    int scale = SCALE;
    
  uint32_t color;
  uint8_t *ptr0;
  uint8_t *ptr1;
  uint8_t *ptr2;

    x = X_OFFSET;
    y = Y_OFFSET;

  for (i = 0; i < length; i++) {
    ptr0 = (uint8_t *)&data0[i];
    ptr1 = (uint8_t *)&data1[i];
    ptr2 = (uint8_t *)&data2[i];
    for (j = 0; j < 4; j++) {
      r = ptr0[j];
      g = ptr1[j];
      b = ptr2[j];        
#ifdef BOARD_EVKIT_V1
      color  = (0x01000100 | ((b & 0xF8) << 13) | ((g & 0x1C) << 19) | ((g & 0xE0) >> 5) | (r & 0xF8));
#endif
#ifdef BOARD_FTHR_REVA
            color = RGB(r, g, b); // convert to RGB565
#endif
      MXC_TFT_WritePixel(x * scale, y * scale, scale, scale, color);
      x += 1;

            if (x >= (IMAGE_XRES + X_OFFSET)) {
                x = X_OFFSET;
        y += 1;

                if ((y + 6) >= (IMAGE_YRES + Y_OFFSET)) {
                    return;
                }
      }
    }
  }
}

/* **************************************************************************** */
int main(void)
{
    int ret = 0;
    int slaveAddress;
    int id;
    int dma_channel;
    

    // Initialize DMA for camera interface
    MXC_DMA_Init();
    dma_channel = MXC_DMA_AcquireChannel();
  
    // Initialize the camera driver.
    camera_init(CAMERA_FREQ);
    printf("\n\nCameraIF Example\n");
    
    // Obtain the I2C slave address of the camera.
    slaveAddress = camera_get_slave_address();
    printf("Camera I2C slave address is %02x\n", slaveAddress);
    
    // Obtain the product ID of the camera.
    ret = camera_get_product_id(&id);
    
    if (ret != STATUS_OK) {
        printf("Error returned from reading camera id. Error %d\n", ret);
        return -1;
    }
    
    printf("Camera Product ID is %04x\n", id);
    
    // Obtain the manufactor ID of the camera.
    ret = camera_get_manufacture_id(&id);
    
    if (ret != STATUS_OK) {
        printf("Error returned from reading camera id. Error %d\n", ret);
        return -1;
    }
    
    printf("Camera Manufacture ID is %04x\n", id);
    
    // Setup the camera image dimensions, pixel format and data aquiring details.PIXFORMAT_RGB565, FIFO_FOUR_BYTE
    //ret = camera_setup(IMAGE_XRES, IMAGE_YRES, PIXFORMAT_RGB888, FIFO_THREE_BYTE, USE_DMA, dma_channel);
    ret = camera_setup(IMAGE_XRES, IMAGE_YRES, PIXFORMAT_BAYER, FIFO_FOUR_BYTE, USE_DMA, dma_channel); // Mono
    //ret = camera_setup(IMAGE_XRES, IMAGE_YRES, PIXFORMAT_RGB565, FIFO_FOUR_BYTE, USE_DMA, dma_channel);
    if (ret != STATUS_OK) {
        printf("Error returned from setting up camera. Error %d\n", ret);
        return -1;
    }



    #ifdef TFT_ENABLE
        /* Initialize TFT display */

    mxc_gpio_cfg_t tft_ss_pin = {MXC_GPIO0, MXC_GPIO_PIN_19, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH};
    MXC_GPIO_Config(&tft_ss_pin);
    MXC_GPIO_OutSet(MXC_GPIO0, MXC_GPIO_PIN_19);
    
    printf("Init LCD.\n");
    #ifdef BOARD_EVKIT_V1
        mxc_gpio_cfg_t tft_reset_pin = {MXC_GPIO0, MXC_GPIO_PIN_19, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH};
        MXC_TFT_Init(MXC_SPI0, 1, &tft_reset_pin, NULL);
        MXC_TFT_ClearScreen();
        MXC_TFT_SetRotation(SCREEN_FLIP);
    #endif
    #ifdef BOARD_FTHR_REVA
          /* Initialize TFT display */
          
          int master = 1;
          int quadMode = 0;
          int numSlaves = 2;
          int ssPol = 0;
          unsigned int tft_hz = 10000000;

          mxc_spi_pins_t tft_pins;

          tft_pins.clock = true;
          tft_pins.ss0 = true;       ///< Slave select pin 0
          tft_pins.ss1 = true;       ///< Slave select pin 1
          tft_pins.ss2 = false;       ///< Slave select pin 2
          tft_pins.miso = true;      ///< miso pin
          tft_pins.mosi = true;      ///< mosi pin
          tft_pins.sdio2 = false;     ///< SDIO2 pin
          tft_pins.sdio3 = false;     ///< SDIO3 pin

          MXC_SPI_Init(SPI, master, quadMode, numSlaves, ssPol, tft_hz, tft_pins);

          MXC_Delay(5000);

          // Set  SPI0 pins to VDDIOH (3.3V) to be compatible with TFT display
          MXC_GPIO_SetVSSEL(SPI, MXC_GPIO_VSSEL_VDDIOH, LORA_TFT_SPI_PINS);
          MXC_SPI_SetDataSize(SPI, 8);
          MXC_SPI_SetWidth(SPI, SPI_WIDTH_STANDARD);
          
          mxc_gpio_cfg_t tft_reset_pin = {MXC_GPIO0, MXC_GPIO_PIN_16, MXC_GPIO_FUNC_OUT, MXC_GPIO_PAD_NONE, MXC_GPIO_VSSEL_VDDIOH};
        MXC_TFT_Init(MXC_SPI0, 2, &tft_reset_pin, NULL);
        MXC_TFT_SetRotation(ROTATE_180);
        MXC_TFT_SetForeGroundColor(WHITE);   // set chars to white
    #endif
        MXC_Delay(1000000);
    #endif

    
    FF_ERRORS[0] = "FR_OK";
    FF_ERRORS[1] = "FR_DISK_ERR";
    FF_ERRORS[2] = "FR_INT_ERR";
    FF_ERRORS[3] = "FR_NOT_READY";
    FF_ERRORS[4] = "FR_NO_FILE";
    FF_ERRORS[5] = "FR_NO_PATH";
    FF_ERRORS[6] = "FR_INVLAID_NAME";
    FF_ERRORS[7] = "FR_DENIED";
    FF_ERRORS[8] = "FR_EXIST";
    FF_ERRORS[9] = "FR_INVALID_OBJECT";
    FF_ERRORS[10] = "FR_WRITE_PROTECTED";
    FF_ERRORS[11] = "FR_INVALID_DRIVE";
    FF_ERRORS[12] = "FR_NOT_ENABLED";
    FF_ERRORS[13] = "FR_NO_FILESYSTEM";
    FF_ERRORS[14] = "FR_MKFS_ABORTED";
    FF_ERRORS[15] = "FR_TIMEOUT";
    FF_ERRORS[16] = "FR_LOCKED";
    FF_ERRORS[17] = "FR_NOT_ENOUGH_CORE";
    FF_ERRORS[18] = "FR_TOO_MANY_OPEN_FILES";
    FF_ERRORS[19] = "FR_INVALID_PARAMETER";

    #ifdef TFT_ENABLE
    char buff[TFT_BUFF_SIZE];
    #endif

    #ifdef TFT_ENABLE
            // MXC_TFT_SetPalette(image_bitmap_2);
        MXC_TFT_SetBackGroundColor(4);
        MXC_TFT_SetRotation(ROTATE_180);
            // MXC_TFT_ShowImage(1, 1, image_bitmap_2);
        memset(buff,32,TFT_BUFF_SIZE);
    #endif


    uint8_t*   raw;
    uint32_t  imgLen;
    uint32_t  w, h;
    //uint8_t imgBlock888[16384];

    int imgNum = 0;

    // Start off the first camera image frame.
    camera_start_capture_image();
    
    while (1) {
        // Check if image is aquired.
        if (camera_is_image_rcv()) {
            // Process the image, send it through the UART console.
        	LED_On(LED1);

            camera_get_image(&raw, &imgLen, &w, &h);


            //img565To888(raw, imgBlock888);
            //process_camera_img(imgBlock888, input_0_camera, input_1_camera, input_2_camera);
            process_camera_img(raw, input_0_camera, input_1_camera, input_2_camera);
            lcd_show_sampledata(input_0_camera, input_1_camera, input_2_camera, TFT_BUFFER_SIZE);
            
            //process_img();
          while(1){
            if(PB_Get(0))
            {
              TFT_Print(buff, 40, 200, font_1, sprintf(buff, "Saved"));
              //int err = createRawImage(raw, imgLen ,imgNum);
              //if (!err)
    	          //imgNum++;
                break;
            }
            if(PB_Get(1))
            {
              TFT_Print(buff, 40, 200, font_1, sprintf(buff, "Deleted"));
              break;
            }

          } 
            MXC_TFT_ClearScreen();
            LED_Off(LED1);
            // Prepare for another frame capture.
            camera_start_capture_image();
        }
    }
    
    return ret;
}
