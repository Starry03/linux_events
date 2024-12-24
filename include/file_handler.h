#ifndef FILE_HANDLER_H
# define FILE_HANDLER_H

# include "unic/shared_rsc.h"
# include "unic/types.h"
# include <linux/input.h>
# include <pthread.h>

typedef struct s_filehandler
{
	pthread_t			tid;
	int					fd;
	struct input_event	ev;
	t_shared_rsc		event_multi_queue;
	t_string			input_path;
}						t_filehandler_;

typedef t_filehandler_	*t_filehandler;

t_filehandler			filehandler_init(t_string input_path, pthread_t tid,
							t_shared_rsc event_multi_queue);
/**
 * @brief Run the filehandler (own thread)
 * @param filehandler
 * @return void
 */
void					*filehandler_run(t_generic args);
void					filehandler_free(t_generic);

#endif // FILE_HANDLER_H