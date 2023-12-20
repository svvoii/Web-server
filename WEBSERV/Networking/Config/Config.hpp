/*
** This class will be used to hold the data from the config file.
** In this version it will be used to test the servers, so
** no parsing will be done.
** The data will be hardcoded for now.
**
** There will be a struct which will temporary hold the data
** from the config file, until the Server class instances are created.
*/
#ifndef CONFIG_HPP
#define CONFIG_HPP


#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "Server.hpp"

typedef struct	s_locationsData {

	std::string					location;
	std::string					root;
	std::vector<std::string>	allowedMethods;
	std::vector<std::string>	indexes;
	//std::vector<std::string>	cgiExtensions;
	//std::vector<std::string>	cgiBinaries;

} t_locationsData;

typedef struct	s_serverData {

	int										port;
	std::string								serverName;
	std::string								uploadPath;
	std::vector<std::string>				errorPages;
	std::map<std::string, t_locationsData>	locations_map;

} t_serverData;

class Config {
	private:

		std::string					_filePath;

		int							_numberOfServers;
		std::vector<t_serverData>	_serversData;

	public:

		Config();
		Config(std::string filePath);
		~Config();

		

		// Setters
		void						setFilePath(std::string filePath);
		void						setServersData();
		void						setNumberOfServers(int number);

		// Getters
		int							getNumberOfServers() const;
		std::vector<t_serverData> 	getServersData() const;

};

#endif
