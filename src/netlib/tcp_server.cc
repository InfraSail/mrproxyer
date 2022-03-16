/**
 * @file tcp_server.cc
 * @brief
 * @date 2022-03-02
 *
 * @copyright Copyright (c) 2022 InfraSail
 *
 */

#include "tcp_server.h"

using namespace netlib;

TcpServer::TcpServer(IoWatcher* io_watcher, NetAddress& bind_address)
    : io_watcher_(io_watcher),
      acceptor_(std::make_unique<Acceptor>(io_watcher, bind_address)) {
  acceptor_->SetOnAccept(std::bind(&TcpServer::OnAccept, this,
                                   std::placeholders::_1,
                                   std::placeholders::_2));
}
TcpServer::~TcpServer() {
  for (const auto& item : connection_map_) {
    item.second->OnDestroying();
  }
}

// call OnConnectionCallback
void TcpServer::OnAccept(int conn_fd, NetAddress& peer_address) {
  auto name = peer_address.GetIpAndPort();
  log_info("new connection from %s fd = %d", name.c_str(), conn_fd);
  auto new_conn =
      std::make_shared<TcpEvent>(io_watcher_, name, conn_fd, peer_address);

  //  connection_map_[name] = new_conn;
  connection_map_.emplace(name, new_conn);
  // call user callback
  new_conn->SetOnConnection(on_connection_);
  new_conn->SetOnOnMessage(on_message_);
  new_conn->SetOnClose(
      std::bind(&TcpServer::OnClose, this, std::placeholders::_1));

  // call OnEstablishing
  new_conn->OnEstablishing();
}
void TcpServer::OnClose(const TcpEventPrt_t& conn) {
  auto name = conn->GetName();
  // remove from conn map
  auto status = connection_map_.erase(name);
  assert(status == 1);
  log_info("remove connection from %s", name.c_str());
  conn->OnDestroying();
}
void TcpServer::Run() { acceptor_->Listen(); }
