#pragma once

#include <vector>

static const int kClientsActivityTimeDelta = 10;
static const int kMaxNumberOfClients = 10000;

namespace Stock
{
  namespace StockInfo
  {
    struct TimeCountOfTrade
    {
      int _time;
      int _count;
    };

    class ClientsActivity {
      std::vector<TimeCountOfTrade> _clients_trades[kMaxNumberOfClients];

    public:
      void AddTradedOrder(uint64_t client_id, uint64_t time, uint32_t count) {
        _clients_trades[client_id].emplace_back(time, count);
      }

      int GetCount(uint64_t client_id, uint64_t time) { //складываю разные типа uint64_t и int
        int result = 0;
        const auto last_time = time - kClientsActivityTimeDelta;

        for (auto it = _clients_trades[client_id].rbegin();
          it != _clients_trades[client_id].rend(); ++it) {

          if (it->_time >= last_time) {
            result += it->_count;
          }
          else {
            break;
          }
        }

        return result;
      }
    };
  }
}