/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/17 18:59:17 by wpepping          #+#    #+#             */
/*   Updated: 2024/12/06 19:56:29 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>

static void	skip_word(const char *str, char c, int *pos)
{
	while (str[*pos] != '\0' && str[*pos] != c)
		*pos += 1;
}

static void	skip_separators(const char *str, char c, int *pos)
{
	while (str[*pos] != '\0' && str[*pos] == c)
		*pos += 1;
}

static char	*read_word(const char *str, char c, int *pos)
{
	int		i;
	int		len;
	char	*result;

	len = 0;
	while (str[*pos + len] != '\0' && str[*pos + len] != c)
		len++;
	result = malloc((len + 1) * sizeof(char));
	i = 0;
	while (i < len)
		result[i++] = str[(*pos)++];
	result[i] = '\0';
	return (result);
}

static int	count_parts(const char *str, char c)
{
	int	pos;
	int	result;

	pos = 0;
	result = 0;
	skip_separators(str, c, &pos);
	while (str[pos] != '\0')
	{
		result++;
		skip_word(str, c, &pos);
		skip_separators(str, c, &pos);
	}
	return (result);
}

char	**ft_split(char const *s, char c)
{
	int		n;
	int		i;
	int		pos;
	char	**result;

	n = count_parts(s, c);
	result = malloc((n + 1) * sizeof(char *));
	i = 0;
	pos = 0;
	while (i < n)
	{
		skip_separators(s, c, &pos);
		result[i] = read_word(s, c, &pos);
		i++;
	}
	result[i] = NULL;
	return (result);
}
