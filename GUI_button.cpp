#include "head_file.h"

Status button_move(ExMessage m, int change, int x, int y, int w, int h, TCHAR* text, COLORREF fill_color, COLORREF line_color, COLORREF text_color, COLORREF back_color)
{
	if (m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h)
	{

		clearrectangle(x, y, x + w, y + h);
		button(x - change, y - change, w + change, h + change, text, fill_color, line_color, text_color);
		FlushBatchDraw();
	}
	else
	{
		setlinecolor(back_color);
		rectangle(x - change, y - change, x + w, y);
		rectangle(x - change, y - change, x, y + h);
		setfillcolor(back_color);
		fillrectangle(x - change, y - change, x + w + change, y + h +change);
		fillrectangle(x - change, y - change, x, y + h);
		button(x, y, w, h, text, fill_color, line_color, text_color);
		FlushBatchDraw();
	}
	return OK;
}

Status button_click(ExMessage m, int change, int x, int y, int w, int h, TCHAR* text, COLORREF fill_color, COLORREF line_color, COLORREF text_color)
{
	if (m.x >= x && m.x <= x + w && m.y >= y && m.y <= y + h)
	{
		button(x - change, y - change, w + change, h + change, text, (fill_color - 0x101010) % 0xFFFFFF, line_color, text_color);
		FlushBatchDraw();
		return OK;
	}

	else FlushBatchDraw();
	return ERROR;
}

Status button(int x, int y, int w, int h, TCHAR* text, COLORREF fill_color, COLORREF line_color, COLORREF text_color)
{

	setbkmode(TRANSPARENT);
	setfillcolor(fill_color);
	fillroundrect(x, y, x + w, y + h, 10, 10);
	setlinecolor(line_color);
	rectangle(x, y, x + w, y + h);
	// Êä³ö×Ö·û´®£¨MBCS ×Ö·û¼¯£©
	TCHAR s1[] = L"ºÚÌå";
	settextstyle(30, 0, s1);
	TCHAR s[50] = L"hello";

	int tx = x + (w - textwidth(text)) / 2;
	int ty = y + (h - textheight(text)) / 2;

	settextcolor(text_color);
	outtextxy(tx, ty, text);

	FlushBatchDraw();

	setlinecolor(WHITE);
	setfillcolor(WHITE);
	return OK;
}