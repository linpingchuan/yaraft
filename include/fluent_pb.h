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

#include "storage.h"

#include <yaraft/pb/raftpb.pb.h>

namespace yaraft {

// This file provides utilities to construct protobuf structs in fluent style.

struct PBMessage {
  pb::Message v;

  PBMessage& Term(uint64_t term) {
    v.set_term(term);
    return *this;
  }

  PBMessage& LogTerm(uint64_t logTerm) {
    v.set_logterm(logTerm);
    return *this;
  }

  PBMessage& Type(pb::MessageType type) {
    v.set_type(type);
    return *this;
  }

  PBMessage& To(uint64_t to) {
    v.set_to(to);
    return *this;
  }

  PBMessage& From(uint64_t from) {
    v.set_from(from);
    return *this;
  }

  PBMessage& Index(uint64_t index) {
    v.set_index(index);
    return *this;
  }

  PBMessage& Commit(uint64_t commit) {
    v.set_commit(commit);
    return *this;
  }

  PBMessage& Reject() {
    v.set_reject(true);
    return *this;
  }

  PBMessage& Reject(bool reject) {
    v.set_reject(reject);
    return *this;
  }

  PBMessage& RejectHint(uint64_t hint) {
    v.set_rejecthint(hint);
    return *this;
  }

  PBMessage& Entries(EntryVec list) {
    for (auto& e : list) {
      v.add_entries()->Swap(&e);
    }
    return *this;
  }

  PBMessage& Snapshot(pb::Snapshot& snap) {
    auto s = new pb::Snapshot();
    s->Swap(&snap);
    v.set_allocated_snapshot(s);
    return *this;
  }
};

using EntriesIterator = ::google::protobuf::RepeatedPtrField<::yaraft::pb::Entry>::iterator;

struct PBEntry {
  pb::Entry v;

  PBEntry& Type(pb::EntryType type) {
    v.set_type(type);
    return *this;
  }

  PBEntry& Index(uint64_t index) {
    v.set_index(index);
    return *this;
  }

  PBEntry& Term(uint64_t term) {
    v.set_term(term);
    return *this;
  }

  PBEntry& Data(const silly::Slice& data) {
    v.set_data(data.RawData(), data.Len());
    return *this;
  }
};

struct PBSnapshot {
  pb::Snapshot v;

  PBSnapshot& MetaIndex(uint64_t index) {
    v.mutable_metadata()->set_index(index);
    return *this;
  }

  PBSnapshot& MetaTerm(uint64_t term) {
    v.mutable_metadata()->set_term(term);
    return *this;
  }

  PBSnapshot& MetaConfState(std::vector<uint64_t> nodes) {
    auto conf = new pb::ConfState;
    for (uint64_t n : nodes)
      conf->add_nodes(n);
    v.mutable_metadata()->set_allocated_conf_state(conf);
    return *this;
  }
};

struct PBHardState {
  pb::HardState v;

  PBHardState& Vote(uint64_t vote) {
    v.set_vote(vote);
    return *this;
  }

  PBHardState& Term(uint64_t term) {
    v.set_term(term);
    return *this;
  }

  PBHardState& Commit(uint64_t commit) {
    v.set_commit(commit);
    return *this;
  }
};

struct PBConfChange {
  pb::ConfChange v;

  PBConfChange& Type(pb::ConfChangeType type) {
    v.set_type(type);
    return *this;
  }

  PBConfChange& NodeId(uint64_t id) {
    v.set_nodeid(id);
    return *this;
  }
};

}  // namespace yaraft
