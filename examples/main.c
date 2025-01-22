#include "events.h"
#include <stdio.h>
#include <unistd.h>

int	main(void)
{
	struct input_event	*event;
	t_event_driver		driver;
	bool				run;
	uint16_t			key;

	driver = event_driver_init("/dev/input/");
	event_driver_run(driver);
	key = EV_KEY;
	run = true;
	while (run)
	{
		sleep(1);
		event = (struct input_event *)event_driver_get(driver, &key);
		if (event)
		{
			if (event->code == KEY_ESC)
				run = false;
			printf("Event type: %d\n", event->type);
			printf("Event code: %d\n", event->code);
			printf("Event value: %d\n", event->value);
			event_driver_clean_get(driver, event);
		}
	}
	event_driver_free(driver);
}
