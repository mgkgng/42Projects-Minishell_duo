/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gphilipp <gphilipp@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/16 15:54:12 by min-kang          #+#    #+#             */
/*   Updated: 2022/03/09 21:42:15 by gphilipp         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	cmd_execute(t_app *app, t_node *node, int *fd, char **envp)
{
	t_redir	redir;
	char	*cmd_path;
	int		fd_in;
	int		fd_out;

	fd_in = fd[0];
	fd_out = fd[1];
	redir = redir_initialize(fd_in, fd_out);
	if (node->right)
		redir_define(&redir, node->right->redir_name, node->right->redir_type);
	dup2(redir.input, 0);
	dup2(redir.output, 1);
	if (builtin_execute(node->left, app) >= 0)
	{
		if (fd[2])
			return (0);
		else
			exit(0);
	}
	cmd_path = path_define(node->left->args[0], envp);
	if (!cmd_path)
		exit(127);
	execve(cmd_path, node->left->args, envp);
	write(fd_out, "", 1);
	exit(127);
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
				cmd_execute(app, node, (int [3]){fd_in, 1, 0}, envp);
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

/**
 * Il faut exécuter les builtins sans fork quand c'est une simple commande pour
 * que export fonctionne car l'impact du code est limité à la scope du fork…
 * Donc aucun changement sur les variables du main.
 **/
int	execute(t_app *app, t_node *node, char **envp)
{
	int	success;

	if (node->root->node_type == 2 && builtin_check(node->root->left) != -1)
	{
		success = cmd_execute(app, node->root, (int [3]){0, 1, 1}, envp);
	}
	else
	{
		success = execute_loop(app, node->root, envp, 0);
	}
	unlink(HEREDOC);
	return (success);
}
