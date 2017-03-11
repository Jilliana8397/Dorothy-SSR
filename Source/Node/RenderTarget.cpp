/* Copyright (c) 2017 Jin Li, http://www.luvfight.me

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. */

#include "Const/Header.h"
#include "Node/RenderTarget.h"
#include "Cache/TextureCache.h"
#include "Node/Sprite.h"
#include "Basic/Camera.h"
#include "Basic/View.h"
#include "Basic/Director.h"
#include "Basic/Scheduler.h"
#include "Basic/Application.h"
#include "lodepng.h"
#include "Common/Async.h"
#include "Basic/Content.h"

NS_DOROTHY_BEGIN

RenderTarget::RenderTarget(Uint16 width, Uint16 height, bgfx::TextureFormat::Enum format):
_textureWidth(width),
_textureHeight(height),
_format(format)
{ }

RenderTarget::~RenderTarget()
{
	if (bgfx::isValid(_frameBufferHandle))
	{
		bgfx::destroyFrameBuffer(_frameBufferHandle);
		_frameBufferHandle = BGFX_INVALID_HANDLE;
	}
}

void RenderTarget::setCamera(Camera* camera)
{
	_camera = camera;
}

Camera* RenderTarget::getCamera() const
{
	return _camera;
}

bool RenderTarget::init()
{
	const Uint32 textureFlags = (
		BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP |
		BGFX_TEXTURE_RT);
	Uint32 extraFlags = 0;
	switch (bgfx::getCaps()->rendererType)
	{
	case bgfx::RendererType::Direct3D9:
	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12:
		break;
	default:
		extraFlags = BGFX_TEXTURE_READ_BACK;
		break;
	}
	_frameBufferHandle = bgfx::createFrameBuffer(_textureWidth, _textureHeight, _format, textureFlags | extraFlags);
	bgfx::TextureHandle textureHandle = bgfx::getTexture(_frameBufferHandle);
	bgfx::TextureInfo info;
	bgfx::calcTextureSize(info,
		_textureWidth, _textureHeight,
		0, false, false, 1, _format);
	_texture = Texture2D::create(textureHandle, info, textureFlags | extraFlags);

	setSize(Size{s_cast<float>(_textureWidth), s_cast<float>(_textureHeight)});

	_sprite = Sprite::create(_texture);
	_sprite->setPosition(Vec2{getWidth() / 2.0f, getHeight() / 2.0f});
	addChild(_sprite);

	return true;
}

void RenderTarget::renderAfterClear(Node* target, bool clear, Color color, float depth, Uint8 stencil)
{
	SharedView.pushName("RenderTarget"_slice, [&]()
	{
		Uint8 viewId = SharedView.getId();
		bgfx::setViewFrameBuffer(viewId, _frameBufferHandle);
		bgfx::setViewRect(viewId, 0, 0, _textureWidth, _textureHeight);
		if (clear)
		{
			bgfx::setViewClear(viewId, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL,
				color.toRGBA(), depth, stencil);
		}
		else
		{
			bgfx::setViewClear(viewId, BGFX_CLEAR_NONE);
		}
		Matrix viewProj;
		if (_camera)
		{
			bx::mtxMul(viewProj, _camera->getView(), SharedView.getProjection());
		}
		else
		{
			switch (bgfx::getCaps()->rendererType)
			{
			case bgfx::RendererType::Direct3D9:
			case bgfx::RendererType::Direct3D11:
			case bgfx::RendererType::Direct3D12:
				bx::mtxOrtho(viewProj, 0, s_cast<float>(_textureWidth), 0, s_cast<float>(_textureHeight), -1000.0f, 1000.0f);
				break;
			default:
				bx::mtxOrtho(viewProj, 0, s_cast<float>(_textureWidth), s_cast<float>(_textureHeight), 0, -1000.0f, 1000.0f);
				break;
			}
		}
		bgfx::setViewTransform(viewId, nullptr, viewProj);
		SharedDirector.pushViewProjection(viewProj, [&]()
		{
			renderOnly(target);
		});
	});
}

void RenderTarget::renderOnly(Node* target)
{
	if (!target) return;
	Node* parent = target->getParent();
	if (parent)
	{
		parent->removeChild(target);
	}
	target->markDirty();
	target->visit();
	SharedRendererManager.flush();
	target->markDirty();
	if (parent)
	{
		target->addTo(parent);
	}
}

void RenderTarget::render(Node* target)
{
	renderAfterClear(target, false);
}

void RenderTarget::renderWithClear(Node* target, Color color, float depth, Uint8 stencil)
{
	renderAfterClear(target, true, color, depth, stencil);
}

void RenderTarget::saveAsync(String filename, const function<void()>& callback)
{
	AssertIf((bgfx::getCaps()->supported & BGFX_CAPS_TEXTURE_READ_BACK) == 0, "texture read back not supported.");

	Uint32 extraFlags = 0;
	switch (bgfx::getCaps()->rendererType)
	{
	case bgfx::RendererType::Direct3D9:
	case bgfx::RendererType::Direct3D11:
	case bgfx::RendererType::Direct3D12:
		extraFlags = BGFX_TEXTURE_BLIT_DST;
		break;
	default:
		break;
	}
	bgfx::TextureHandle textureHandle;
	if (extraFlags)
	{
		const Uint32 textureFlags = BGFX_TEXTURE_U_CLAMP | BGFX_TEXTURE_V_CLAMP | BGFX_TEXTURE_READ_BACK;
		textureHandle = bgfx::createTexture2D(_textureWidth, _textureHeight, false, 1, _format, textureFlags | extraFlags);
		SharedView.pushName("SaveTarget"_slice, [&]()
		{
			bgfx::blit(SharedView.getId(), textureHandle, 0, 0, _texture->getHandle());
		});
	}
	else
	{
		textureHandle = _texture->getHandle();
	}
	Uint8* data = new Uint8[_texture->getInfo().storageSize];
	Uint32 frame = bgfx::readTexture(textureHandle, data);
	Uint32 width = s_cast<Uint32>(_textureWidth);
	Uint32 height = s_cast<Uint32>(_textureHeight);
	string file(filename);
	SharedDirector.getSystemScheduler()->schedule([frame, textureHandle, extraFlags, data, width, height, file, callback](double deltaTime)
	{
		DORA_UNUSED_PARAM(deltaTime);
		if (frame <= SharedApplication.getFrame())
		{
			if (extraFlags)
			{
				bgfx::destroyTexture(textureHandle);
			}
			SharedAsyncThread.Process.run([data, width, height]()
			{
				unsigned error;
				LodePNGState state;
				lodepng_state_init(&state);
				Uint8* out = nullptr;
				size_t outSize = 0;
				error = lodepng_encode(&out, &outSize, data, width, height, &state);
				lodepng_state_cleanup(&state);
				delete [] data;
				return Values::create(out, outSize);
			}, [callback, file](Values* values)
			{
				Uint8* out;
				size_t outSize;
				values->get(out, outSize);
				Slice content(r_cast<char*>(out), outSize);
				SharedContent.saveToFileAsync(file, content, [out, callback]()
				{
					::free(out);
					callback();
				});
			});
			return true;
		}
		return false;
	});
}

NS_DOROTHY_END
