/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2015. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef NT_LEB128_H_
#define NT_LEB128_H_

#include <cstddef>

namespace nt {

class raw_istream;

std::size_t SizeUleb128(unsigned long val);
std::size_t WriteUleb128(char* addr, unsigned long val);
std::size_t ReadUleb128(const char* addr, unsigned long* ret);
bool ReadUleb128(raw_istream& is, unsigned long* ret);

}  // namespace nt

#endif  // NT_LEB128_H_
