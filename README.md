# Web Server Configuration File Handler

A robust C++ configuration file parser for web servers. This project parses and validates server configuration files into structured C++ objects, supporting multiple servers with location-based routing, error pages, and CGI handling.

## Overview

This configuration file handler is designed to parse nginx-like configuration files for web servers. It validates the configuration structure, syntax, and values while populating a collection of `ServerConfig` objects that can be used by a web server application.

### Key Features

- **Multi-server support**: Parse multiple server blocks within a single configuration file
- **Location-based routing**: Define location blocks with specific path configurations
- **Error page handling**: Configure custom error pages for various HTTP status codes
- **CGI integration**: Support for CGI pass configuration with file extension mapping
- **HTTP method control**: Specify allowed HTTP methods (GET, POST, DELETE) per location
- **Client body size limits**: Set maximum request body sizes at server and location levels
- **Auto-indexing**: Enable/disable directory listing
- **Redirections**: Configure HTTP redirections within locations
- **Comprehensive validation**: Format validation, shape validation, and value validation
- **Error reporting**: Detailed error messages with color-coded output

## Project Structure

```
├── main.cpp                          # Entry point
├── configFile.hpp                    # Main header file
├── Makefile                          # Build configuration
├── file.conf                         # Example configuration file
│
├── Core Parsing Components:
│   ├── configFileHandler.cpp         # Main configuration parser
│   ├── Tokenizer.cpp                 # Lexical tokenizer
│   ├── parseConfig.cpp               # AST parser
│   └── Utils.cpp                     # Utility functions
│
├── Validation Components:
│   ├── FormatValidator.cpp           # Format validation (braces, syntax)
│   ├── ShapeValidator.cpp            # Structure validation
│   ├── validateBlocks.cpp            # Block-level validation
│   └── validateServerValues.cpp      # Server value validation
│
└── Classes:
    ├── ServerConfig.cpp/hpp          # Server configuration class
    ├── Location.cpp/hpp              # Location block class
    ├── ConfigException.cpp/hpp       # Exception handling
    └── ConfigFileTypes.hpp           # Type definitions and structures
```

## Building the Project

### Prerequisites
- C++11 or later compiler
- Make utility

### Compilation

```bash
# Build the project
make

# Build with debug info and ASAN
make all

# Remove object files
make clean

# Remove all generated files (including binary)
make fclean

# Clean and rebuild
make re

# Run with Valgrind
make debug
```

The build process compiles all source files with Address Sanitizer (`-fsanitize=address`) and debug symbols (`-g3`) by default.

## Usage

```bash
./parser <config_file_path>
```

**Arguments:**
- `config_file_path`: Path to the configuration file to parse

**Example:**
```bash
./parser file.conf
```

The parser reads the configuration file, validates it, and populates a vector of `ServerConfig` objects.

## Configuration File Format

### Server Block Structure

```nginx
server {
    listen 8080;
    server_name example.com;
    client_body_size 1000000;
    root ./www;
    index index.html;
    
    error_page {
        404 error_pages/404.html;
        500 error_pages/500.html;
    }
    
    cgi_pass {
        .php /usr/bin/php-cgi;
        .py /usr/bin/python3;
    }
    
    location /api {
        root ./api;
        index api.html;
        allow_methods GET POST;
        client_body_size 5000000;
        auto_index on;
        upload_path ./uploads;
        
        redirection {
            /old_path /new_path 301;
        }
    }
}
```

### Configuration Directives

| Directive | Level | Type | Description |
|-----------|-------|------|-------------|
| `listen` | server | integer | Port number the server listens on |
| `server_name` | server | string | Server name/domain |
| `root` | server/location | path | Root directory for serving files |
| `index` | server/location | filename | Default file to serve |
| `client_body_size` | server/location | size | Max request body size in bytes |
| `error_page` | server | block | Maps error codes to HTML pages |
| `cgi_pass` | server | block | Maps file extensions to CGI interpreters |
| `allow_methods` | location | list | Allowed HTTP methods (GET, POST, DELETE) |
| `auto_index` | location | boolean | Enable/disable directory listing |
| `upload_path` | location | path | Directory for file uploads |
| `redirection` | location | block | HTTP redirections (old_path new_path status_code) |

## Core Classes

### ServerConfig

Represents a server block configuration.

**Key Methods:**
- `SetServerName(const std::string &val)` - Set server name
- `SetListen(const int &val)` - Set listening port
- `SetRoot(const std::string &val)` - Set root directory
- `SetIndex(const std::string &val)` - Set default index file
- `SetClientBodySize(const size_t &val)` - Set max body size
- `AddErrorPage(const errorPages_t &val)` - Add error page mapping
- `AddLocation(const Location &location)` - Add location block
- `AddCgiPass(const cgiPass_t &cgi)` - Add CGI pass mapping
- `PrintServerInfo() const` - Print server configuration

### Location

Represents a location block within a server.

**Key Methods:**
- `SetPath(const std::string &val)` - Set location path
- `SetRoot(const std::string &val)` - Set location root
- `SetIndex(const std::string &val)` - Set default index
- `SetUploadPath(const std::string &val)` - Set upload directory
- `SetClientBodySize(const size_t &val)` - Set max body size
- `SetAutoindex(const bool &val)` - Enable/disable auto-indexing
- `SetAllowGet/Post/Delete(const bool &val)` - Set allowed methods
- `AddRedirection(const redirection_t &red)` - Add redirection rule
- `PrintLocationInfo(size_t index) const` - Print location details

### ConfigException

Exception class for configuration errors, inherits from `std::exception`.

## Parsing Pipeline

1. **Tokenization**: The config file is tokenized into logical tokens
2. **Format Validation**: Checks syntax (braces matching, proper structure)
3. **Shape Validation**: Validates the hierarchical structure (server/location nesting)
4. **Value Validation**: Validates individual configuration values and constraints
5. **Parsing**: Populates ServerConfig objects with parsed values

## Example: Parsing a Configuration

```cpp
#include "include/configFileHandler.hpp"

int main() {
    std::vector<ServerConfig> servers;
    
    if (configFileHandler("server.conf", servers)) {
        // Successfully parsed
        for (const auto& server : servers) {
            server.PrintServerInfo();
        }
    }
    return 0;
}
```

## Error Handling

The parser provides comprehensive error messages with color-coded output:
- **RED**: Errors
- **YELLOW**: Warnings
- **GREEN**: Success messages
- **CYAN**: Information messages

Errors are reported using the `putErr()` function and may throw `ConfigException` for critical issues.

## Technologies Used

- **Language**: C++11
- **Build System**: Make
- **Debugging**: AddressSanitizer (ASAN), Valgrind support
- **Exception Handling**: Standard C++ exceptions

## Example Configuration File

See `file.conf` in the project root for a working example configuration that can be parsed by this tool.

## Development Notes

- The parser uses memory sanitization by default (AddressSanitizer)
- All source files follow consistent naming conventions
- The project includes comprehensive validation at multiple levels
- Configuration errors are caught early in the parsing pipeline

## License

This project is part of the webServ project suite.
