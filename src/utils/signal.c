/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 13:02:06 by mschlenz          #+#    #+#             */
/*   Updated: 2022/10/18 12:55:51 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	signal_handler_child(int sig, siginfo_t *info, void *context)
{
	(void)context;
	if (info->si_pid == 0)
		printf("");
	if (sig == SIGCHLD && info->si_status == 3)
	{
		printf("Quit");
		printf("\n");
	}
	else if (sig == SIGCHLD && info->si_status && info->si_status != 13)
		printf("");
}

void	signal_handler(int sig, siginfo_t *info, void *context)
{
	(void)context;
	if (sig == SIGINT && info->si_pid != 0)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	signals(bool in_child)
{
	struct sigaction	sa;
	struct sigaction	sa_child;
	struct sigaction	sa_ignore;

	ft_memset(&sa, 0, sizeof(sa));
	ft_memset(&sa_child, 0, sizeof(sa_child));
	ft_memset(&sa_ignore, 0, sizeof(sa_ignore));
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa_child.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = &signal_handler;
	sa_child.sa_sigaction = &signal_handler_child;
	sa_ignore.sa_handler = SIG_IGN;
	if (!in_child)
	{
		sigaction(SIGQUIT, &sa_ignore, NULL);
		sigaction(SIGINT, &sa, NULL);
	}
	sigaction(SIGCHLD, &sa_child, NULL);
}
