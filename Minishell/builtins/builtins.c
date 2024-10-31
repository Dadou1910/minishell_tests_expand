/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jealefev <jealefev@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/07 13:11:17 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/07 13:11:50 by jealefev         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	is_builtins(t_command *cmd, char **envp)
{
	if (strcmp(cmd->args[0], "echo") == 0)
		return (echo(cmd->args));
	else if (strcmp(cmd->args[0], "cd") == 0)
		return (cd(cmd->args));
	else if (strcmp(cmd->args[0], "pwd") == 0)
		return (pwd());
	else if (strcmp(cmd->args[0], "env") == 0)
		return (env(envp));
	else if (strcmp(cmd->args[0], "unset") == 0)
		return (unset(envp, cmd->args[1]));
	return (-1);
}
