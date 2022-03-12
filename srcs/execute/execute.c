/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: min-kang <minguk.gaang@gmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:54:12 by min-kang          #+#    #+#             */
/*   Updated: 2022/03/12 20:13:39 by min-kang         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	do_cmd_execute(t_node *node, int *fd, char **envp)
{
	char		*cmd_path;

	cmd_path = path_define(node->left->args[0], envp);
	if (!cmd_path)
		exit(127);
	execve(cmd_path, node->left->args, envp);
	write(fd[1], "", 1);
	exit(127);
	return (1);
}

static int	cmd_execute(t_app *app, t_node *node, int *fd, char **envp)
{
	t_redir		redir;
	int			exit_code;
	int			copy[2];

	if (fd[2])
	{
		copy[0] = dup(STDIN_FILENO);
		copy[1] = dup(STDOUT_FILENO);
	}
	redir = redir_initialize(fd[0], fd[1]);
	if (node->right && (redir_define(&redir, node->right->redir_name,
				node->right->redir_type, fd[2])))
		return (1);
	dup2_2d(redir.input, redir.output);
	exit_code = builtin_execute(node->left, app, fd[2]);
	if (exit_code >= 0)
	{
		dup2_2d(copy[0], copy[1]);
		if (fd[2])
			return (exit_code);
		else
			exit(exit_code);
	}
	return (do_cmd_execute(node, fd, envp));
}

static int	execute_loop(t_app *app, t_node *node, char **envp, int fd_in)
{
	int		fd[2];
	pid_t	pid;

	if (node && node->node_type > 1)
	{
		pipe(fd);
		pid = fork();
		if (pid == 0)
		{
			close(fd[0]);
			if (node->right && node->right->node_type > 1)
				cmd_execute(app, node->left, (int [3]){fd_in, fd[1], 0}, envp);
			else
				cmd_execute(app, node,
					(int [3]){fd_in, STDOUT_FILENO, 0}, envp);
		}
		else
		{
			close(fd[1]);
			execute_loop(app, node->right, envp, fd[0]);
			close(fd[0]);
			waitpid(pid, &g_res, 0);
		}
	}
	return (WEXITSTATUS(g_res));
}

int	parse_execute(t_app *app, t_token *begin, int index, char **envp)
{
	t_node	*node;
	int		res;

	node = parser(begin, index);
	if (node->root->node_type == 2 && builtin_check(node->root->left) != -1)
		res = cmd_execute(app, node->root, (int [3]){0, 1, 1}, envp);
	else
		res = execute_loop(app, node->root, envp, 0);
	free_node(node->root);
	free(node);
	unlink(HEREDOC);
	return (res);
}
