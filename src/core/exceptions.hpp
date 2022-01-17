
#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <stdexcept>


//// @class base_exception
class base_exception : public std::exception
{
public:
	//// @brief Constructor
    base_exception(const char*);

	//// @brief Constructor
    base_exception(const std::string&);

	//// @brief Move Constructor
    base_exception(base_exception&&) = default;

	//// @brief Copy Constructor
    base_exception(const base_exception&) = default;

	//// @brief Destructor
    virtual ~base_exception() = default;

public:
	//// @brief Deleted Copy Assignment
    base_exception& operator= (const base_exception&) = delete;

	//// @brief Deleted Move Assignment
    base_exception&& operator= (base_exception&&) = delete;

public:
	//// @brief Gets the error message
	//// @note base_exception::what() returns "Unknown Error" message
    const char* what() const noexcept override;

protected:
    const char* m_err_msg = "Unknown Error";
};


//// @class json_error
class json_error : public base_exception
{
public:
	//// @brief Constructor
    json_error(const char*);

};


//// @class positive_integer_error
class positive_integer_error : public base_exception
{
public:
	//// @brief Constructor
    positive_integer_error();

};


//// @class board_size_error
class board_size_error : public base_exception
{
public:
	//// @brief Constructor
	//// @param[in] a Represents an area: The value must be "rows" or "cols" or empty
    board_size_error();

};


//// @class moves_count_error
class moves_count_error : public base_exception
{
public:
	//// @brief Constructor
    moves_count_error();

};


//// @class figures_count_error
class figures_count_error : public base_exception
{
public:
	//// @brief Constructor
    figures_count_error();

};


//// @class figures_color_error
class figure_color_error : public base_exception
{
public:
	//// @brief Constructor
    figure_color_error();

};


//// @class objectives_count_error
class objectives_count_error : public base_exception
{
public:
	//// @brief Constructor
    objectives_count_error();

};


#endif /// EXCEPTIONS_HPP
