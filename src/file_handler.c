#include "events.h"
#include "unic/dictionary.h"
#include "unic/hashfunctions.h"
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

t_filehandler	filehandler_init(t_string input_path, pthread_t tid,
		t_shared_rsc event_multi_queue)
{
	t_filehandler	filehandler;

	filehandler = (t_filehandler)malloc(sizeof(t_filehandler_));
	if (!filehandler)
		return (NULL);
	filehandler->input_path = input_path;
	filehandler->tid = tid;
	filehandler->fd = open(input_path, O_RDONLY);
	filehandler->event_multi_queue = event_multi_queue;
	if (filehandler->fd == -1)
	{
		free(filehandler);
		return (NULL);
	}
	return (filehandler);
}
void	free_event(t_generic event)
{
	printf("freeing event\n");
	free(event);
}

void	*filehandler_run(t_generic args)
{
	const t_filehandler	filehandler = (t_filehandler)args;
	const size_t		size = sizeof(struct input_event);
	const int			fd = filehandler->fd;
	struct input_event	event_buf;
	struct input_event	*event;

	while (true)
	{
		event_buf = (struct input_event){0};
		if (read(fd, (t_generic)&event_buf, size) == -1)
			break ;
		shared_rsc_wait(filehandler->event_multi_queue);
		event = (struct input_event *)calloc(1, size);
		memcpy(event, &event_buf, size);
		dict_add(filehandler->event_multi_queue->data, &(event->type),
			(t_generic)event, &free_event);
		shared_rsc_post(filehandler->event_multi_queue);
	}
	return (NULL);
}

void	filehandler_free(t_generic handler)
{
	const t_filehandler filehandler = (t_filehandler)handler;
	close(filehandler->fd);
	free(filehandler->input_path);
	free(filehandler);
}