flags =  -fsanitize=address -g3 

src			= 	main.cpp \
				configFileHandler.cpp\
				FormatValidator.cpp \
				ShapeValidator.cpp \
				parseConfig.cpp \
				Tokenizer.cpp \
				Utils.cpp \
				validateBlocks.cpp \
				validateServerValues.cpp \
				classes/ConfigException.cpp \
				classes/Location.cpp \
				classes/ServerConfig.cpp
				
				
				
				
				
				


OBJ			= $(src:.cpp=.o)

NAME = parser

all: $(NAME)

debug:
	valgrind --track-origins=yes ./$(NAME)  
	
#--leak-check=full --show-leak-kinds=all 

$(NAME): $(OBJ)
	c++ $(flags) $(OBJ) -o $(NAME)

%.o: %.cpp
	c++ $(flags) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean:	clean
	rm -f $(NAME)


re: fclean all

.PHONY: all fclean re clean
