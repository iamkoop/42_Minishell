/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstdelone.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nildruon <nildruon@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/01/19 14:19:46 by nilsdruon         #+#    #+#             */
/*   Updated: 2026/02/05 21:15:26 by nildruon         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"
/* #include <stdio.h>
void del_content(void *content)
{
    printf("Deleting content: %s\n", (char *)content);
    free(content);
} */

/* void	ft_lstdelone(t_stack *lst, void (*del)(void*))
{
	if (!lst || !del)
		return ;
	del(lst->data);
	free(lst);
} */

/* int main(void)
{
    t_list *node;
    char   *str;

    str = malloc(6);
    if (!str)
        return (1);
    str[0] = 'H';
    str[1] = 'e';
    str[2] = 'l';
    str[3] = 'l';
    str[4] = 'o';
    str[5] = '\0';

    node = malloc(sizeof(t_list));
    if (!node)
        return (1);
    node->content = str;
    node->next = NULL;

    printf("Node content before delete: %s\n", (char *)node->content);

    ft_lstdelone(node, del_content);

    printf("Node deleted successfully\n");

    return (0);
} */
