/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abremont <abremont@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/25 14:01:59 by jealefev          #+#    #+#             */
/*   Updated: 2024/10/31 12:56:31 by abremont         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../Libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <sys/wait.h>
# include <unistd.h>

# define READ_END 0
# define WRITE_END 1
typedef struct s_table
{
	int		num_pids;
	int		*pids;
	int		ipids;
}			t_table;

typedef struct s_command
{
	char **args;            // Command  + command arguments (e.g., ["-l",
							// "/home"])
	int fd_in;              // File descriptor of the input file in the block
	int fd_out;             // File descriptor of the output file in the block
	int p[2];               // 0 or 1 for read_end and write_end
	int pprev;              // to redirect stdin to read_end of pipe
	struct s_command *next; // Pointer to the next command (for pipelines)
	char	**token_quotes; // handles the quotes for the expand
	t_table	*table;
}			t_command;

typedef struct s_state
{
	int		n[2];
	int		i;
	int		sq_open;
	int		dq_open;
}			t_state;

extern int exit_sig;

// =========================================================================
//                                PARSING
// =========================================================================

t_command	*initialize_t_command(t_table *table);
t_table		*initialize_t_main(char *line);
t_state		*initialize_t_state(t_state *state);

void		allocate_t_table(t_command *, int count);
int			open_fct_check(char *file, int i);
void		free_t_command(t_command *cmd);
void		new_t_command(t_command *cmd, t_table *table);
void		add_line_to_history(const char *line);
t_command	*tokenize_input(char *input);
t_command	*fill_t_command(char *input);
int			find_fd_out(t_command *cmd);
int			helper_fd_out(t_command *cmd, char *file, int flags);
int			find_fd_in(t_command *cmd);

// Expand

t_command	*parse_pipeline_commands(char **tokens);
char		*process_char(char *input, char *result, t_state *state, t_command *cmd);
char		*process_char_helper(const char *input, char *result, t_state *state);
char		*get_prompt(void);
void		print_command(t_command *cmd);
void		handle_quotes_and_expand(char *arg, char *result, t_command *cmd, t_state *state);
char		*helper_quotes(char *arg, char c, t_state *state);
void		copy_quote_strings(t_command *cmd, char *quotes[4096], t_state *state);

// =========================================================================
//                                EXECUTION
// =========================================================================

int			launch(t_command *cmd, char **envp);
void		execute(t_command *cmd, char **envp);
int			execute_cmd(t_command *cmd, char **envp);
int			deal_in_pipe(t_command *cmd);
int			deal_out_pipe(t_command *cmd);
void		freetab(char **tab);
void		free_cmd(t_command *cmd);

// =========================================================================
//                                BUILTINS
// =========================================================================

// int is_builtins(t_command *cmd);
int			is_builtins(t_command *cmd, char **envp);
int			echo(char **args);
int			cd(char **tab);
int			pwd(void);
int			env(char **envp);
int			unset(char **envp, const char *var);
#endif
