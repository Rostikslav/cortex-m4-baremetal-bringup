# GPIOTE (GPIO Tasks and Events)

## Goal
This step introduces the GPIOTE peripheral and extends the system with hardware-supported GPIO event detection and task-based output control.

The goal is to implement a minimal but complete GPIOTE driver covering:
- channel-based event generation
- task-based output control
- port event handling via GPIO sense mechanism

## Implementation design

### Channel configuration

A single configuration interface is used for both event and task modes:

``` c
void gpiote_configure_channel(gpiote_config_t cfg);
```
The configuration includes:
- pin selection (`PSEL`)
- channel index
- mode (`EVENT` / `TASK`)
- polarity
- initial output state (`OUTINIT`)

``` c
typedef struct {
    uint8_t pin;
    gpiote_channel_t channel;
    gpiote_mode_t mode;
    gpiote_polarity_t polarity;
    gpiote_outinit_t outinit;
} gpiote_config_t;
```

This allows unified setup of GPIOTE channels independent of their runtime usage.

### Event mode (channel events)

In event mode, a channel generates an event on a configured pin transition:
- edge detection defined by `POLARITY`
- event exposed via `EVENTS_IN[n]`
- optional interrupt generation via INTENSET

The interrupt handler:
- scans all channels
- clears pending events
- invokes registered callbacks

### Task mode (output control)

In task mode, a channel controls a GPIO output via hardware tasks:
- `TASKS_OUT[n]` – executes configured output action
- `TASKS_SET[n]` – forces output high
- `TASKS_CLR[n]` – forces output low

When task mode is enabled:
- the selected pin is automatically configured as output
- GPIO `DIR` and `OUT` settings are overridden

Task functions are exposed as inline wrappers:
``` c
static inline void gpiote_task_out(gpiote_channel_t ch);
static inline void gpiote_task_set(gpiote_channel_t ch);
static inline void gpiote_task_clr(gpiote_channel_t ch);
```

### Port events (GPIO sense)

In addition to channel-based events, GPIOTE supports aggregated port events.

GPIO pins configured with `SENSE` generate a `PORT` event when the condition is met.

Characteristics:
- event exposed via `EVENTS_PORT`
- single interrupt source for all sensed pins
- source pin must be resolved in software

A dedicated port callback is invoked when a `PORT` event is detected.

## Testing

### Channel event test
- button input with pull-up
- channel configured for high-to-low edge detection
- ISR increments a counter
- main loop toggles LED based on accumulated events

Observed behavior:
- each button press triggers the callback which increments the counter
- main loop consumes the counter and blinks the LED with small delay for each decrement
- this results in accumulated blinking effect without distorting the blinking interval

### Task mode test
- channel configured in task mode with `POLARITY` = `TOGGLE`
- LED pin bound via `PSEL`
- delay before the main loop
- periodic calls to `gpiote_task_out()` with small delay

Observed behavior:
- initial output state defined by `OUTINIT`
- each task invocation toggles the output

This confirms correct task execution and output override behavior.

### Port event test
- two button inputs configured with pull-up and `GPIO_SENSE_LOW`
- PORT interrupt enabled
- callback toggles LED

Observed behavior:
- pressing either button generates a PORT event
- ISR clears `EVENTS_PORT` and invokes callback
- LED toggles on each button press

This confirms correct integration between GPIO sense mechanism and GPIOTE interrupt handling.

## Conclusion
Step05 extends the system with a functional GPIOTE driver supporting:
- channel-based event detection
- hardware-driven GPIO tasks
- aggregated port event handling

This completes the implementation of GPIO-driven event and task mechanisms on the nRF52832.

The system is now ready for communication peripherals in the next step.
