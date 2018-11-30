#pragma once

#include "TestClient.h"

namespace Stock
{
  namespace Client
  {
    // ‘абричный метод дл€ генерации клиента по строке
    static std::unique_ptr<BaseClient> CreateClient(const std::string& client_type_name)
    {
      if (client_type_name == "TestClient")
      {
        return std::unique_ptr<BaseClient>(new TestClient);
      }
    }
  }
}