/* Copyright (c) 2017 Jin Li, http://www.luvfight.me

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#pragma once

NS_DOROTHY_BEGIN

class Event;
class EventType;

typedef Delegate<void (Event* event)> EventHandler;

/** @brief Use event listener to handle event. */
class Listener : public Object
{
public:
	PROPERTY_BOOL(Enabled);
	PROPERTY_REF(EventHandler, Handler);
	PROPERTY_READONLY_REF(string, Name);
	virtual ~Listener();
	virtual bool init() override;
	void clearHandler();
	void handle(Event* e);
	CREATE_FUNC(Listener);
protected:
	Listener(const string& name, const EventHandler& handler);
private:
	bool _enabled;
	string _name;
	EventHandler _handler;
	friend class EventType;
	DORA_TYPE_OVERRIDE(Listener);
};

NS_DOROTHY_END
