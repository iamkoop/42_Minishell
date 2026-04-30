/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_back.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:55:23 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/02/05 18:25:05 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_back(t_stack **lst, t_stack *new)
{
	t_stack	*l;

	if (!lst || !new)
		return ;
	if (!*lst)
	{
		*lst = new;
	}
	else
	{
		l = *lst;
		while (l->next)
		{
			l = l->next;
		}
		l->next = new;
	}
}

/* #include <stdio.h>
int main()
{
    t_list *head;
    t_list *newlist = NULL;
    t_list *new = ft_lstnew("H");
    t_list *new2 = ft_lstnew("E");
    t_list *addback = ft_lstnew("L");
    t_list *newtest;
    head=new;
    new->next=new2;
    new2->next=NULL;
    newtest = head;  
    while (newtest)
    {
        printf("%s", (char *)newtest->content);
        newtest=newtest->next;
    }
    ft_lstadd_back(&head,addback);
    newtest=head;
    printf("\n-------------------------------------------\n");
    while (newtest)
    {
        printf("%s", (char *)newtest->content);
        newtest=newtest->next;
    }
    return(0);
} */