/*----------------------------------------------------------------------------*/
/* Copyright (c) FIRST 2015. All Rights Reserved.                             */
/* Open Source Software - may be modified and shared by FRC teams. The code   */
/* must be accompanied by the FIRST BSD license file in the root directory of */
/* the project.                                                               */
/*----------------------------------------------------------------------------*/

#ifndef NT_DISPATCHER_H_
#define NT_DISPATCHER_H_

#include <atomic>
#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "llvm/StringRef.h"

#include "NetworkConnection.h"
#include "Storage.h"

class TCPAcceptor;

namespace nt {

class Dispatcher {
 public:
  static Dispatcher& GetInstance() {
    if (!m_instance) m_instance.reset(new Dispatcher);
    return *m_instance;
  }
  ~Dispatcher();

  void StartServer(const char* listen_address, unsigned int port);
  void StartClient(const char* server_name, unsigned int port);
  void Stop();
  void SetUpdateRate(double interval);
  void SetIdentity(llvm::StringRef name);
  void Flush();

  bool active() const { return m_active; }

  Dispatcher(const Dispatcher&) = delete;
  Dispatcher& operator=(const Dispatcher&) = delete;

 private:
  Dispatcher();

  void DispatchThreadMain();
  void ServerThreadMain(const char* listen_address, unsigned int port);
  void ClientThreadMain(const char* server_name, unsigned int port);

  void ClientReconnect();

  NT_Type GetEntryType(unsigned int id) const;

  struct Connection {
    enum State {
    };
    State state;
    std::string remote_id;
    std::unique_ptr<NetworkConnection> net;
  };

  void AddConnection(Connection&& conn);

  bool m_server;
  std::thread m_dispatch_thread;
  std::thread m_clientserver_thread;
  std::thread m_notifier_thread;

  std::shared_ptr<TCPAcceptor> m_server_acceptor;

  // Mutex for user-accessible items
  std::mutex m_user_mutex;
  std::vector<Connection> m_connections;
  std::string m_identity;

  std::atomic_bool m_active;  // set to false to terminate threads
  std::atomic_uint m_update_rate;  // periodic dispatch update rate, in ms

  // Condition variable for forced dispatch wakeup (flush)
  std::mutex m_flush_mutex;
  std::condition_variable m_flush_cv;
  std::chrono::steady_clock::time_point m_last_flush;
  bool m_do_flush;

  // Condition variable for client reconnect
  std::mutex m_reconnect_mutex;
  std::condition_variable m_reconnect_cv;
  bool m_do_reconnect;

  // Map from integer id to storage entry.  Id is 16-bit, so just use a vector.
  mutable std::mutex m_idmap_mutex;
  std::vector<std::shared_ptr<StorageEntry>> m_idmap;

  // Global instance
  static std::unique_ptr<Dispatcher> m_instance;
};

}  // namespace nt

#endif  // NT_DISPATCHER_H_
