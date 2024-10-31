/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abremont <abremont@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/21 13:12:20 by abremont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

t_state	*initialize_t_state()
{
	t_state	*state;

	state = malloc(sizeof(t_state));
	ft_bzero(state->n, sizeof(int) * 2);
	state->i = 0;
	state->dq_open = 0;
	state->sq_open = 0;
	return (state);
}
t_command	*initialize_t_command(t_table *table)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	cmd->args = malloc(sizeof(char *) * _SC_ARG_MAX);
	if (!cmd->args)
	{
		perror("malloc failed");
		exit(EXIT_FAILURE);
	}
	cmd->fd_in = -2;
	cmd->fd_out = -2;
	cmd->pprev = -2;
	cmd->table = table;
	cmd->next = NULL;
	cmd->token_quotes = NULL;
	return (cmd);
}

int	ft_count_pipe(char *str)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] == '|')
			count++;
		i++;
	}
	return (count + 1);
}

t_table	*initialize_t_main(char *line)
{
	t_table	*table;
	int		i;

	table = malloc(sizeof(t_table));
	table->num_pids = ft_count_pipe(line);
	table->pids = malloc(sizeof(int) * table->num_pids + 1);
	i = 0;
	while (i < table->num_pids)
	{
		table->pids[i] = -1;
		i++;
	}
	table->ipids = 0;
	return (table);
}

int	open_fct_check(char *file, int i)
{
	int	filein;

	filein = 0;
	if (i == 0)
		filein = open(file, O_RDONLY);
	else if (i == 1)
		filein = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
	else if (i == 2)
		filein = open(file, O_WRONLY | O_CREAT | O_APPEND, 0644);
	return (filein);
}

void	free_t_command(t_command *cmd)
{
	t_command	*temp;

	while (cmd)
	{
		temp = cmd->next;
		freetab(cmd->args);
		free(cmd->table->pids);
		free(cmd);
		cmd = temp;
	}
}

void	new_t_command(t_command *cmd, t_table *table)
{
	t_command	*new_cmd;

	new_cmd = initialize_t_command(table);
	cmd->next = new_cmd;
}
