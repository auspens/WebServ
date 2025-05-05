/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestSource.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: auspensk <auspensk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:48:50 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/05 16:34:20 by auspensk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../Source/StaticFileSource.hpp"
#include "../ConfigClasses/Config.hpp"
#include "../Server.hpp"

void TestStaticFileSource(){
//set up the config
	ServerConfig serverConfig(DEFAULT_HOST, DEFAULT_PORT);
	std::string defaultRoot = "www";
	serverConfig.setRootFolder(defaultRoot);

//try to get /about.html
	std::string target = "/about.html";
	// StaticFileSource source(target, serverConfig);
	// source.read();
	// std::cout<< "Code: " << source.getCode() << "\nSize: " << source.getSize() << "\nBytes read:" <<std::endl;
	// std::vector<char> const&contents = source.getBytesRead();
	// for(std::vector<char>::const_iterator iter = contents.begin();iter != contents.end();++iter)
	// 	std::cout<<*iter;
	// std::cout <<std::endl;

//try to get /index.html by sending the request to root
	target = "/";
	StaticFileSource source1(target, serverConfig);
	source1.read();
	std::cout<< "Code: " << source1.getCode() << "\nSize: " << source1.getSize() << "\nBytes read:" <<std::endl;
	std::vector<char> const&contents1 = source1.getBytesRead();
	for(std::vector<char>::const_iterator iter = contents1.begin();iter != contents1.end();++iter)
		std::cout<<*iter;
	std::cout <<std::endl;
}
