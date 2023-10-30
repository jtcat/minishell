/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jcat <joaoteix@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 12:04:34 by jcat              #+#    #+#             */
/*   Updated: 2023/10/30 12:06:00 by jcat             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <minishell.h>
#include <env.h>
#include <unistd.h>

void	export_var(t_shctx *ctx, char *var)
{
	t_dlist		*exp_ref;
	const char	*varval;

	exp_ref = get_export_ref(ctx, var);
	if (!exp_ref)
	{
		ft_dlstadd_back(&ctx->exports, ft_dlstnew(get_var_id(var)));
		ctx->exports_len++;
	}
	else
	{
		free(exp_ref->content);
		exp_ref->content = get_var_id(var);
	}
	varval = ft_strchr(var, '=');
	if (varval)
		set_var(ctx, var, varval + 1);
}

#define ERR_EXP_INV "`%s\': not a valid identifier\n"

// Need to validate ID, failing an assigment does not prevent others
int	export_vars(t_shctx *ctx, char **vars)
{
	int			exitval;

	exitval = 0;
	while (*vars)
	{
		if (val_var_id(*vars))
			export_var(ctx, *vars);
		else
		{
			ft_dprintf(STDERR_FILENO, MSH_ERR_PFIX ERR_EXP_INV, *vars);
			exitval = 1;
		}
		vars++;
	}
	return (exitval);
}

int	unset_cmd(t_shctx *ctx, char **var_ids)
{
	t_dlist	*var_ref;

	while (*var_ids)
	{
		var_ref = get_var_ref(ctx, *var_ids);
		if (var_ref)
		{
			ft_dlstrmone(&ctx->envp, var_ref, free);
			ctx->envp_len--;
		}
		var_ref = get_export_ref(ctx, *var_ids);
		if (var_ref)
		{
			ft_dlstrmone(&ctx->exports, var_ref, free);
			ctx->exports_len--;
		}
		var_ids++;
	}
	return (0);
}

#define EXP_FULL "declare -x %s=\"%s\"\n"
#define EXP_ID "declare -x %s\n"

int	export_ls_vars(t_shctx *ctx)
{
	t_dlist		*lrgst;
	t_dlist		*dup_exp;
	t_dlist		*var;
	char		*varid;

	dup_exp = ft_dlstdup(ctx->exports);
	while (dup_exp)
	{
		lrgst = NULL;
		var = dup_exp;
		while (var)
		{
			if (!lrgst || ft_strcmp(var->content, lrgst->content) < 0)
				lrgst = var;
			var = var->next;
		}
		varid = get_var_id(lrgst->content);
		if (get_var_val(ctx, lrgst->content))
			ft_dprintf(STDOUT_FILENO, EXP_FULL, varid, get_var_val(ctx, varid));
		else
			ft_dprintf(STDOUT_FILENO, EXP_ID, varid);
		free(varid);
		ft_dlstrmone(&dup_exp, lrgst, NULL);
	}
	return (0);
}

int	export_cmd(t_shctx *ctx, char **var_ids)
{
	if (*var_ids)
		return (export_vars(ctx, var_ids));
	return (export_ls_vars(ctx));
}
