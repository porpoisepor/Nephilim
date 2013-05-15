#ifndef UISizePolicy_h__
#define UISizePolicy_h__

#include "Platform.h"

NEPHILIM_NS_BEGIN

class NEPHILIM_API UISizePolicy
{
public:

	enum Styles
	{
		Auto =					1 << 0,///< Arbitrary pixel dimensions, the default style, "no policies"
		FixedPixel =			1 << 1, ///< Fixed pixel amount, only the actual code can change this sizes.
		WindowProportional =	1 << 2, ///< A % relative to the window's dimensions
		ParentProportional =	1 << 3, ///< A % relative to the parent's dimensions
	};

	Styles widthPolicy;
	float width, height;
};

NEPHILIM_NS_END
#endif // UISizePolicy_h__