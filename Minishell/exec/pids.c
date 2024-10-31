#include "../includes/minishell.h"

void	wait_pids(t_command *cmd)
{
	int	i;

	i = 0;
	while (cmd)
	{
		wait(&cmd->table->pids[i]);
		i++;
		cmd = cmd->next;
	}
}

int	launch(t_command *cmd, char **envp)
{
	t_command *head;

	head = cmd;
	while (cmd && cmd->args[0])
	{
		if (is_builtins(cmd, envp) == -1)
			execute_cmd(cmd, envp);
		cmd->table->ipids++;
		cmd = cmd->next;
	}
	wait_pids(cmd);
	free_cmd(head);
	return (1);
}
