#include "Config.hpp"

Config::Config() 
	: _filePath("") {

		std::cout << "Config default constructor called" << std::endl;
}

Config::Config(std::string filePath) 
	: _filePath(filePath) {
		
		std::cout << "Config constructor called" << std::endl;
}

Config::~Config() {
	
	std::cout << "Config destructor called" << std::endl;
}

void	Config::setFilePath(std::string filePath) {

	this->_filePath = filePath;
}

/*
** This function is for testing purposes only !!!
** The hardcoded data shall be parsed from the config file.
*/
void	Config::setServersData() {

	// This is the data for the first server
	t_serverData	s01;
	s01.port = 8008;
	s01.serverName = "Server_01";
	s01.uploadPath = "www/Server_01/upload";
	s01.errorPages.push_back("404 www/Server_01/error/404.html");
	s01.errorPages.push_back("403 www/Server_01/error/403.html");
	s01.errorPages.push_back("405 www/Server_01/error/405.html");
	
	t_locationsData		location_s01_l01;

	location_s01_l01.location = "/";
	location_s01_l01.root = "../www/Server_01/";
	location_s01_l01.allowedMethods.push_back("GET");
	location_s01_l01.allowedMethods.push_back("POST");
	location_s01_l01.indexes.push_back("index.html");

	s01.locations_map[location_s01_l01.location] = location_s01_l01;

	t_locationsData		location_s01_l02;

	location_s01_l02.location = "/upload";
	location_s01_l02.root = "../www/Server_01/";
	location_s01_l02.allowedMethods.push_back("GET");
	location_s01_l02.allowedMethods.push_back("POST");
	location_s01_l02.indexes.push_back("index.html");

	s01.locations_map[location_s01_l02.location] = location_s01_l02;

	this->_serversData.push_back(s01);

	// This is the data for the second server
	t_serverData	s02;

	s02.port = 8080;
	s02.serverName = "Server_02";
	s02.uploadPath = "www/Server_02/upload";
	s02.errorPages.push_back("404 www/Server_02/error/404.html");
	s02.errorPages.push_back("403 www/Server_02/error/403.html");
	s02.errorPages.push_back("405 www/Server_02/error/405.html");

	t_locationsData		location_s02_l01;

	location_s02_l01.location = "/01_website";
	location_s02_l01.root = "../www/Server_02/01_website";
	location_s02_l01.allowedMethods.push_back("GET");
	location_s02_l01.allowedMethods.push_back("POST");
	location_s02_l01.indexes.push_back("index.html");

	s02.locations_map[location_s02_l01.location] = location_s02_l01;

	t_locationsData		location_s02_l02;

	location_s02_l02.location = "/02_website";
	location_s02_l02.root = "../www/Server_02/02_website";
	location_s02_l02.allowedMethods.push_back("GET");
	location_s02_l02.allowedMethods.push_back("POST");
	location_s02_l02.indexes.push_back("index.html");

	s02.locations_map[location_s02_l02.location] = location_s02_l02;

	this->_serversData.push_back(s02);

	// This is the data for the third server
	t_serverData	s03;

	s03.port = 8800;
	s03.serverName = "Server_03";
	s03.uploadPath = "";
	s03.errorPages.push_back("404 www/Server_03/error/404.html");

	t_locationsData		location_s03_l01;

	location_s03_l01.location = "/";
	location_s03_l01.root = "../www/Server_03/";
	location_s03_l01.allowedMethods.push_back("GET");

	s03.locations_map[location_s03_l01.location] = location_s03_l01;

	this->_serversData.push_back(s03);

	// ...

	setNumberOfServers(this->_serversData.size());
}

void	Config::setNumberOfServers(int number) {

	this->_numberOfServers = number;
}

int	Config::getNumberOfServers() const {

	return this->_numberOfServers;
}

std::vector<t_serverData>	Config::getServersData() const {

	return this->_serversData;
}