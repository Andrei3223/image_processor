#pragma once

#include <exception>

class ArgumentsError : public std::exception {};

class BMPHeaderError : public std::exception {};

class WrongInputPath : public std::exception {};

class WrongOutputPath : public  std::exception {};

