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

### Example

```c
/* ./examples/main.c */

#include "events.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Log event and cleans driver event list
 */
void	log_event(t_event_driver driver, struct input_event *event)
{
	if (!event)
		return ;
	printf("\n--------------------------------\n");
	printf("Event type: %d\n", event->type);
	printf("Event code: %d\n", event->code);
	printf("Event value: %d\n", event->value);
	printf("--------------------------------\n");
	event_driver_clean_get(driver, event);
}

int	main(void)
{
	struct input_event	*event;
	t_event_driver		driver;
	bool				run;
	uint16_t			key;

	if (getuid())
	{
		printf("%s\n", "Accessing /dev/input/* requires root");
		return (EXIT_FAILURE);
	}
	printf("This example will print events from /dev/input/*\n");
	printf("Press ESC to exit\n");
	printf("Try to move your mouse or press some keys\n");
	driver = event_driver_init("/dev/input/");
	event_driver_run(driver);
	key = EV_KEY;
	run = true;
	while (run)
	{
		sleep(1);
		event = (struct input_event *)event_driver_get(driver, &key);
		log_event(driver, event);
		key = EV_MSC;
		event = (struct input_event *)event_driver_get(driver, &key);
		log_event(driver, event);
		if (event && event->code == KEY_ESC)
			run = false;
	}
	event_driver_free(driver);
	return (EXIT_SUCCESS);
}

```