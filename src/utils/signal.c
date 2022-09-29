/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 13:02:06 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/29 15:07:01 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>

void	signal_handler_child(int sig, siginfo_t *info, void *context)
{
	(void)context;
	
	if (info->si_pid == 0)
		printf("\n");
	if (sig == SIGINT)
	{
		printf("wtf\n");
		rl_on_new_line();
		rl_replace_line("fgfghf", 0);
		rl_redisplay();
	}
	if (sig == SIGCHLD && info->si_status == 3)
	{
		printf("Quit");
		printf("\n");
	}
	else if (sig == SIGCHLD)
	{
		printf("\n");
	}
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
	struct sigaction	sa; // = { { 0 } };
	struct sigaction	sa_child; // = { { 0 } };
	struct sigaction	sa_ignore; // = { { 0 } };

	ft_memset(&sa, 0, sizeof(sa));
	ft_memset(&sa_child, 0, sizeof(sa_child));
	ft_memset(&sa_ignore, 0, sizeof(sa_ignore));
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa_child.sa_flags = SA_RESTART | SA_SIGINFO;
	sa.sa_sigaction = &signal_handler;
	sa_child.sa_sigaction = &signal_handler_child;
	sa_ignore.sa_handler = SIG_IGN;
	//sigfillset(&sa.sa_mask);
	//signal(SIGINT, SIG_DFL);
	if (!in_child)
	{
		sigaction(SIGQUIT, &sa_ignore, NULL);
		sigaction(SIGINT, &sa, NULL);
	}
	// else
	// {
	// 	printf("in child\n");
	// 	//sigaction(SIGQUIT, &sa, NULL);
	// 	sigaction(SIGINT, &sa, NULL);
	// 	// sigaction(SIGINT, &sa_child, NULL);
	// }
	sigaction(SIGCHLD, &sa_child, NULL);
}