/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:10:19 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/14 12:18:40 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	execute_cmd(t_command *cmd, char **envp)
{
	if (pipe(cmd->p) == -1)
		perror("pipe");
	cmd->table->pids[cmd->table->ipids] = fork();
	if (cmd->table->pids[cmd->table->ipids] == 0)
	{
		print_command(cmd);
		deal_in_pipe(cmd);
		deal_out_pipe(cmd);
		execute(cmd, envp);
		perror("execute");
	}
	else if (cmd->table->pids[cmd->table->ipids] < 0)
	{
		perror("fork");
	}
	return (0);
}
