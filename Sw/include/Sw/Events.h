#pragma once

#include "Base.h"

namespace Sw
{

class EventListener 
{
	typedef std::function<LRESULT(WPARAM, LPARAM)> FnEvent;

protected:
	std::multimap<UINT, FnEvent> m_mapEvents;

public:
	EventListener();
	virtual ~EventListener();

	LRESULT SendEvent(UINT Msg, WPARAM wParam, LPARAM lParam);
	void SubscribeEvent(UINT Msg, FnEvent Cb);
	void UnsubscribeEvent(UINT Msg);
};

inline EventListener::EventListener()
{

}

inline EventListener::~EventListener()
{
}

inline LRESULT EventListener::SendEvent( UINT Msg, WPARAM wParam, LPARAM lParam )
{
	auto range = m_mapEvents.equal_range(Msg);
	for( auto itr = range.first; itr != range.second; itr++ )
	{
		(*itr).second(wParam, lParam);
	}
	return -1;
}

inline void EventListener::SubscribeEvent( UINT Msg, FnEvent Cb )
{
	m_mapEvents.insert( std::make_pair(Msg, Cb) );
}

inline void EventListener::UnsubscribeEvent( UINT Msg )
{
	auto itr = m_mapEvents.find(Msg);
	while( itr != m_mapEvents.end() )
	{
		m_mapEvents.erase(itr);
		itr = m_mapEvents.find(Msg);
	}
}

}
