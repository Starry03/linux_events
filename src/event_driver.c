#include "events.h"
#include "unic/dictionary.h"
#include <pthread.h>
#include <stdlib.h>

t_event_driver	event_driver_init(t_string DEV_PATH)
{
	t_event_driver	driver;

	driver = (t_event_driver)malloc(sizeof(t_event_driver_));
	if (!driver)
		return (NULL);
	driver->DEV_PATH = DEV_PATH;
	driver->event_multi_queue = shared_rsc_init((t_generic)dict_init(0,
				&cmp_event, &hash_event_key, NULL), &dict_free);
	driver->handler_manager = filehandler_manager_init(DEV_PATH,
			driver->event_multi_queue);
	if (!driver->handler_manager)
	{
		free(driver);
		return (NULL);
	}
	return (driver);
}

void	event_driver_run(t_event_driver driver)
{
	driver->thread_count = filehandler_manager_spawn(driver->handler_manager);
	// for (pthread_t t = 0; t < driver->thread_count; t++)
	// 	pthread_join(t, NULL);
}
void	event_driver_free(t_event_driver driver)
{
	filehandler_manager_free(driver->handler_manager);
	shared_rsc_free(driver->event_multi_queue);
	free(driver);
}