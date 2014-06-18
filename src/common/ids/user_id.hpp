#ifndef USER_ID_HPP
#define USER_ID_HPP

#include <cstdint>
#include <common/packables/packable_uint16.hpp>

namespace como {

typedef std::uint16_t UserID;
typedef PackableUint16< UserID > PackableUserID;

const UserID NO_USER = 0;

} // namespace como

#endif // USER_ID_HPP
