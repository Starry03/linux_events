#include "events.h"
#include "unic/dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	bool				run;
	t_event_driver		driver;
	struct input_event	*event;
	uint16_t			i;

	driver = event_driver_init("/dev/input/");
	if (!driver)
		return (EXIT_FAILURE);
	event_driver_run(driver);
	i = EV_KEY;
	run = true;
	printf("running: %zu threads\n", driver->thread_count);
	while (run)
	{
		sleep(1);
		event = (struct input_event *)event_driver_get(driver, &i);
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
