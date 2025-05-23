/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   TestSource.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wpepping <wpepping@student.42berlin.de>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/05 11:48:50 by auspensk          #+#    #+#             */
/*   Updated: 2025/05/23 18:02:15 by wpepping         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "TestSource.hpp"

void TestStaticFileSource(){
//set up the config
	ServerConfig serverConfig;

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
	Source * source1 = Source::getNewSource(target, serverConfig);
	source1->readSource();
	std::cout<< "Code: " << source1->getCode() << "\nSize: " << source1->getSize() << "\nBytes read:" <<std::endl;
	std::vector<char> const&contents1 = source1->getBytesRead();
	for(std::vector<char>::const_iterator iter = contents1.begin();iter != contents1.end();++iter)
		std::cout<<*iter;
	std::cout <<std::endl;
}
