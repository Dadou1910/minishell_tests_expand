/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/07 10:55:04 by jealefev          #+#    #+#             */
/*   Updated: 2024/05/19 11:24:37 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnstr(const char *str, const char *to_find, size_t nb)
{
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	if (nb < i)
		return (NULL);
	if (!to_find || str[0] == '\0')
		return ((char *)str);
	if (to_find[0] == '\0')
		return ((char *)str);
	while (str[i])
	{
		while (str[i + j] == to_find[j] && (i + j) < nb)
		{
			j++;
			if (to_find[j] == '\0')
				return ((char *)str + i);
		}
		j = 0;
		i++;
		if (j == nb)
			return (NULL);
	}
	return (NULL);
}
// #include <bsd/string.h>
// int main(void)
// {
// 	char haystack[30] = "aaabcabcd";
// 	char needle[10] = "aabc";
// 	printf("%s\n", ft_strnstr(haystack, "cd", 8));
// 	printf("%s\n", strnstr(haystack, "cd", 8));
// 	return (0);
// }
