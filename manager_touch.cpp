#include "manager_touch.h"

int touch_element::GetTouchX() const
{
	return touch_mgr->cur_samp.x;
}
int touch_element::GetTouchY() const
{
	return touch_mgr->cur_samp.y;
}
int touch_element::GetTouchP() const
{
	return touch_mgr->cur_samp.pressure;
}

