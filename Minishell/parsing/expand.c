/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abremont <abremont@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/04 01:25:41 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/31 13:44:33 by abremont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expand_env_var_1(const char *input, t_state *state)
{
	char	*env;

	env = NULL;
	if (!ft_strncmp(&input[state->i + 1], "PATH", 4))
	{
		env = getenv("PATH");
		state->n[0] += 4;
		state->i += 4;
	}
	else if (!ft_strncmp(&input[state->i + 1], "HOME", 4))
	{
		env = getenv("HOME");
		state->n[0] += 4;
		state->i += 4;
	}
	else if (!ft_strncmp(&input[state->i + 1], "PWD", 3))
	{
		env = getenv("PWD");
		state->n[0] += 3;
		state->i += 3;
	}
	else if (!ft_strncmp(&input[state->i + 1], "OLDPWD", 6))
	{
		env = getenv("OLDPWD");
		state->n[0] += 6;
		state->i += 6;
	}
	return (env);
}

char	*expand_env_var_2(const char *input, t_state *state)
{
	char	*env;

	env = NULL;
	if (!ft_strncmp(&input[state->i + 1], "SHLVL", 5))
	{
		env = getenv("SHLVL");
		state->n[0] += 5;
		state->i += 5;
	}
	else if (!ft_strncmp(&input[state->i + 1], "_", 1))
	{
		env = getenv("_");
		state->n[0] += 1;
		state->i += 1;
	}
	else if (!ft_strncmp(&input[state->i + 1], "USER", 4))
	{
		env = getenv("USER");
		state->n[0] += 4;
		state->i += 4;
	}
	else if (!ft_strncmp(&input[state->i + 1], "TERM", 4))
	{
		env = getenv("TERM");
		state->n[0] += 4;
		state->i += 4;
	}
	return (env);
}

void	allocate_resources(char **result, t_state **state)
{
	*result = malloc(sizeof(char) * 4096);
	if (!(*result))
	{
		perror("Error allocating memory for result");
		exit(1);
	}
	(*result)[0] = '\0';
	*state = malloc(sizeof(t_state));
	if (!(*state))
	{
		perror("Error allocating memory for state");
		free(*result);
		exit(1);
	}
	ft_bzero((*state)->n, sizeof(int) * 2);
	(*state)->i = 0;
	(*state)->sq_open = 0;
	(*state)->dq_open = 0;
}

char	*process_char_helper(const char *input, char *result, t_state *state)
{
	char	*env;
	size_t	result_size;
	char	temp[2];

	result_size = 4096;
	env = NULL;
	// printf("input[state->i] : %c\n", input[state->i]);
	// printf("state->i in char_helper = %d\n", state->i);
	// printf("input in char_helper = %s\n", input);
	if (!result)
	{
		result = malloc(result_size);
		result[0] = '\0';
	}
	if (!result)
	{
		perror("Memory allocation failed for result");
		exit(1);
	}
	printf("input : %s\n", input);
	printf("input[state->i] = %c\n", input[state->i]);
	printf("state->i : %d\n", state->i);
	while (input[state->i])
	{
		temp[0] = input[state->i];
		temp[1] = '\0';
		if (input[state->i] == '\'' && !state->dq_open)
		{
			// printf("A\n");
			state->sq_open = !state->sq_open;
			ft_strlcat(result, "\'", result_size);
		}
		else if (input[state->i] == '\"' && !state->sq_open)
		{
			printf("enters double quote block in helper\n");
			state->dq_open = !state->dq_open;
			ft_strlcat(result, "\"", result_size);
		}
		else if (input[state->i] == '$' && (!state->sq_open && !state->dq_open))
		{
			//printf("--->sq in process : %d\n", state->sq_open);
			// printf("Entered expand in char_helper\n");
			env = expand_env_var_1(input, state);
			if (!env)
			{
				env = expand_env_var_2(input, state);
			}
			// printf("env : %s\n", env);
			if (env != NULL)
			{
				printf("RESULT : %s\n", result);
				ft_strlcat(result, env, result_size);
				printf("result in env block : %s\n", result);
				break ;
			}
		}
		else if (state->dq_open == 0)
		{
			state->n[0] = state->i + 1;
			return (result);
		}
		else
		{
			printf("enters manual caracter adding in helper\n");
			printf("printing caracter : %c\n", temp[0]);
			ft_strlcat(result, temp, result_size);
		}
		state->i++;
	}
	state->i = 0;
	state->sq_open = 0;
	state->dq_open = 0;
	printf("state->dq_open at the end of helper : %d\n", state->dq_open);
	printf("result at the end of helper : %s\n", result);
	return (result);
}

char	*process_char(char *input, char *result, t_state *state, t_command *cmd)
{
	int	i;

	i = 0;
	allocate_resources(&result, &state);
	handle_quotes_and_expand(input, result, cmd, state);
	return (result);
}

void handle_quotes_and_expand(char *arg, char *result, t_command *cmd, t_state *state)
{
    char    *quotes[4096];
    char    *str;
    
    ft_bzero(quotes, sizeof(char *) * 4096);
	printf("n[0] at the beginning of hanle quotes : %d\n", state->n[0]);
    while (arg[state->n[0]])
    {
		printf("n[1] = %d\n", state->n[1]);
		printf("n[0] = %d\n", state->n[0]);
        if (arg[state->n[0]] == '\'')
        {
			//printf("n[0] in main function = %d\n", state->n[0]);
			printf("Single quotes encountered\n");
            state->sq_open = !state->sq_open;
			printf("--->SQ open : %d\n", state->sq_open);
            if (state->sq_open)
            {
                str = helper_quotes(arg, '\'', state);
                if (str == NULL)
                    break;
                quotes[state->n[1]] = ft_strdup(str);
                // printf("--->quotes : %s\n", quotes[state->n[1]]);
        		if (str[0] != '\0' && str)
					state->n[1]++;
            	state->sq_open = 0;
                free(str);
            }
			// printf("arg[(n[0])] end of block : %c\n", arg[(state->n[0])]);
			// printf("n[0] in main function after helper = %d\n", state->n[0]);
			printf("--->sq open end of block : %d\n", state->sq_open);
        }
        else if (arg[state->n[0]] == '\"')
        {
			printf("Double quotes encountered\n");
            state->dq_open = !state->dq_open;
			printf("--->DQ open : %d\n", state->dq_open);
            if (state->dq_open)
			{
				str = helper_quotes(arg, '\"', state);
				if (str == NULL)
					break;
				result = process_char_helper(str, result, state);
				printf("result in double quotes block : %s\n", result);
				if (result[0] != '\0' && result)
				{
					printf("enters quotes creation block\n");
					quotes[state->n[1]] = ft_strdup(result);
					printf("n[1] = %d\n", state->n[1]);
					printf("quotes[state->n[1]] : %s\n", quotes[state->n[1]]);
					printf("quotes[0] up : %s\n", quotes[0]);
					state->n[1]++;
            		state->dq_open = 0;
					result = NULL;
				}
				free(str);
			}
        }
        else if (arg[state->n[0]] == '$' && !state->sq_open)
         {
		 	printf("n[0] in $ else if block = %d\n", state->n[0]);
		 	printf("$ encountered\n");
		 	printf("--->sq : %d\n", state->sq_open);
			str = helper_quotes(arg, '\"', state);
			if (str == NULL)
				break;
		 	result = process_char_helper(str, result, state);
		 	if (result)
		 	{
		 		quotes[state->n[1]] = ft_strdup(result);
				state->n[1]++;
				printf("quotes[0] up : %s\n", quotes[0]);
		 		result = NULL;
		 	}
			free(str);
			printf("coming out of $ block\n");
        }
        else if (!ft_strchr(arg, '$'))
        {
			printf("enters manual add block\n");
			quotes[state->n[1]] = malloc(2);
            if (!quotes[state->n[1]])
            {
                perror("Memory allocation failed");
                exit(1);
            }
            quotes[state->n[1]][0] = arg[state->n[0]];
            quotes[state->n[1]][1] = '\0'; 
        	state->n[1]++;
		}
        if (state->n[1] >= 4096)
        {
            perror("Too many quoted strings");
            exit(1);
        }
		// printf("n[0] in end of function = %d\n", state->n[0]);
		// printf("quotes[n[1]] : %s\n", quotes[state->n[1]]);
        state->n[0]++;
        //state->n[1]++;
	}
    cmd->token_quotes = malloc(sizeof(char *) * (state->n[1] + 1));
	printf("token_quotes malloc\n");
    if (!cmd->token_quotes)
    {
        perror("Error allocating memory for token_quotes");
        exit(1);
    }
	printf("quotes[0] : %s\n", quotes[0]);
	copy_quote_strings(cmd, quotes, state);
	// printf("cmd->token_quotes[state->n[1]] : %s\n", cmd->token_quotes[state->n[1] - 1]);
	//ft_bzero(state->n, 2);
}



