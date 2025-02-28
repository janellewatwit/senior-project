#pragma once
#include "vstgui/vstgui.h"

class UIView : public VSTGUI::CView
{
public:
	UIView (const VSTGUI::CRect& size) : CView (size) {}

	void draw (VSTGUI::CDrawContext* context) override
	{
		context->setFrameColor (VSTGUI::kGreenCColor);
		context->drawLine (VSTGUI::CPoint (0,0), VSTGUI::CPoint (10, 10));
	}
};
