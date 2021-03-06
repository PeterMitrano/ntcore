/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2015. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#include "raw_socket_istream.h"

using namespace nt;

raw_socket_istream::~raw_socket_istream() {}

bool raw_socket_istream::read(void* data, std::size_t len) {
  char* cdata = static_cast<char*>(data);
  std::size_t pos = 0;

  while (pos < len) {
    TCPStream::Error err;
    std::size_t count =
        m_stream.receive(&cdata[pos], len - pos, &err, m_timeout);
    if (count == 0) return false;
    pos += count;
  }
  return true;
}

void raw_socket_istream::close() { m_stream.close(); }
