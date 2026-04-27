#include "include/configFileHandler.hpp"
#include <iostream>

int main(int ac, char **av)
{

    std::vector<ServerConfig> servers;

	if (ac != 2)
	{
		std::cerr << "Usage: " << av[0] << " <config_file_path>" << std::endl;
		return 1;
	}
    if (configFileHandler(av[1], servers) == false)
        return (1);
    return (0);
}