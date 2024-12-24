#include "events.h"
#include "unic/dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	t_event_driver	driver;
	t_linkedlist	bucket;
	t_dict_obj		obj;
	t_dict			dict;

	driver = event_driver_init("/dev/input/");
	if (!driver)
		return (EXIT_FAILURE);
	printf("initialized driver\n");
	printf("got %zu devices\n",
		linkedlist_size(driver->handler_manager->filehandlers));
	event_driver_run(driver);
	printf("generated %ld threads\n", driver->thread_count);
	if (!driver->thread_count)
		return (EXIT_FAILURE);
	// print events scanning 1st event in every dict bucket
	dict = driver->event_multi_queue->data;
	while (1)
	{
		shared_rsc_wait(driver->event_multi_queue);
		bucket = dict_get_buckets(dict);
		shared_rsc_post(driver->event_multi_queue);
		dict_status(dict);
		sleep(1);
	}
	event_driver_free(driver);
}
