/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokens.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abremont <abremont@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/31 13:46:56 by abremont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char    *helper_quotes(char *arg, char c, t_state *state)
{
	char	*str;
	int		i;
	int		j;
	
	i = 0;
	j = state->n[0];
	if (arg[j] != '$')
		j++;
	str = malloc(1024);
	printf("enters helper_quotes\n");
	//state->n[0]++;
	// printf("n[0] before str creation = %d\n", state->n[0]);
	while (arg[j] != c && arg[j])
	{
		str[i]= arg[j];
		// printf("n[0] in helper = %d\n", state->n[0]);
		// printf("arg[(n[0])] in str: %c\n", arg[state->n[0]]);
		i++;
		j++;
	}
	printf("IN HELPER_QUOTES arg[%d] : %c\n", j, arg[j]);
	// if (arg[state->n[0]] == c)
	str[i] = '\0';
	if (!str)
		return (NULL);
	if (!ft_strchr(str, '$') || state->sq_open
		|| (ft_strchr(str, '$') && state->dq_open))
		state->n[0] = j;
	printf("str in helper_quotes : %s\n", str);
	return (str);
}

void	copy_quote_strings(t_command *cmd, char *quotes[4096], t_state *state)
{
	int	i;

	i = 0;
	while (quotes[i])
    {
		cmd->token_quotes[i] = malloc(1024);
		if (!cmd->token_quotes[i])
		{
			perror("Error allocating memory for token_quotes");
			exit(1);
		}
        cmd->token_quotes[i] = ft_strdup(quotes[i]);
		i++;
    }
    cmd->token_quotes[state->n[1]] = NULL;
}

char	*copy_quotes_to_arg(t_command *cmd, int *i)
{
	int	total_size;
	int	j;
	
	j = 0;
	total_size = 0;
    while (cmd->token_quotes[j])
        total_size += strlen(cmd->token_quotes[j++]);
    free(cmd->args[(*i)]);
    cmd->args[(*i)] = malloc(total_size + 1);
    if (!cmd->args[(*i)])
    {
        perror("Error allocating memory for concatenated args");
        exit(1);
    }
    cmd->args[(*i)][0] = '\0';
	j = 0;
    while (cmd->token_quotes[j])
	{
        ft_strlcat(cmd->args[(*i)], cmd->token_quotes[j++], total_size + 1);
		printf("cmd->args[*i] : %s\n", cmd->args[*i]);
	}
	cmd->args[++(*i)] = NULL;
	return (cmd->args[*i]);
}

void	process_quotes(t_command *cmd, t_state *state)
{
	int	i;

	i = 0;
	while (cmd->args[i])
	{
		if (ft_strchr(cmd->args[i], '\'') || ft_strchr(cmd->args[i], '\"')
			|| ft_strchr(cmd->args[i], '$'))
		{
			cmd->args[i] = process_char(cmd->args[i], cmd->args[i], state, cmd);
			break ;
		}
		i++;
	}
	cmd->args[i] = copy_quotes_to_arg(cmd, &i);
}

void	process_tokens(char **tokens, t_command *cmd, int *j)
{
	int	i;
	char	*result;
	t_state	*state;

	i = 0;
	state = NULL;
	state = initialize_t_state(state);
	result = NULL;
	cmd->args = malloc(sizeof(char *) * 1024);
	if (!cmd->args)
	{
		perror("Error allocating memory for args");
		exit(1);
	}
	while (tokens[*j] && ft_strcmp(tokens[*j], "|"))
	{
		cmd->args[i] = ft_strdup(tokens[*j]);
		if (!cmd->args[i])
		{
			perror("Error duplicating token");
			exit(1);
		}
		i++;
		(*j)++;
	}
	cmd->args[i] = NULL;
	i = 0;
	while (tokens[i])
	{
		if (ft_strchr(tokens[i], '$'))
		{
			if (ft_strchr(tokens[i], '\"') || ft_strchr(tokens[i], '\''))
			{
				process_quotes(cmd, state);
			}
			else
			{
				free(cmd->args[i]);
				cmd->args[i] = process_char_helper(tokens[i], result, state);
			}
		}
		i++;
	}
	if (find_fd_in(cmd) || find_fd_out(cmd))
	{
		//free_t_command(cmd);
		return;
	}
}

t_command	*tokenize_input(char *input)
{
	char		**tokens;
	t_table		*table;
	t_command	*cmd;
	t_command	*head;
	int			count;
	int			j;

	table = initialize_t_main(input);
	cmd = initialize_t_command(table);
	count = 0;
	j = 0;
	if (!input)
		return (NULL);
	if (!cmd)
	{
		perror("Error allocating memory");
		return (NULL);
	}
	ft_bzero(cmd, sizeof(cmd));
	head = cmd;
	tokens = ft_split(input, ' ');
	while (tokens[j])
	{
		process_tokens(tokens, cmd, &j);
		if (tokens[j] && ft_strcmp(tokens[j], "|") == 0)
		{
			count++;
			new_t_command(cmd, table);
			cmd = cmd->next;
			j++;
		}
	}
	freetab(tokens);
	return (head);
}
