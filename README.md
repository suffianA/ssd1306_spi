# _Simple SPI Display for SSD1306 OLED DISPLAY_

Implementation of SPI using freeRTOS library

API's:
- Send command via DC using transmit (oled_cmd)
- Send data via DC using polling transmit (oled_data)
- Fill OLED display (oled_fill)
- Pre-defined initialization for SPI (oled_init)
-  

# _File Structure_
```
├── CMakeLists.txt
├── main
│   ├── CMakeLists.txt
│   └── main.c
└── README.md                  This is the file you are currently reading
