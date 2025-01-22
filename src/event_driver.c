#include "events.h"
#include "unic/comparators.h"
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

/**
 * @brief run the event driver
 * @note this function will spawn the file handlers and start the event driver
 */
void	event_driver_run(t_event_driver driver)
{
	driver->thread_count = filehandler_manager_spawn(driver->handler_manager);
}

/**
 * @brief get the last event of a certain type
 *
 * @param driver the event driver
 * @param event_type the type of the event to get
 *
 * @return the last event of the type
 */
t_generic	event_driver_get(t_event_driver driver, uint16_t *event_type)
{
	t_dict		dict;
	t_generic	event;

	shared_rsc_wait(driver->event_multi_queue);
	dict = (t_dict)driver->event_multi_queue->data;
	event = dict_get(dict, event_type);
	shared_rsc_post(driver->event_multi_queue);
	return (event);
}

/**
 * @brief clean last event from the event queue after "event_driver_get"
 * @note not doing that will cause the last got event to be returned again
 *
 * @param driver the event driver
 * @param event the event to clean
 *
 * @return true if an event was cleaned, false otherwise
 */
bool	event_driver_clean_get(t_event_driver driver, struct input_event *event)
{
	t_dict	dict;

	if (!driver || !event)
		return (false);
	shared_rsc_wait(driver->event_multi_queue);
	dict = (t_dict)driver->event_multi_queue->data;
	if (!dict_remove(dict, (t_generic)&event->type))
		return (false);
	shared_rsc_post(driver->event_multi_queue);
	return (true);
}

void	event_driver_free(t_event_driver driver)
{
	filehandler_manager_free(driver->handler_manager);
	shared_rsc_free(driver->event_multi_queue);
	free(driver);
}