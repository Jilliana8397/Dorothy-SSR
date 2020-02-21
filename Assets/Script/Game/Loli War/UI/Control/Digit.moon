Dorothy builtin.Platformer
import "UI.View.Digit"

Class Digit,
	__init:=>
		@_value = 99
		@maxValue = 99

	value: property (=> @_value),
		(value)=>
			@_value = math.max math.min(@maxValue,value),0
			@removeAllChildren!
			two = math.floor @_value/10
			if two > 0
				with Sprite "Image/misc.clip|#{two}"
					.anchor = Vec2 0,0.5
					\addTo @
			one = @_value%10
			with Sprite "Image/misc.clip|#{one}"
				.x = 6
				.anchor = Vec2 0,0.5
				\addTo @
