/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstlast.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:37:36 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/02/05 18:12:47 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_stack	*ft_lstlast(t_stack *lst)
{
	t_stack	*l;

	if (!lst)
		return (NULL);
	l = lst;
	while (l->next)
	{
		l = l->next;
	}
	return (l);
}

/* #include <stdio.h>
int main()
{
    t_list *head;
    t_list *new = ft_lstnew("E");
    t_list *new2 = ft_lstnew("L");
    t_list *addfr = ft_lstnew("H");
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
    printf("%s", (char *)ft_lstlast(head)->content);
    newtest=head;
    printf("\n-------------------------------------------\n");
    while (newtest)
    {
        printf("%s", (char *)newtest->content);
        newtest=newtest->next;
    }
    return(0);
} */