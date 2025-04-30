/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 12:40:08 by auspensk          #+#    #+#             */
/*   Updated: 2025/04/30 14:56:31 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location(std::string &path)
		: _path(path),
		_autoindex(false),
		_index(""),
		_redirect(0, ""){}
Location::~Location(){}
Location::Location(const Location &src)
			: _path(src._path),
			_autoindex(src._autoindex),
			_index(src._index),
			_redirect(src._redirect){};
Location &Location::operator =(const Location &other){
	if (this != &other){
		_path = other._path;
		_autoindex = other._autoindex;
		_index = other._index;
		_redirect = other._redirect;
	}
	return *this;
}
const std::string &Location::getPath()const{
	return _path;
}
const std::string &Location::getIndex()const{
	return _index;
}
bool Location::autoindexOn()const{
	return _autoindex;
}
const std::pair<int, std::string> &Location::getRedirect()const{
	return _redirect;
}
const std::map<int, std::string> &Location::getErorrPagesLocal()const{
	return _errorPagesLocal;
}
void Location::setIndex(std::string &index){
	_index = index;
}
void Location::setAutoindex(bool autoindex){
	_autoindex = autoindex;
}
void Location::setRedirect(std::pair<int, std::string> &redirect){
	_redirect = redirect;
}
void Location::setErrorPagesLocal(std::map<int,std::string> &errorPages){
	_errorPagesLocal = errorPages;
}
