#include "events.h"
#include "unic/comparators.h"
#include "unic/hashfunctions.h"
#include <string.h>

int	cmp_event(const t_generic a, const t_generic b)
{
	return (cmp_uint16(a, b));
}

size_t	hash_event_key(t_generic event_key, size_t size)
{
	// events will be stored grouped by type -> poor hash function to group them
	size_t temp = 0;

	memcpy(&temp, event_key, sizeof(uint16_t));
	return (hash_generic(temp, size));
}