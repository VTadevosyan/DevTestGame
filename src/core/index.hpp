
#ifndef INDEX_HPP
#define INDEX_HPP

#include <cstddef>
#include <limits>


//// @class index
//// @brief Represents an board index item to set and get values using index
////
class index
{
public:
	//// @brief Constructor
	//// @param[in] r The row index
	//// @param[in] c The column index
    index(size_t r, size_t c) noexcept;

	//// @brief Constructor
	//// @note Don't use this function
	//// Used to create static invalid index object during compilation
	constexpr index(size_t invalid_value) noexcept
		: m_row(invalid_value)
		, m_col(invalid_value)
	{
	}

	//// @brief Copy Constructor
    index(const index&) noexcept;

	//// @brief Destructor
    ~index() = default;

public:
	//// @brief Returns true if two indexes keeps same rows and columns, false - otherwise
	bool operator== (const index&) const noexcept;

	//// @brief Returns true if two indexes keeps different rows and columns, false - otherwise
	bool operator!= (const index&) const noexcept;

	//// @brief Returns true if first index is less than second
	//// Less means that first index has smaller row index than second
	//// If rows are equal checks the columns. First index's column should be smaller than second's column
	bool operator<  (const index&) const noexcept;


public:
	//// @brief Gets the row's index
    size_t row() const noexcept;

	//// @brief Gets the column's index
	size_t column() const noexcept;

	//// @brief Checks the index validity
    bool is_valid() const noexcept;

private:
    size_t m_row;
    size_t m_col;
};

static constexpr size_t max_value = std::numeric_limits<size_t>::max();
//// pre-defined invalid index
static constexpr index invalid_index(max_value);


#endif // INDEX_HPP
