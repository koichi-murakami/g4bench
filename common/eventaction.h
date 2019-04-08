/*============================================================================
Copyright 2017-2019 Koichi Murakami

Distributed under the OSI-approved BSD License (the "License");
see accompanying file LICENSE for details.

This software is distributed WITHOUT ANY WARRANTY; without even the
implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the License for more information.
============================================================================*/
#ifndef EVENT_ACTION_H_
#define EVENT_ACTION_H_

#include "G4UserEventAction.hh"

class EventAction : public G4UserEventAction {
public:
  EventAction();
  virtual ~EventAction() = default;

  void SetCheckCounter(int val);

  virtual void BeginOfEventAction(const G4Event* event);
  virtual void EndOfEventAction(const G4Event* event);

private:
  int check_counter_;

};

// ==========================================================================
inline void EventAction::SetCheckCounter(int val)
{
  check_counter_ = val;
}

#endif
