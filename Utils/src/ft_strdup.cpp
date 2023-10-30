#include "../../include/includes.hpp"

char *ft_strdup(const char *s1)
{
	char *s2;
	int i;

	i = 0;
	if (!s1)
		return (NULL);
	while (s1[i])
		i++;
	s2 = new char[i + 1];
	i = 0;
	while (s1[i])
	{
		s2[i] = s1[i];
		i++;
	}
	s2[i] = s1[i];
	return (s2);
}
