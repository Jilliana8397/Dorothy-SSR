/* Copyright (c) 2017 Jin Li, http://www.luvfight.me

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#pragma once

struct ImDrawData;

NS_DOROTHY_BEGIN

class Listener;
class TouchHandler;
class Texture2D;
class Effect;

class ImGUIDora : public TouchHandler
{
public:
	virtual ~ImGUIDora();
	bool init();
	void begin();
	void end();
	void handleEvent(Event* e);
	virtual bool handle(const SDL_Event& event) override;
protected:
	ImGUIDora();
	static const char* getClipboardText(void*);
	static void setClipboardText(void*, const char* text);
	static void renderDrawLists(ImDrawData* _drawData);
private:
	bool _textInputing;
	bool _mousePressed[3];
	float _mouseWheel;
	Ref<Texture2D> _fontTexture;
	Ref<Effect> _effect;
	Ref<Listener> _listener;
	bgfx::UniformHandle _textureSampler;
	bgfx::VertexDecl _vertexDecl;
	uint8_t _viewId;
	list<SDL_Event> _inputs;
	int _editingDel;
	int _keydownDel;
	int _textLength;
	char _textEditing[SDL_TEXTINPUTEVENT_TEXT_SIZE];
};

#define SharedImGUI \
	silly::Singleton<ImGUIDora, SingletonIndex::ImGUIDora>::shared()

NS_DOROTHY_END
