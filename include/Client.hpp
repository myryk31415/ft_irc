/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:35:23 by padam             #+#    #+#             */
/*   Updated: 2024/10/16 22:37:34 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "ft_irc.hpp"

class Client
{
	private:
		int			_fd;
		std::string	_ip_addr;
	public:
		Client();
		~Client();

		int		get_fd() const;
		void	set_fd(int fd);
		void	set_ip_addr(std::string ip_addr);
};
