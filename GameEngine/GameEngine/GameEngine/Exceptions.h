#pragma once

#include <string>

struct WindowCreationException : std::exception {
	const char * what() const throw () {
		return "Window could not be created!";
	}
};

struct ApplicationManagerCreationException : std::exception {
	const char * what() const throw () {
		return "ApplicationManager could not be created!";
	}
};

struct GraphicsManagerCreationException : std::exception {
	const char * what() const throw () {
		return "GraphicsManager could not be created!";
	}
};

class UninitializedException : std::exception {

	LPCSTR varName;

	const char * what() const throw () {
		return (std::string(varName) +  std::string(" has not been initialized correctly!")).c_str();
	}

	public:
		UninitializedException(LPCSTR varName) {
			this->varName = varName;
		}
};

class AlreadyInitializedException : std::exception {

	LPCSTR varName;

	const char * what() const throw () {
		return (std::string(varName) + std::string(" has already been initialized!")).c_str();
	}

public:
	AlreadyInitializedException(LPCSTR varName) {
		this->varName = varName;
	}
};