// Copyright 2017 Wu Tao
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include <cassert>

#include <boost/optional.hpp>
#include <silly/status.h>

namespace yaraft {

class Error {
 public:
  enum ErrorCodes {
    OK,

    OutOfBound,
    InvalidConfig,
    LogCompacted,
    StepLocalMsg,
    StepPeerNotFound,
    SnapshotUnavailable,
    ProposeToNonLeader,
  };

  static constexpr uint64_t ErrorCodesNum = LogCompacted + 1;

  static inline std::string ToString(unsigned int errorCode) {
    return toString(errorCode);
  }

 private:
  friend class silly::Status<Error, Error::ErrorCodes>;

  static std::string toString(unsigned int errorCode);
};

typedef silly::Status<Error, Error::ErrorCodes> Status;

template <typename T>
class StatusWith {
 public:
  // for ok case
  StatusWith(T value) : status_(Status::OK()), value_(value) {}

  // for error case
  StatusWith(Status status) : status_(std::move(status)) {}

  StatusWith(Error::ErrorCodes code, const silly::Slice& reason)
      : StatusWith(Status::Make(code, reason)) {}

  StatusWith(Error::ErrorCodes code) : StatusWith(Status::Make(code, nullptr)) {}

  const T& GetValue() const {
    assert(status_.IsOK());
    return *value_;
  }

  T& GetValue() {
    assert(status_.IsOK());
    return *value_;
  }

  const Status& GetStatus() const {
    return status_;
  }

  bool IsOK() const {
    return status_.IsOK();
  }

  std::string ToString() const {
    return status_.ToString();
  }

 private:
  Status status_;
  boost::optional<T> value_;
};

}  // namespace yaraft
