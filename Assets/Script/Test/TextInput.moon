Dorothy!
LineRect = require "UI.View.Shape.LineRect"
SolidRect = require "UI.View.Shape.SolidRect"
utf8 = require "utf8"

TextInput = Class ((args)->
	{:fontName,:fontSize,:width,:height,:hint,:text} = args
	hint or= ""
	text or= hint
	label = with Label fontName,fontSize
		.batched = false
		.text = text
		.y = height/2 - fontSize/2
		.anchor = Vec2.zero
		.alignment = TextAlign.Left

	cursor = Line {Vec2.zero,Vec2(0,fontSize+2)},Color 0xffffffff
	blink = -> loop ->
		cursor.visible = true
		sleep 0.5
		cursor.visible = false
		sleep 0.5
	with cursor
		.y = label.y
		.visible = false

	updateText = (text)->
		label.text = text
		offsetX = math.max label.width+3-width, 0
		label.x = -offsetX
		cursor.x = label.width-offsetX+1
		cursor\schedule blink!

	node = with ClipNode SolidRect width:width,height:height
		textEditing = ""
		textDisplay = ""

		.size = Size width,height
		.position = Vec2(width,height)/2
		.hint = hint
		\addChild label
		\addChild cursor

		updateIMEPos = ->
			\convertToWindowSpace Vec2(-label.x+label.width,0),(pos)->
				Keyboard\updateIMEPosHint pos
		startEditing = ->
			\attachIME!
			updateIMEPos!
		.updateDisplayText = (text)=>
			textDisplay = text
			label.text = text

		.imeAttached = false
		\slot "AttachIME",->
			.imeAttached = true
			updateText textDisplay

		\slot "DetachIME",->
			.imeAttached = false
			cursor.visible = false
			cursor\unschedule!
			label.x = 0
			label.text = .hint if textDisplay == ""

		.touchEnabled = true
		\slot "Tapped",-> startEditing! unless .imeAttached

		.keyboardEnabled = true
		\slot "KeyPressed",(key)->
			return if textEditing ~= ""
			switch key
				when "BackSpace"
					if #textDisplay > 0
						textDisplay = utf8.sub textDisplay, 1, -2
						updateText textDisplay
				when "Return"
					\detachIME!

		\slot "TextInput",(text)->
			textDisplay = utf8.sub(textDisplay, 1, -1-utf8.len(textEditing))..text
			textEditing = ""
			updateText textDisplay
			updateIMEPos!

		\slot "TextEditing",(text,start)->
			textDisplay = utf8.sub(textDisplay, 1, -1-utf8.len(textEditing))..text
			textEditing = text
			label.text = textDisplay
			offsetX = math.max label.width+3-width, 0
			label.x = -offsetX
			charSprite = label\getCharacter utf8.len(textDisplay)-utf8.len(textEditing)+start
			if charSprite
				cursor.x = charSprite.x+charSprite.width/2-offsetX+1
				cursor\schedule blink!
			updateIMEPos!

	with Node!
		.content = node
		.cursor = cursor
		.label = label
		.size = Size width,height
		\addChild node
	),{
		text:property => @label.text,
			(value)=>
				@content\detachIME! if @content.imeAttached
				@content\updateDisplayText value
	}

Director\pushEntry with TextInput hint:"点这里进行输入",width:200,height:34,fontName:"NotoSansHans-Regular",fontSize:20
	--.text = "默认的文本 Default Text"
	--.cursor.color = Color 0xff00ffff
	.label.color = Color 0xffff0080
	\addChild LineRect x:-2,width:.width+4,height:.height,color:0xff00ffff