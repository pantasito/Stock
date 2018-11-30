#pragma once

#include "ClientsBroker.h"

#include <memory>
#include <cstdint>

namespace Stock
{
  namespace Client
  {
    class ClientsBroker;

    // ������� ����� ��� ���� ��������
    class BaseClient
    {
      static uint64_t _last_client_id; // ��� ��������� ���� �������� ����������� id
      uint64_t _id;

    protected:
      std::weak_ptr<ClientsBroker> _broker;

    public:
      BaseClient()
      {
        _id = _last_client_id++;
      }
      virtual ~BaseClient() = default;

      BaseClient(const BaseClient&) = delete;
      BaseClient& operator=(BaseClient&) = delete;

      void SetBroker(std::weak_ptr<ClientsBroker> broker) { _broker = broker; }

      uint64_t Id() { return _id; }

      virtual void Run() = 0;
    };

    uint64_t BaseClient::_last_client_id = 0;
  }
}