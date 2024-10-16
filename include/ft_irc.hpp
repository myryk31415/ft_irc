/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/16 15:38:34 by padam             #+#    #+#             */
/*   Updated: 2024/10/17 00:42:00 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <iostream>

#define RESET "\e[0;37m"
#define RED "\e[1;31m"
#define GREEN "\e[1;32m"
#define YELLOE "\e[1;33m"

#define MAX_PORTS 65536
#define PRIVILEGED_PORTS 1023
#define PRIVATE_PORTS 49152
