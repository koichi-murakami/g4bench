/*============================================================================
Copyright 2017 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#include <sstream>
#include <string>
#include "G4Event.hh"
#include "eventaction.h"
#include "util/timehistory.h"

// --------------------------------------------------------------------------
namespace {

TimeHistory* gtimer = nullptr;

void ShowProgress(int nprocessed, const std::string& key) {
  std::cout << "[MESSAGE] event-loop check point: "
            << nprocessed << " events processed." << std::endl;
  gtimer-> ShowHistory(key);
}

} // end of namespace

// --------------------------------------------------------------------------
EventAction::EventAction()
{
  ::gtimer = TimeHistory::GetTimeHistory();
}

// --------------------------------------------------------------------------
EventAction::~EventAction()
{
}

// --------------------------------------------------------------------------
void EventAction::BeginOfEventAction(const G4Event* event)
{
  int ievent = event-> GetEventID();
  if ( ievent == 0 ) {
    ::gtimer-> TakeSplit("FirstEventStart");
  }
}

// --------------------------------------------------------------------------
void EventAction::EndOfEventAction(const G4Event* event)
{
  int ievent = event-> GetEventID();
  const int kKiloEvents = 1000;

  if ( ievent % kKiloEvents == 0 && ievent != 0 ) {
    int event_in_mega = ievent / kKiloEvents;
    std::stringstream key;
    key << "EventCheckPoint:" << event_in_mega << "K";
    ::gtimer-> TakeSplit(key.str());
    ::ShowProgress(ievent, key.str());
  }
}
