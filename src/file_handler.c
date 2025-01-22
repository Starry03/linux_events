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
	filehandler->tid = (pthread_t *)malloc(sizeof(pthread_t));
	*filehandler->tid = tid;
	filehandler->fd = open(input_path, O_RDONLY);
	filehandler->event_multi_queue = event_multi_queue;
	filehandler->run = shared_rsc_init(malloc(sizeof(bool)), &free);
	shared_rsc_wait(filehandler->run);
	*(bool *)filehandler->run->data = true;
	shared_rsc_post(filehandler->run);
	if (filehandler->fd == -1)
	{
		free(filehandler->tid);
		free(filehandler);
		return (NULL);
	}
	return (filehandler);
}
static inline void	free_event(t_generic event)
{
	free((struct input_event *)event);
}

void	*filehandler_run(t_generic args)
{
	const t_filehandler	filehandler = (t_filehandler)args;
	const size_t		size = sizeof(struct input_event);
	const int			fd = filehandler->fd;
	struct input_event	event_buf;
	struct input_event	*event;
	ssize_t				ret;

	fcntl(fd, F_SETFL, O_NONBLOCK);
	memset(&event_buf, 0, size);
	while (true)
	{
		shared_rsc_wait(filehandler->run);
		if (!*(bool *)filehandler->run->data)
		{
			shared_rsc_post(filehandler->run);
			break ;
		}
		shared_rsc_post(filehandler->run);
		ret = read(fd, (t_generic)&event_buf, size);
		if (ret <= 0)
			continue ;
		shared_rsc_wait(filehandler->event_multi_queue);
		event = (struct input_event *)calloc(1, size);
		memcpy(event, &event_buf, size);
		if (!dict_add(filehandler->event_multi_queue->data, &(event->type),
				(t_generic)event, &free_event))
			free_event(event);
		shared_rsc_post(filehandler->event_multi_queue);
		memset(&event_buf, 0, size);
	}
	close(fd);
	return (NULL);
}

void	filehandler_free(t_generic handler)
{
	const t_filehandler filehandler = (t_filehandler)handler;
	free(filehandler->tid);
	free(filehandler->input_path);
	shared_rsc_free(filehandler->run);
	free(filehandler);
}