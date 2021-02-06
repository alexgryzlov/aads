#include "error.hpp"

namespace {

    struct ErrorCategory : std::error_category {
      [[nodiscard]] const char* name() const noexcept override;
      [[nodiscard]] std::string message(int ev) const override;
    };

    const char* ErrorCategory::name() const noexcept {
      return "hash-table";
    }

    std::string ErrorCategory::message(int ev) const
    {
      switch (ev)
      {
        case KEY_EXIST:
          return "This key already exists.";
        case KEY_NOT_EXIST:
          return "This key does not exist.";
        case DELETE_FAIL:
          return "Unable to delete due to the key absence.";
        default:
          return "Unknown error.";
      }
    }

    const ErrorCategory myErrorCategory {}; // using const to avoid static initialization order fiasco.

}

std::error_code make_error_code(ErrorCode e)
{
  return {static_cast<int>(e), myErrorCategory};
}