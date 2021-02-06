#ifndef INC_HASH_TABLE
#define INC_HASH_TABLE

#include <system_error>


enum ErrorCode : int {
  KEY_EXIST = 1,
  KEY_NOT_EXIST,
  DELETE_FAIL
};
template <>
struct std::is_error_code_enum<ErrorCode > : true_type {};

std::error_code make_error_code(ErrorCode);

#endif //INC_HASH_TABLE
