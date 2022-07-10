#ifndef GUARD_CONSTANTS_HPP
#define GUARD_CONSTANTS_HPP

inline constexpr unsigned int BYTES_PER_WORD = 4;
inline constexpr unsigned int SP_WORD_OFFSET = 2;
inline constexpr unsigned int SP_BYTE_OFFSET = BYTES_PER_WORD * SP_WORD_OFFSET;

inline constexpr unsigned int CHAR_SIZE = 1;
inline constexpr unsigned int SHORT_SIZE = 2;
inline constexpr unsigned int INT_SIZE = 4;
inline constexpr unsigned int LONG_SIZE = 4;
inline constexpr unsigned int POINTER_SIZE = 4; // REVIEW might be 8
inline constexpr unsigned int FLOAT_SIZE = 4;
inline constexpr unsigned int DOUBLE_SIZE = 8;
inline constexpr unsigned int LONG_DOUBLE_SIZE = 8;
inline constexpr unsigned int ENUM_SIZE = INT_SIZE;

inline constexpr unsigned int CHAR_ALLIGNMENT = 1;
inline constexpr unsigned int SHORT_ALLIGNMENT = 2;
inline constexpr unsigned int INT_ALLIGNMENT = 4;
inline constexpr unsigned int LONG_ALLIGNMENT = 4;
inline constexpr unsigned int POINTER_ALLIGNMENT = 4;
inline constexpr unsigned int FLOAT_ALLIGNMENT = 4;
inline constexpr unsigned int DOUBLE_ALLIGNMENT = 8;
inline constexpr unsigned int LONG_DOUBLE_ALLIGNMENT = 8;

inline const std::string tab("\t");

#endif