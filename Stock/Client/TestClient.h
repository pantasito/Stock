#pragma once
#include "BaseClient.h"

namespace Stock
{
  namespace Client
  {
    class TestClient : public BaseClient
    {
      public:
        TestClient() = default;

        void Run() override
        {
          // Тут код клиентской активности
        }
    };
  }
}