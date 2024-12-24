#include "events.h"
#include "unic/hashfunctions.h"
#include <stdio.h>

int	cmp_event(const t_generic a, const t_generic b)
{
	const struct input_event	ev_a = *(struct input_event *)a;
	const struct input_event	ev_b = *(struct input_event *)b;

	return (int)(ev_a.time.tv_sec - ev_b.time.tv_sec);
}
size_t	hash_event_key(t_generic event_key, size_t size)
{
	// events will be stored grouped by type -> poor hash function to group them

	return (hash_int(event_key, size));
}