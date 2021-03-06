/*
 Copyright (c) 2011, The Cinder Project, All rights reserved.
 This code is intended for use with the Cinder C++ library: http://libcinder.org

 Based on the sc-Choreograph CinderBlock by David Wicks: http://sansumbrella.com/

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "TimelineItem.h"
#include "Timeline.h"

#include "cinder/CinderMath.h"

namespace cinder {

TimelineItem::TimelineItem( class Timeline *parent )
	: mParent( parent ), mTarget( 0 ), mStartTime( 0 ), mDuration( 0 ), mInvDuration( 0 ), mHasStarted( false ),
		mComplete( false ), mMarkedForRemoval( false ), mAutoRemove( true ), mInfinite( false ), mLoop( false ), mLastLoopIteration( -1 ), mUseAbsoluteTime( false )
{
}

TimelineItem::TimelineItem( Timeline *parent, void *target, float startTime, float duration )
	: mParent( parent ), mTarget( target ), mStartTime( startTime ), mDuration( duration ), mInvDuration( duration == 0 ? 0 : (1 / duration) ), mHasStarted( false ),
		mComplete( false ), mMarkedForRemoval( false ), mAutoRemove( true ), mInfinite( false ), mLoop( false ), mLastLoopIteration( -1 ), mUseAbsoluteTime( false )
{
}

void TimelineItem::removeSelf()
{
	mMarkedForRemoval = true;
}

void TimelineItem::stepTo( float newTime )
{
	if( mComplete || mMarkedForRemoval )
		return;
	
	float absTime = newTime - mStartTime;
	
	if( newTime >= mStartTime ) {
		float relTime;
		if( mLoop ) {
			relTime = math<float>::fmod( absTime * mInvDuration, 1 );
		}
		else
			relTime = math<float>::min( absTime * mInvDuration, 1 );
		
		if( ! mHasStarted ) {
			mHasStarted = true;
			loopStart();
			start();
		}
		
		float time = ( mUseAbsoluteTime ) ? absTime : relTime;
		
		if( mLoop ) {
			int32_t loopIteration = static_cast<int32_t>( ( newTime - mStartTime ) * mInvDuration );
			if( loopIteration != mLastLoopIteration ) {
				mLastLoopIteration = loopIteration;
				loopStart();
				update( time );
			}
			else
				update( time );
		}
		else
			update( time );
	}
	if( newTime >= mStartTime + mDuration && ( ! mLoop ) && ( ! mInfinite ) ) {
		mComplete = true;
		complete();
	}
}

void TimelineItem::setStartTime( float time )
{
	mStartTime = time;
	if( mParent )
		mParent->itemTimeChanged( this );
}

void TimelineItem::setDuration( float duration )
{
	mDuration = duration;
	mInvDuration = duration == 0 ? 1 : ( 1 / duration );
	if( mParent )
		mParent->itemTimeChanged( this );
}

float TimelineItem::loopTime( float absTime )
{
	float result = absTime;
	
	if( mLoop ) {
		result = math<float>::fmod( result * mInvDuration, 1 );
		result *= mDuration;
	}

	return result;
}

} // namespace cinder