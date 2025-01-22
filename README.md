# Linux Events

## About

Interface to periodically get input events

## Usage

```c
#include "events.h"

// init
struct input_event	*event;
driver = event_driver_init("/dev/input/");
event_driver_run(driver);

// get
event = (struct input_event *)event_driver_get(driver, &key);
event_driver_clean_get(driver, event);

// free
event_driver_free(driver);
```