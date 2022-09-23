/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mschlenz <mschlenz@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/04 13:02:06 by mschlenz          #+#    #+#             */
/*   Updated: 2022/09/12 10:19:23 by mschlenz         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>



void	signal_handler(int sig, siginfo_t *info, void *context)
{
	if (info->si_pid == 0)
			printf("\n");
	if (sig == SIGINT && info->si_pid != 0)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	// if (sig == SIGQUIT)  --> das exited automatisch davor es kommt garnicht in das if rein lol
	// 	exit(0);
	if (sig == SIGCHLD && info->si_status == 3) //falls der si status auf mac anders ist, dann einfach : info->si_status != 0
	{
		printf("Quit (core dumped)");
		printf("\n");
	}
}

void	signals(bool in_child)
{
	struct sigaction	sa;
	struct sigaction	sa_ignore;
	
	sa.sa_sigaction = signal_handler;
	//sigfillset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART | SA_SIGINFO;
	sa_ignore.sa_handler = SIG_IGN;
	
	if (!in_child)
		sigaction(SIGQUIT, &sa_ignore, NULL);
	else
	{
		sigaction(SIGQUIT, &sa, NULL);
	}
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGCHLD, &sa, NULL);
}