#ifndef EVENTS_H
# define EVENTS_H

# include "file_handler.h"
# include "file_handler_manager.h"

typedef struct s_event_driver
{
	t_string				DEV_PATH;
	t_filehandler_manager	handler_manager;
	t_shared_rsc			event_multi_queue;
	pthread_t				thread_count;
}							t_event_driver_;

typedef t_event_driver_		*t_event_driver;

t_event_driver				event_driver_init(t_string DEV_PATH);
void						event_driver_run(t_event_driver driver);
void						event_driver_free(t_event_driver driver);

int							cmp_event(const t_generic a, const t_generic b);
size_t						hash_event_key(t_generic event, size_t size);

#endif
