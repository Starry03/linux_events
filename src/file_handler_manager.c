#include "file_handler_manager.h"
#include "libft/libft.h"
#include <dirent.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef DT_CHR
# define DT_CHR 2
#endif

static t_string	join_path(t_string dir, t_string name)
{
	return (ft_strjoin(dir, name));
}

static t_linkedlist	get_handlers(t_string path, t_shared_rsc event_multi_queue)
{
	DIR				*dir;
	struct dirent	*entry;
	t_linkedlist	file_handlers;
	t_filehandler	handler;

	file_handlers = LINKEDLIST_EMPTY;
	dir = opendir(path);
	if (!dir)
		return (file_handlers);
	while ((entry = readdir(dir)))
	{
		if (entry->d_type != DT_CHR)
			continue ;
		handler = filehandler_init(join_path(path, entry->d_name), 0,
				event_multi_queue);
		if (handler)
			linkedlist_push(&file_handlers, handler);
	}
	closedir(dir);
	return (file_handlers);
}

t_filehandler_manager	filehandler_manager_init(t_string dev_path,
		t_shared_rsc event_multi_queue)
{
	t_filehandler_manager	manager;

	manager = (t_filehandler_manager)malloc(sizeof(t_filehandler_manager_));
	if (!manager)
		return (NULL);
	manager->dev_path = dev_path;
	manager->event_multi_queue = event_multi_queue;
	manager->filehandlers = get_handlers(dev_path, event_multi_queue);
	return (manager);
}

pthread_t	filehandler_manager_spawn(t_filehandler_manager manager)
{
	t_linkedlist	node;
	t_filehandler	handler;
	pthread_t		*tids;
	pthread_t		i;

	i = 0;
	node = manager->filehandlers;
	tids = (pthread_t *)calloc(linkedlist_size(node), sizeof(pthread_t));
	while (node)
	{
		handler = (t_filehandler)linkedlist_getinfo(node);
		tids[i] = i;
		handler->tid = tids[i];
		pthread_create(tids + i, NULL, &filehandler_run, (t_generic)handler);
		node = linkedlist_getnext(node);
		i++;
	}
	free(tids);
	return (i);
}

void	filehandler_manager_free(t_filehandler_manager manager)
{
	shared_rsc_free(manager->event_multi_queue);
	linkedlist_dealloc(manager->filehandlers, &filehandler_free);
}