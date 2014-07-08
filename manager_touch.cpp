#include "manager_touch.h"

int touch_element::GetTouchX()
{
	return touch_mgr->cur_samp.x;
}
int touch_element::GetTouchY()
{
	return touch_mgr->cur_samp.y;
}
int touch_element::GetTouchP()
{
	return touch_mgr->cur_samp.pressure;
}

