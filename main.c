#include "events.h"
#include "unic/dictionary.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int	main(void)
{
	t_event_driver	driver;
	t_linkedlist	*buckets;
	t_linkedlist	bucket;
	t_dict_obj		dict_obj;
	struct input_event		event;
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
	dict = driver->event_multi_queue->data;
	buckets = dict_get_buckets(dict);
	while (1)
	{
		shared_rsc_wait(driver->event_multi_queue);
		for (size_t i = 0; i < dict->size; i++)
		{
			bucket = buckets[i];
			while (bucket)
			{
				dict_obj = (t_dict_obj)(linkedlist_getinfo(bucket));
				event = *(struct input_event *)dict_obj->value;
				printf("event[%zu]: %zu -> %d\n", i, *(size_t *)dict_obj->key, event.code);
				bucket = linkedlist_getnext(bucket);
			}
		}
		dict_status(dict);
		shared_rsc_post(driver->event_multi_queue);
		sleep(1);
	}
	event_driver_free(driver);
}
