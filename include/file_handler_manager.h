#ifndef FILE_HANDLER_MANAGER_H
# define FILE_HANDLER_MANAGER_H

# include "file_handler.h"
# include "unic/linkedlist.h"
# include "unic/shared_rsc.h"
# include <semaphore.h>

typedef struct s_filehandler_manager
{
	t_string					dev_path;
	t_linkedlist				filehandlers;
	t_shared_rsc				event_multi_queue;
}								t_filehandler_manager_;

typedef t_filehandler_manager_	*t_filehandler_manager;

t_filehandler_manager			filehandler_manager_init(t_string dev_path,
									t_shared_rsc event_multi_queue);
/**
 * @brief Get all file handlers from dev_path
 * @return last thread id spawned
 */
pthread_t						filehandler_manager_spawn(t_filehandler_manager manager);
void							filehandler_manager_free(t_filehandler_manager manager);

#endif