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
	t_dict				dict;
	uint16_t			i;

	driver = event_driver_init("/dev/input/");
	if (!driver)
		return (EXIT_FAILURE);
	i = EV_KEY;
	run = true;
	printf("initialized driver\n");
	printf("got %zu devices\n",
		linkedlist_size(driver->handler_manager->filehandlers));
	event_driver_run(driver);
	printf("generated %ld threads\n", driver->thread_count);
	if (!driver->thread_count)
		return (EXIT_FAILURE);
	if (!driver->event_multi_queue || !driver->event_multi_queue->data)
		return (EXIT_FAILURE);
	shared_rsc_wait(driver->event_multi_queue);
	dict = driver->event_multi_queue->data;
	if (!dict)
		return (EXIT_FAILURE);
	shared_rsc_post(driver->event_multi_queue);
	while (run)
	{
		sleep(1);
		shared_rsc_wait(driver->event_multi_queue);
		event = (struct input_event *)dict_get(dict, &i);
		if (!event)
		{
			shared_rsc_post(driver->event_multi_queue);
			continue ;
		}
		printf("type: %d, code: %d, value: %d\n", event->type, event->code,
			event->value);
		if (event->code == KEY_ESC)
			run = false;
		dict_remove(dict, &i);
		shared_rsc_post(driver->event_multi_queue);
		dict_status(dict);
	}
	event_driver_free(driver);
}
