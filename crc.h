#pragma once

#include <array>
#include <string>
#include <unordered_map>
#include <vector>
#include <cstdint>

#include "transform.h"

class CRC
{

public:
  struct CRCParams
  {
    int bitWidth;
    uint64_t polynomial;
    uint64_t initialValue;
    uint64_t finalXorValue;
    bool refIn;
    bool refOut;
  };

  explicit CRC(std::string_view predefined);

  explicit CRC(const CRCParams &crcParams);

  static std::vector<std::string> getPoly();

  [[nodiscard]] TransformOut string(const std::string &data) const;

  [[nodiscard]] TransformOut file(const std::string &filePath) const;

private:
  std::vector<uint64_t> table_;
  std::array<uint8_t, 256> reverseTable_{};
  CRCParams params_{};
  uint64_t mask_;
  static const std::unordered_map<std::string, CRCParams> predefinedParams;

  uint64_t reverseBits(uint64_t value, int bitWidth = -1) const;

  uint64_t singleCRC(uint64_t data) const;

  void generateCrcTable();

  [[nodiscard]] uint64_t calculateCRC(uint8_t data, uint64_t crc) const;
};
