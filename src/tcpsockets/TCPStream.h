/*
   TCPStream.h

   TCPStream class interface. TCPStream provides methods to trasnfer
   data between peers over a TCP/IP connection.

   ------------------------------------------

   Copyright � 2013 [Vic Hargrave - http://vichargrave.com]

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

#ifndef TCPSOCKETS_TCPSTREAM_H_
#define TCPSOCKETS_TCPSTREAM_H_

#include <cstddef>
#include <string>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#endif

#include "llvm/StringRef.h"

class TCPStream {
  int m_sd;
  std::string m_peerIP;
  int m_peerPort;

 public:
  friend class TCPAcceptor;
  friend class TCPConnector;

  ~TCPStream();

  enum Error {
    kConnectionClosed = 0,
    kConnectionReset = -1,
    kConnectionTimedOut = -2
  };

  std::size_t send(const char* buffer, std::size_t len, Error* err);
  std::size_t receive(char* buffer, std::size_t len, Error* err,
                      int timeout = 0);
  void close();

  llvm::StringRef getPeerIP() const { return m_peerIP; }
  int getPeerPort() const { return m_peerPort; }

  TCPStream(const TCPStream& stream) = delete;
  TCPStream& operator=(const TCPStream&) = delete;
 private:
  bool WaitForReadEvent(int timeout);

  TCPStream(int sd, struct sockaddr_in* address);
  TCPStream() = delete;
};

#endif
