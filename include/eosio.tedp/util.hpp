// Copyright (c) Microsoft Corporation. All rights reserved.
// Copyright (c) 2020 Syed Jafri. All rights reserved.
// Licensed under the MIT License..

#pragma once

namespace bigint {
  using checksum256 = intx::uint256;
}

namespace eosio_evm
{
  inline std::array<uint8_t, 32u> toBin(const Address& address)
  {
    std::array<uint8_t, 32> address_bytes = {};
    intx::be::unsafe::store(address_bytes.data(), address);
    return address_bytes;
  }

  inline eosio::checksum256 toChecksum256(const Address& address)
  {
    return eosio::checksum256( toBin(address) );
  }

  static inline uint256_t checksum256ToValue(const eosio::checksum256& input) {
    std::array<uint8_t, 32U> output = {};
    auto input_bytes = input.extract_as_byte_array();
    std::copy(std::begin(input_bytes), std::end(input_bytes), std::begin(output));
    return intx::be::unsafe::load<uint256_t>(output.data());
  }
}