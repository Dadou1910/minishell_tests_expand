/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/07 14:04:40 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdlib.h>

void	freetab(char **tab)
{
	int	i;

	if (tab)
	{
		i = 0;
		while (tab[i])
		{
			free(tab[i]);
			i++;
		}
		free(tab);
	}
}

char	*find_path(char *cmd)
{
	char	*path;
	char	**pathoche;
	char	*res;
	char	*tmp;
	int		i;

	i = 0;
	path = getenv("PATH");
	if (!path)
		return (NULL);
	pathoche = ft_split(path, ':');
	while (pathoche[i])
	{
		tmp = ft_strjoin(pathoche[i], "/");
		if (!tmp)
			return (freetab(pathoche), NULL);
		res = ft_strjoin(tmp, cmd);
		free(tmp);
		if (!res)
			return (freetab(pathoche), NULL);
		if (access(res, X_OK) == 0)
		{
			freetab(pathoche);
			return (res);
		}
		free(res);
		i++;
	}
	freetab(pathoche);
	return (NULL);
}

void	execute(t_command *cmd, char **envp)
{
	char	*pathoche;

	if (!cmd || !cmd->args || !cmd->args[0])
		perror("invalid command");
	if (ft_strncmp(cmd->args[0], "/", 1) == 0)
		pathoche = cmd->args[0];
	else
		pathoche = find_path(cmd->args[0]);
	if (!pathoche)
		perror("command not found");
	if (execve(pathoche, cmd->args, envp) == -1)
	{
		free(pathoche);
		perror("command execution failed");
	}
}
