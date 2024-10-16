/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 23:37:52 by padam             #+#    #+#             */
/*   Updated: 2024/10/17 00:00:09 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <string>
#include "ft_irc.hpp"

void	usage()
{
	std::cout << "usage: ./ircserv <port> <password>" << std::endl;
}

int	main(int argc, char **argv)
{
	std::string	pass;
	int	port;

	if (argc != 3)
		return (usage(), 1);
	try
	{
		port = std::stoi(argv[1]);
	}
	catch (std::exception &e)
	{
		std::cout << "Port out of range" << std::endl;
	}
	if (port <= PRIVILEGED_PORTS || port > MAX_PORTS - 1)
	{
		if (port < 0)
			std::cout << "Port has to be positive" << std::endl;
		else if (port <= PRIVILEGED_PORTS)
			std::cout << "Ports below " << PRIVILEGED_PORTS + 1 << " are privileged" << std::endl;
		else if (port > MAX_PORTS)
			std::cout << "Port has to be lower than " << MAX_PORTS - 1 << std::endl;
		return (1);
	}
	std::cout << port << std::endl;
	pass = std::string(argv[2]);
}
