// Copyright 2018 E*D Films. All Rights Reserved.

/**
 * midiWriter.h
 *
 * Writer interfaces to turn an EventGraph into Midi or XML files
 * 
 * @author  dotBunny <hello@dotbunny.com>
 * @version 1
 * @since	  1.0.0
 */

#pragma once

#include "midiRequired.h"
#include "midiEventGraph.h"

namespace SceneTrackMidi
{

  typedef void(*EventWriterFunction)(const char* dst, EventGraph* graph);

  void EventXmlWriter(const char* dst, EventGraph* graph, std::map<u32, std::string>& names);

  void EventMidiWriter(const char* dst, EventGraph* graph, std::map<u32, std::string>& names);

}
