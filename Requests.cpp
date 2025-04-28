/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Requests.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wouter <wouter@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/25 14:09:57 by wouter            #+#    #+#             */
/*   Updated: 2025/04/25 14:11:38 by wouter           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <Request.hpp>

Request::Request() : _requestType("GET"), _target("/index.html"), _isReady(true) {}

Request::~Request() {}
