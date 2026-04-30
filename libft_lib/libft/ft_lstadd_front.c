/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstadd_front.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 12:27:12 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/02/05 13:58:14 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_lstadd_front(t_stack **lst, t_stack *new)
{
	new->next = *lst;
	*lst = new;
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
    ft_lstadd_front(&head,addfr);
    newtest=head;
    printf("\n-------------------------------------------\n");
    while (newtest)
    {
        printf("%s", (char *)newtest->content);
        newtest=newtest->next;
    }
    return(0);
} */