#include"../includes/ft_irc.hpp"

size_t	ft_wcount(char const *s, char c)
{
	size_t	i;
	size_t	r;

	i = 0;
	r = 0;
	if (!s)
		return (-1);
	if (s[i] == 0)
		return (0);
	while (s[i] == c)
		i++;
	while (s[i])
	{
		if (s[i] == c)
		{
			r++;
			while (s[i] == c)
				i++;
		}
		else
			i++;
	}
	if (s[i - 1] != c && s[i] == 0 && i > 0)
		r++;
	return (r);
}

char	*ft_writetab(char *p, char c, char **p2)
{
	size_t	i;
	size_t	j;
	char	*line;

	i = 0;
	j = 0;
	while (*p == c)
		p++;
	while (p[i] != c && p[i] != 0)
		i++;
	line = (char *)malloc(sizeof(char) * (i + 1));
	if (line == NULL)
		return (0);
	while (j < i)
	{
		line[j] = p[j];
		j++;
	}
	line[j] = 0;
	while (p[i] == c && p[i + 1])
		i++;
	*p2 = &p[i];
	return (line);
}

void	ft_freetab(char **tab, int i)
{
	while (i >= 0)
	{
		free(tab[i]);
		i--;
	}
	free(tab);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	char	*p;
	char	**tab;
	size_t	ct;

	ct = ft_wcount(s, c);
	tab = (char **)malloc(sizeof(char *) * (ct + 1));
	if (!tab || (ct + 1) == 0)
		return (NULL);
	i = -1;
	p = (char *)s;
	while (++i < ct)
	{
		tab[i] = ft_writetab(p, c, &p);
		if (tab[i] == NULL)
		{
			ft_freetab(tab, i);
			break ;
		}
	}
	tab[i] = NULL;
	return (tab);
}

int ispass(int c) {
	if ((c <= '0' || c >= '9') && (c <= 'a' || c >= 'z') && (c <= 'A' || c >= 'Z')
		&& c != '!' && c != '@'&& c != '#' && c != '$' && c != '%'&& c != '&')
		return (0);
	return(1);
}

char	*ft_strjoin(char const *s1, char const *s2)
{
	size_t	i;
	size_t	j;
	char	*dup;

	if (!s1 || !s2)
		return (NULL);
	dup = (char *)malloc(strlen(s1) + strlen(s2) + 1);
	if (!dup)
		return (NULL);
	i = -1;
	j = -1;
	while (s1[++i])
		dup[i] = s1[i];
	i--;
	while (s2[++j])
		dup[++i] = s2[j];
	dup[i + 1] = '\0';
	return (dup);
}