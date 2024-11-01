/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abremont <abremont@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/24 12:16:22 by abremont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	find_fd_in(t_command *cmd)
{
	int	i;
	int	fd;

	i = 1;
	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], "<") == 0)
		{
			if (cmd->fd_in != -1)
				close(cmd->fd_in);
			fd = open_fct_check(cmd->args[i + 1], 0);
			if (fd == -1)
			{
				perror("Error opening input file");
				return (1);
			}
			cmd->fd_in = fd;
			i++;
		}
		i++;
	}
	return (0);
}

int	helper_fd_out(t_command *cmd, char *file, int flags)
{
	int	fd;

	if (cmd->fd_out != -2)
		close(cmd->fd_out);
	fd = open_fct_check(file, flags);
	if (fd == -1)
		return (perror("Error opening output file"), 1);
	cmd->fd_out = fd;
	return (0);
}

int	find_fd_out(t_command *cmd)
{
	int	i;

	i = 1;
	while (cmd->args[i])
	{
		if (ft_strcmp(cmd->args[i], ">") == 0)
			if (helper_fd_out(cmd, cmd->args[++i], 1))
				return (1);
		if (ft_strcmp(cmd->args[i], ">>") == 0)
			if (helper_fd_out(cmd, cmd->args[++i], 2))
				return (1);
		i++;
	}
	return (0);
}

void	change_args_for_exec(t_command *cmd)
{
	t_command	*head;
	char		**new_args;
	int			i;
	int			j;

	head = cmd;
	while (head)
	{
		i = 0;
		while (head->args[i] && (head->args[i][0] != '|'
				&& head->args[i][0] != '<' && head->args[i][0] != '>'))
			i++;
		new_args = malloc(sizeof(char *) * (i + 1));
		if (!new_args)
			return (perror("Error with malloc"));
		j = -1;
		while (++j < i)
		{
			new_args[j] = ft_strdup(head->args[j]);
			printf("new_args : %s\n", new_args[j]);
		}
		new_args[j] = NULL;
		freetab(head->args);
		head->args = new_args;
		head = head->next;
	}
}

t_command	*fill_t_command(char *input)
{
	t_command	*cmd;

	(void)input;
	cmd = tokenize_input(input);
	if (!cmd)
		return (NULL);
	// if (find_fd_in(cmd))
	// {
	// 	free_t_command(cmd);
	// 	return (NULL);
	// }
	// if (find_fd_out(cmd))
	// {
	// 	free_t_command(cmd);
	// 	return (NULL);
	// }
	change_args_for_exec(cmd);
	return (cmd);
}
