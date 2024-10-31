/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abremont <abremont@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 14:03:20 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/28 11:14:43 by abremont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <unistd.h>

void	add_line_to_history(const char *line)
{
	if (line && *line)
	{
		add_history(line);
	}
}

char	*get_prompt(void)
{
	char	cwd[1024];
	char	*tmp;

	tmp = NULL;
	if (getcwd(cwd, sizeof(cwd)) != NULL)
		tmp = ft_strjoin(cwd, " >> ");
	return (tmp);
}

void	print_command(t_command *cmd)
{
	int	i;

	i = 0;
	printf("Arguments: ");
	if (cmd->args)
	{
		while (cmd->args[i])
		{
			printf("%s\n", cmd->args[i]);
			i++;
		}
	}
	printf("\n");
	printf("fd_in: %d\n", cmd->fd_in);
	printf("fd_out: %d\n", cmd->fd_out);
	printf("Pipe read_end: %d, write_end: %d\n", cmd->p[0], cmd->p[1]);
	printf("Previous pipe input (pprev): %d\n", cmd->pprev);
	i = 0;
	if (cmd->token_quotes && cmd->token_quotes[i])
	{
		while (cmd->token_quotes[i])
		{
			printf("token_quotes[%d] : %s\n", i, cmd->token_quotes[i]);
			i++;
		}
	}
	else
		printf("cmd->token_quotes not initialized\n");
	if (cmd->next)
		printf("Next command exists.\n");
	else
		printf("No next command.\n");
}


void free_table_struct(t_table *table, char *line)
{
	if (table->pids)
		free(table->pids);
	free(line);
}

void	free_cmd(t_command *cmd)
{
	if(cmd->p[READ_END] && cmd->p[READ_END] > 0)
		close(cmd->p[READ_END]);
	if(cmd->p[READ_END] && cmd->p[READ_END] > 0)
		close(cmd->p[WRITE_END]);
	if(cmd->fd_in && cmd->fd_in > 0 )
		close(cmd->fd_in);
	else if(cmd->fd_out && cmd->fd_out > 0)
		close(cmd->fd_out);
	int i = 0;
	while(cmd->args[i])
	{
		free(cmd->args[i]);
		i++;
	}
	if (cmd->args)
		free(cmd->args);
	if (cmd->token_quotes)
		freetab(cmd->token_quotes);
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_command	*cmd;

	line = NULL;
	(void)argc;
	(void)argv;
	(void)envp;
	while (1)
	{
		line = readline(get_prompt());
		add_line_to_history(line);
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		cmd = fill_t_command(line);
		print_command(cmd);
		launch(cmd, envp);
		free_table_struct(cmd->table, line);
	}
	return (0);
}
