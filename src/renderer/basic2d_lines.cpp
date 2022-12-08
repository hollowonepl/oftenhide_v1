#include <stdio.h>
#include <stdlib.h>

void PutPixelGeneric(int* buf, int col, int x, int y, int pitch, int height)
{
	if (x<0 || x>=pitch || y<0 || y>=height)
		return;

	buf[x+y*pitch] = col;
}

void PutPixel640x480(int* buf, int col, int x, int y, int pitch, int height)
{
	buf[x+y*640] = col;
}

#define PIXEL PutPixelGeneric

void MidPointLine(
	int* buf, int argb,
	int x1, int y1, int x2, int y2,
	int pitch, int height
	)
{
	bool yLonger=false;
	int incrementVal, endVal;
	int shortLen=y2-y1;
	int longLen=x2-x1;
	if (abs(shortLen)>abs(longLen)) {
		int swap=shortLen;
		shortLen=longLen;
		longLen=swap;
		yLonger=true;
	}
	endVal=longLen;
	if (longLen<0) {
		incrementVal=-1;
		longLen=-longLen;
	} else incrementVal=1;
	int decInc;
	if (longLen==0) decInc=0;
	else decInc = (shortLen << 16) / longLen;
	int j=0;
	if (yLonger) {
		for (int i=0;i!=endVal;i+=incrementVal) {
			PIXEL(buf, argb, x1+(j >> 16),y1+i, pitch, height);
			j+=decInc;
		}
	} else {
		for (int i=0;i!=endVal;i+=incrementVal) {
			PIXEL(buf, argb, x1+i,y1+(j >> 16), pitch, height);
			j+=decInc;
		}
	}
}

void EFLALine(int* buf, int argb,
	int x1, int y1, int x2, int y2,
	int pitch, int height)
{
	bool yLonger=false;
	int shortLen=y2-y1;
	int longLen=x2-x1;

	if (abs(shortLen)>abs(longLen)) {
		int swap=shortLen;
		shortLen=longLen;
		longLen=swap;
		yLonger=true;
	}

	int decInc;
	if (longLen==0) decInc=0;
	else decInc = (shortLen << 16) / longLen;

	if (yLonger) {
		if (longLen>0) {
			longLen+=y1;
			for (int j=0x8000+(x1<<16);y1<=longLen;++y1) {
				PIXEL(buf, argb,j >> 16,y1,pitch, height);
				j+=decInc;
			}
			return;
		}
		longLen+=y1;
		for (int j=0x8000+(x1<<16);y1>=longLen;--y1) {
			PIXEL(buf, argb,j >> 16,y1,pitch, height);
			j-=decInc;
		}
		return;
	}

	if (longLen>0) {
		longLen+=x1;
		for (int j=0x8000+(y1<<16);x1<=longLen;++x1) {
			PIXEL(buf, argb,x1,j >> 16,pitch, height);
			j+=decInc;
		}
		return;
	}
	longLen+=x1;
	for (int j=0x8000+(y1<<16);x1>=longLen;--x1) {
		PIXEL(buf, argb,x1,j >> 16,pitch, height);
		j-=decInc;
	}

}
