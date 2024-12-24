#include "events.h"
#include "unic/dictionary.h"
#include "unic/hashfunctions.h"
#include <fcntl.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>

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

void	*filehandler_run(t_generic args)
{
	const t_filehandler			filehandler = (t_filehandler)args;
	const size_t				size = sizeof(struct input_event);
	const int					fd = filehandler->fd;
	const struct input_event	*ev = &filehandler->ev;

	memset((t_generic)ev, 0, size);
	while (true)
	{
		if (read(fd, (t_generic)ev, size) == -1)
			break ;
		shared_rsc_wait(filehandler->event_multi_queue);
		dict_add(filehandler->event_multi_queue->data,
			(t_generic)&filehandler->ev.type, (t_generic)&filehandler->ev,
			NULL);
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