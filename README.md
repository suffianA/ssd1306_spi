# SSD1306 OLED SPI Driver using FreeRTOS

A lightweight implementation of the SPI communication protocol for the **SSD1306 OLED display**, optimized for use within the **FreeRTOS** environment. This driver is designed for efficiency, utilizing polling-based transmission and direct GPIO control for high-speed display updates.

## **Key API Functions**

*   **`oled_init`**: Executes a pre-defined hardware initialization sequence to configure the SSD1306 controller.
*   **`oled_cmd`**: Transmits single-byte commands by pulling the **D/C (Data/Command)** pin low.
*   **`oled_data`**: Transmits pixel or configuration data using polling-based SPI transmission with the **D/C** pin pulled high.
*   **`oled_fill`**: Clears or fills the entire display memory with a specified byte pattern (e.g., `0x00` for clear, `0xFF` for full white).

---

## **Hardware Implementation**

This driver assumes an SPI configuration used on an **ESP32** or similar microcontrollers. Ensure your wiring matches the following requirements:

*   **D/C Pin**: Used to distinguish between Command and Data modes.
*   **SPI Mode**: Supports standard 4-wire SPI (SCLK, MOSI, CS, D/C).
*   **FreeRTOS Integration**: Designed to be called within a dedicated display task or during system startup.

---

## **Project Structure**
```text
├── CMakeLists.txt         # Project-level build configuration[cite: 1]
├── main/                  # Application source directory[cite: 1]
│   ├── CMakeLists.txt     # Main component build rules[cite: 1]
│   └── main.c             # Implementation of SPI logic and FreeRTOS tasks[cite: 1]
└── README.md              # Project documentation[cite: 1]
