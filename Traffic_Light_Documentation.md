# Lesson 3 — GPIO and Modular Traffic-Light Controller

**Project:** ESP32 Embedded Systems Learning  
**Framework:** ESP-IDF v6.0.2  
**Target:** ESP32 DevKit V1  
**Status:** Complete and verified on hardware

---

## 1. Purpose

This lesson developed GPIO input/output fundamentals into a small, modular embedded application.

The final project is a traffic-light controller using three LEDs:

- Red
- Yellow
- Green

The project was intentionally implemented using a professional modular C structure rather than placing hardware control and application logic inside `app_main()`.

---

## 2. Functional Requirements

The controller shall:

1. Initialize the LED hardware.
2. Start in the RED state.
3. Display RED for approximately 3 seconds.
4. Display RED + YELLOW for approximately 1 second.
5. Display GREEN for approximately 3 seconds.
6. Display YELLOW for approximately 1 second.
7. Repeat continuously.

Expected sequence:

```text
RED
  ↓
RED + YELLOW
  ↓
GREEN
  ↓
YELLOW
  ↓
RED
  ↓
repeat
```

---

## 3. Hardware

### Controller

ESP32 DevKit V1.

### LED connections

| Function | GPIO |
|---|---:|
| Red LED | GPIO2 |
| Yellow LED | GPIO18 |
| Green LED | GPIO19 |

Each LED is connected through a current-limiting resistor to GND:

```text
GPIO → resistor → LED → GND
```

### Hardware observation

GPIO4 and GPIO5 were initially observed to glow dimly because they had previously been configured as inputs with internal pull-ups. This was investigated and confirmed to be expected weak pull-up behaviour. GPIO18 and GPIO19 were therefore selected for the traffic-light LEDs.

---

## 4. Software Architecture

The application was deliberately separated into modules.

```text
main/
├── hello_world_main.c
├── board.h
├── led.c
├── led.h
├── traffic_light.c
├── traffic_light.h
└── CMakeLists.txt
```

### Architecture

```text
hello_world_main.c
        │
        ├──────────────► led.c
        │
        └──────────────► traffic_light.c
                              │
                              ▼
                           led.c
                              │
                              ▼
                       ESP-IDF GPIO driver
                              │
                              ▼
                            ESP32
```

### Module responsibilities

#### `hello_world_main.c`

Owns application startup and module initialization.

It does not contain GPIO implementation details or traffic-light state logic.

#### `board.h`

Contains board-specific hardware definitions such as GPIO assignments.

#### `led.c / led.h`

Owns LED hardware control.

Public interface:

```c
void led_init(void);
void led_set(led_mask_t mask);
```

#### `traffic_light.c / traffic_light.h`

Owns traffic-light behaviour, state transitions, and timing.

Public interface:

```c
void traffic_light_init(void);
void traffic_light_run(void);
```

---

## 5. LED API Design

The LED module uses a bit-mask representation.

```c
typedef enum
{
    LED_OFF    = 0,
    LED_RED    = 1 << 0,
    LED_YELLOW = 1 << 1,
    LED_GREEN  = 1 << 2
} led_mask_t;
```

This allows individual LEDs or combinations of LEDs to be represented.

For example:

```c
led_set(LED_RED);
```

turns on only the red LED.

```c
led_set(LED_RED | LED_YELLOW);
```

turns on both red and yellow.

### Bit-mask principle

Each LED occupies a different bit:

```text
RED       = 001
YELLOW    = 010
GREEN     = 100
```

Combining red and yellow:

```text
  001
| 010
-----
  011
```

The LED driver tests individual bits using bitwise AND (`&`).

```c
(mask & LED_RED) != 0
```

This asks whether the red bit is set.

Bit masks are widely used in embedded C for hardware registers, flags, configuration fields, and peripheral control.

---

## 6. LED Module Encapsulation

The LED module contains a private helper:

```c
static void set_all_off(void);
```

The `static` keyword makes the function private to `led.c`.

The rest of the application only sees the public API declared in `led.h`.

This provides encapsulation:

```text
PUBLIC
├── led_init()
└── led_set()

PRIVATE
└── set_all_off()
```

The application does not need to know about:

- GPIO numbers
- `gpio_set_level()`
- GPIO direction configuration
- LED implementation details

---

## 7. Traffic-Light State Machine

The traffic-light controller uses an enumeration to represent its finite states:

```c
typedef enum
{
    TRAFFIC_RED,
    TRAFFIC_RED_YELLOW,
    TRAFFIC_GREEN,
    TRAFFIC_YELLOW
} traffic_state_t;
```

The current state is private to `traffic_light.c`:

```c
static traffic_state_t current_state;
```

The state machine follows:

```text
TRAFFIC_RED
     ↓
TRAFFIC_RED_YELLOW
     ↓
TRAFFIC_GREEN
     ↓
TRAFFIC_YELLOW
     ↓
TRAFFIC_RED
```

The state machine does not directly manipulate GPIOs.

Instead, it calls the LED module:

```c
led_set(LED_RED);
led_set(LED_RED | LED_YELLOW);
led_set(LED_GREEN);
led_set(LED_YELLOW);
```

This maintains separation between application behaviour and hardware control.

---

## 8. Timing Configuration

Traffic-light durations are defined using named constants rather than magic numbers:

```c
#define RED_DURATION_MS          3000U
#define RED_YELLOW_DURATION_MS   1000U
#define GREEN_DURATION_MS        3000U
#define YELLOW_DURATION_MS       1000U
```

The `U` suffix explicitly makes the integer constants unsigned.

Timing is implemented using FreeRTOS delay:

```c
vTaskDelay(pdMS_TO_TICKS(RED_DURATION_MS));
```

For this introductory project, blocking the current task during a traffic-light state is acceptable.

More advanced non-blocking designs will be introduced later when FreeRTOS tasks, timers, and event-driven architecture are studied.

---

## 9. Application Entry Point

The final application entry point is intentionally small:

```c
#include "led.h"
#include "traffic_light.h"

void app_main(void)
{
    led_init();
    traffic_light_init();

    while (1)
    {
        traffic_light_run();
    }
}
```

The application startup sequence is explicit:

1. Initialize the LED module.
2. Initialize the traffic-light module.
3. Run the traffic-light controller continuously.

`app_main()` coordinates modules but does not implement their internal behaviour.

---

## 10. Initialization Dependency

A real debugging issue occurred during integration.

The traffic-light state machine was correctly producing:

```text
STATE: RED
STATE: RED + YELLOW
STATE: GREEN
STATE: YELLOW
STATE: RED
```

However, the LEDs were not physically operating.

The cause was that `traffic_light_init()` initialized only the traffic-light state:

```c
current_state = TRAFFIC_RED;
```

It did not initialize the LED hardware.

The final solution was to make application startup explicitly initialize both modules:

```c
led_init();
traffic_light_init();
```

This preserves clear module ownership:

- `led.c` owns LED initialization.
- `traffic_light.c` owns traffic-light initialization.
- `app_main()` controls application initialization order.

---

## 11. Testing and Verification

### LED module test

The LED driver was independently tested before integrating the traffic-light state machine.

Verified:

```text
RED
RED + YELLOW
GREEN
YELLOW
OFF
```

The combined LED test verified that the bit-mask implementation correctly controlled multiple LEDs simultaneously.

### Traffic-light integration test

The traffic-light controller was tested using the ESP32 hardware.

Serial Monitor output confirmed the state sequence:

```text
STATE: RED
STATE: RED + YELLOW
STATE: GREEN
STATE: YELLOW
STATE: RED
...
```

Physical LED operation was subsequently verified.

### Build status

The complete modular traffic-light project built successfully.

### Hardware status

The complete traffic-light sequence operates successfully on the ESP32.

---

## 12. Problems Encountered and Resolutions

### Problem 1 — GPIO4 and GPIO5 appeared dim

These pins had previously been used as inputs with internal pull-ups.

A weak pull-up can source enough current to make an LED glow dimly.

Resolution:

- Avoided those pins for the traffic-light LEDs.
- Used GPIO18 and GPIO19 instead.

### Problem 2 — Multiple-definition linker error

The linker reported multiple definitions of:

```text
red_on
yellow_on
green_on
all_off
```

The functions existed both in the old `my_functions.c` module and in the new `led.c` module.

Resolution:

- Removed obsolete `my_functions.c`.
- Removed obsolete `my_functions.h`.
- Removed `my_functions.c` from `CMakeLists.txt`.

This reinforced the rule that one implementation of a global function should exist in the final linked application.

### Problem 3 — Traffic-light state machine ran but LEDs were off

Serial output proved that the state machine was functioning.

The actual problem was missing LED initialization.

Resolution:

```c
led_init();
traffic_light_init();
```

was added to `app_main()`.

---

## 13. C Concepts Practised

This lesson applied several important C concepts in an embedded context.

### Header/source separation

`.h` files expose interfaces.

`.c` files contain implementations.

### Enumeration

Used for finite named states:

```c
typedef enum
{
    TRAFFIC_RED,
    TRAFFIC_RED_YELLOW,
    TRAFFIC_GREEN,
    TRAFFIC_YELLOW
} traffic_state_t;
```

### Bit masks

Used to represent combinations of LEDs.

### Bitwise OR

Used to combine flags:

```c
LED_RED | LED_YELLOW
```

### Bitwise AND

Used to test individual flags:

```c
mask & LED_RED
```

### `static`

Used to keep implementation details private to a source file.

### Function interfaces

Modules expose small APIs rather than exposing hardware implementation details.

### `switch`

Used to implement state-dependent behaviour.

### Constants

Named timing constants replace unexplained numeric literals.

---

## 14. Engineering Principles Reinforced

### Single responsibility

Each module has a clear responsibility.

### Encapsulation

Implementation details remain private where possible.

### Separation of concerns

Traffic-light behaviour and GPIO hardware control are separate.

### Hardware abstraction

Traffic-light logic does not depend directly on GPIO numbers.

### Reusability

The LED module can be reused by another application without requiring traffic-light logic.

### Incremental testing

Modules were tested independently before integration.

### Explicit initialization

Hardware dependencies are initialized deliberately during application startup.

### Avoid unnecessary abstraction

A structure was considered but deliberately not introduced because it did not solve a current problem.

---

## 15. Why a `struct` Was Not Used

A structure would become useful if the application needed multiple configurable traffic-light instances, for example:

```c
typedef struct
{
    led_mask_t lights;
    uint32_t duration_ms;
    traffic_state_t state;
} traffic_light_t;
```

For the current single fixed traffic light, this would add unnecessary complexity.

The design principle is:

> Use abstraction when it solves a real engineering problem, not simply because the language provides the feature.

---

## 16. Current Project Status

**Status: COMPLETE**

Verified:

- [x] ESP32 GPIO output
- [x] Multiple LED control
- [x] Modular LED driver
- [x] Header/source separation
- [x] LED bit-mask API
- [x] `enum` for traffic states
- [x] Traffic-light state machine
- [x] Timing control
- [x] Module initialization
- [x] ESP-IDF build
- [x] Hardware flashing
- [x] Physical LED verification
- [x] Serial Monitor verification

---

## 17. Next Development Stage

The next stage will build on this modular architecture rather than returning to monolithic tutorial code.

Future modules will follow the same principles:

```text
button.c / button.h
uart.c / uart.h
adc.c / adc.h
pwm.c / pwm.h
sensor.c / sensor.h
```

Advanced C concepts such as structures, pointers, callbacks, and function pointers will be introduced when a real project requirement makes them useful.

The next major ESP-IDF topic will be FreeRTOS and multitasking.

---

## 18. Engineering Takeaway

The most important lesson from this project is not the traffic light itself.

The important lesson is the transition from:

```text
One large main.c
```

to:

```text
Application
    ↓
Modules
    ↓
Hardware abstraction
    ↓
ESP-IDF drivers
    ↓
Hardware
```

The objective is to become comfortable reading and designing firmware that is modular, testable, reusable, and maintainable.
