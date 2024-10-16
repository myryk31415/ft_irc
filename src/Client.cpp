/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:38:38 by padam             #+#    #+#             */
/*   Updated: 2024/10/16 22:37:30 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client::Client()
{
}

Client::~Client()
{
}

int		Client::get_fd() const
{
	return (_fd);
}

void	Client::set_fd(int fd)
{
	_fd = fd;
}

void	Client::set_ip_addr(std::string ip_addr)
{
	_ip_addr = ip_addr;
}
