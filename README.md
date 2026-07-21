# lottery

EOSIO smart contract (C++) for a **lottery-style flow** that requests randomness via **Oraclize** (Provable) and handles the callback on-chain.

## What this repo contains

- `lottery.cpp` — single contract file
  - `getrand` — authenticated action that queries Oraclize for a random integer (via random.org URL JSON)
  - `callback` — Oraclize callback handler that receives the result/proof

## Stack

- EOSIO C++ contract API (`eosiolib`)
- Oraclize / Provable EOS API (`oraclize/eos_api.hpp`)

## Notes

- This is a compact learning / prototype contract, not a full production lottery product (no UI, limited economic logic in-repo).
- Oraclize/Provable integration and EOSIO tooling have evolved; compiling today may require an older CDT / toolchain matching the includes.

## Author

[ArmaghanRazaChaudhary](https://github.com/ArmaghanRazaChaudhary)
