/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:10:40 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/07 13:10:42 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	count_env_vars(char **envp)
{
	int	count;

	count = 0;
	while (envp[count] != NULL)
		count++;
	return (count);
}

int	unset(char ***envp, const char *var)
{
	int		count;
	char	**new_env;
	int		i;
	int		j;

	count = count_env_vars(*envp);
	new_env = malloc(sizeof(char *) * count);
	i = 0;
	j = 0;
	if (!new_env)
	{
		perror("malloc error");
		return (-1);
	}
	while (i < count)
	{
		if (strncmp((*envp)[i], var, strlen(var)) != 0
			|| (*envp)[i][strlen(var)] != '=')
			new_env[j++] = (*envp)[i];
		i++;
	}
	new_env[j] = NULL;
	free(*envp);
	*envp = new_env;
	return (0);
}
