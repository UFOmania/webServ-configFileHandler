#include "configFile.hpp"


int detectType(Configtokens & t, size_t &idx)
{
	/*ex:
	server {  } 		-- group
	location /img {  }	-- special group
	*/

	if (!isWord(t[idx])) //first its always word
	{
		putErr("Invalid '{', '}', ';' position");
		return 0;
	}

	// ---- WORD ----

	idx++;
	if (t[idx] == "{") // ---- WORD { ----
	{
		return 1;
	}
	else if (isWord(t[idx])) // ---- WORD WORD ----
	{
		idx++;
		if (t[idx] == "{") // ---- WORD WORD { ----
			return 2;
		else
		{
			for(; idx < t.size() && isWord(t[idx]); idx++)
				;
			if (t[idx] != ";")
			{
				putErr("Key Value/s is not closed ';'");
				return 0;
			}
			return 3;
		}
	}
	else
	{
		putErr("Invalid sybmol after a WORD");
		return 0; // ---- WORD ; ---- | ---- WORD } ----
	}
}


void validateShape(Configtokens &t)
{
	int s = t.size();
	int blockDepth = 0;
	bool freshGroup = false;


	for(size_t i = 0; i < s; i++)
	{
		if(t[i] == "}")
		{
			// std::cout << "End Block detected } --\n";
			if (freshGroup)
				throw ConfigException("Empty block is not allowed");


			blockDepth--;
			continue;
		}

		switch (detectType(t, i))
		{
			case (1):
				blockDepth++;
				// std::cout << "group detected \n";
				freshGroup = true;
				break;
			case (2):
				//validate special group
				blockDepth++;
				freshGroup = true;
				// std::cout << "special group detected\n";
				break;
			case (3):
				if (blockDepth == 0)
					throw ConfigException("Value outside of a block is not allowed");


				freshGroup = false;
				//validate key
				// std::cout << "keyValue detected\n";
				break;
			default://the probleam was here
				//error
				// std::cout << "error detected\n";
				throw ConfigException("unixpected error in configFile shape");
		}
		
	}
}