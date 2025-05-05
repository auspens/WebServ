/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/30 11:41:02 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/05 15:13:06 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#include <map>

class Location{
	public:
		Location();
		Location(const std::string &path);
		~Location();
		Location(const Location &src);
		Location &operator =(const Location &other);

		const std::string &getPath()const;
		const std::string &getIndex()const;
		bool autoindexOn()const;
		const std::pair<int, std::string> &getRedirect()const;
		const std::map<int, std::string> &getErorrPagesLocal()const;

		void setIndex(const std::string &index);
		void setAutoindex(bool autoindex);
		void setRedirect(std::pair<int, std::string> &redirect);
		void setErrorPagesLocal(std::map<int, std::string> &errorPages);

	private:
		std::string 				_path;
		bool 						_autoindex;
		std::string 				_index;
		std::pair<int, std::string> _redirect;
		std::map<int, std::string>	_errorPagesLocal;
};
