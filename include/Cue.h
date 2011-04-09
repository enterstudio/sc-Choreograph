/*
 *  Cue.h
 *  BasicTween
 *
 *  Created by David Wicks on 2/24/11.
 *  Copyright 2011 David Wicks. All rights reserved.
 *
 */

#pragma once

#include "TimelineItem.h"
#include "cinder/Function.h"

namespace cinder
{

	namespace tween
	{
		class Cue : public TimelineItem
  		{
		  public:
			Cue( std::function<void ()> fn, float atTime = 0 );

			void					setFn( std::function<void ()> fn ) { mFunction = fn; }
			std::function<void ()>	getFn() const { return mFunction; }
			
 		  protected:
			virtual void				reverse() { /* no-op */ }
			virtual TimelineItemRef	cloneReverse() const;

			virtual void start() {} // starting is a no-op for Cues
			virtual void loopStart();
			virtual void update( float relativeTime ) {} // update is a no-op for Cues
			virtual void complete() {} // completion is a no-op for Cues
			virtual bool updateAtLoopStart() { return true; }
		  
			std::function<void ()>		mFunction;
		};
		
		typedef std::shared_ptr<Cue>	CueRef;
	}
	
	
}