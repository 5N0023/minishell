/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbennani <mbennani@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 03:20:19 by mlektaib          #+#    #+#             */
/*   Updated: 2023/06/16 22:09:15 by mbennani         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute.h"

int check_if_anyinput(t_ast *node)
{
    t_ast    *tmp;

    tmp = node->u_data.operation.right;
    if(!tmp)
        return (1);
    if(tmp->type != ast_redirect_in && tmp->type != ast_heredoc && tmp->type != ast_redirect_out)
        return (1);
    while(tmp)
    {
        if (tmp->type == ast_redirect_in || tmp->type == ast_heredoc)
            return (0);
        if(tmp->type == ast_heredoc)
            tmp = tmp->u_data.heredoc.next;
        else if (tmp->type == ast_redirect_in)
            tmp = tmp->u_data.redirect_in.next;
        else if(tmp->type == ast_redirect_out)
            tmp = tmp->u_data.redirect_out.next;
    }
    return (1);
}
int    create_right_child(t_ast *node, int pipefd[2], int left_pid, t_env **env)
{
    int        right_pid;
    int        status;

    status = 0;
    right_pid = fork();
    if (right_pid == -1)
        exit(1);
    else if (right_pid == 0)
    {
        close(pipefd[1]);
        if (check_if_anyinput(node))
            dup2(pipefd[0], STDIN_FILENO);
        status = execute_commands(node->u_data.operation.right, env);
        exit(status);
    }
    else
    {
        close(pipefd[0]);
        close(pipefd[1]);
        waitpid(left_pid, &status, 0);
        waitpid(right_pid, &status, 0);
        return (status);
    }
}

int	create_pipe(t_ast *node, t_env **env)
{
	int		pipefd[2];
	int		left_pid;
	int		status;

	status = 0;
	if (pipe(pipefd) == -1)
		exit(1);
	left_pid = fork();
	if (left_pid == -1)
		exit(1);
	else if (left_pid == 0)
	{
		close(pipefd[0]);
		dup2(pipefd[1], STDOUT_FILENO);
		status = execute_commands(node->u_data.operation.left, env);
		exit(status);
	}
	else
		return (create_right_child(node, pipefd, left_pid, env));
}
