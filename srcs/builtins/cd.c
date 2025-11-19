/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aalbugar <aalbugar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/30 17:07:42 by ghsaad            #+#    #+#             */
/*   Updated: 2025/11/18 16:59:14 by aalbugar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"


static int      update_env_var(t_list **env, char *key, char *value)
{
        char    *new_var;
        t_list  *tmp;

        if (!env || !key || !value)
                return (1);
        new_var = create_env_var_string(key, value);
        if (!new_var)
                return (1);
        tmp = *env;
        while (tmp)
        {
                if (ft_strncmp(tmp->str, key, ft_strlen(key)) == 0
                        && tmp->str[ft_strlen(key)] == '=')
                {
                        free(tmp->str);
                        tmp->str = new_var;
                        return (0);
                }
                tmp = tmp->next;
                if (tmp == *env)
                        break ;
        }
        if (append_to_list(env, new_var))
                return (0);
        return (1);
}

int     handle_cd_with_path(char *path)
{
        if (chdir(path) == -1)
        {
                error_type_msg(ERR_CHDIR, "cd", path, errno);
                return (1);
        }
        return (0);
}

static char     *get_home_from_env_list(t_list *env)
{
        t_list  *tmp;

        tmp = env;
        while (tmp)
        {
                if (ft_strncmp(tmp->str, "HOME=", 5) == 0)
                        return (tmp->str + 5);
                tmp = tmp->next;
                if (tmp == env)
                        break ;
        }
        return (NULL);
}
static int      handle_cd_to_home(t_list *env)
{
        char    *home;

        home = get_home_from_env_list(env);
        if (!home)
        {
                error_type_msg(ERR_HOME_NOT_SET, "cd", NULL, 0);
                return (1);
        }
        return (handle_cd_with_path(home));
}
int     ft_cd(char **args, t_list **env)
{
        char    old_pwd[PATH_MAX];
        char    new_pwd[PATH_MAX];

        if (!env || !*env)
        {
                error_type_msg(ERR_ENV_NOT_SET, "cd", NULL, 0);
                return (1);
        }
        if (!getcwd(old_pwd, PATH_MAX))
        {
                error_type_msg(ERR_GENERAL, "cd", "getcwd", errno);
                return (1);
        }
        if (!args[1])
        {
                if (handle_cd_to_home(*env))
                        return (1);
        }
        else if (ft_strcmp(args[1], "-") == 0)
        {
                error_type_msg(ERR_OLDPWD_NOT_SET, "cd", NULL, 0);
                return (1);
        }
        else if (handle_cd_with_path(args[1]))
                return (1);
        if (!getcwd(new_pwd, PATH_MAX))
        {
                error_type_msg(ERR_GENERAL, "cd", "getcwd", errno);
                return (1);
        }
        if (update_env_var(env, "OLDPWD", old_pwd))
                return (1);
        if (update_env_var(env, "PWD", new_pwd))
                return (1);
        return (0);
}
