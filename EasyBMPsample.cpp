#include <iostream>
#include <cmath>
#include <cctype>
#include <cstring>
#include <ctime> 
#include "pthread.h"
#include "EasyBMP.h"

#define NUM_THREADS     8
using namespace std;
struct RGBData{
	RGBApixel** Pixels;
	RGBApixel** NeWPixels;
	int startposx;
	int endposx;
	int startposy;
	int endposy;

};
struct DATA{
	int b;
	RGBApixel** Pixels;
};

void *thread_func(void *vptr_args)
{

	cout << "thread"<<endl;
	int i;
	RGBData *rgbdata;
	rgbdata = (RGBData *)vptr_args;
	for (int i = rgbdata->startposx; i < rgbdata->endposx; i++)
	{
		for (int j = rgbdata->startposy; j < rgbdata->endposy; j++)
		{
		//(rgbdata->Pixels[i][j].Red + rgbdata->Pixels[i][j].Green + rgbdata->Pixels[i][j].Blue)/3;
			rgbdata->NeWPixels[i][j].Red = 255 - rgbdata->Pixels[i][j].Red;
			rgbdata->NeWPixels[i][j].Green = 255 - rgbdata->Pixels[i][j].Green;
			rgbdata->NeWPixels[i][j].Blue = 255 - rgbdata->Pixels[i][j].Blue;
			rgbdata->NeWPixels[i][j].Alpha = rgbdata->Pixels[i][j].Alpha;

		}
	}

	return NULL;
}
int main(int argc, char* argv[])
{
	pthread_t threads[NUM_THREADS];
	RGBData t[NUM_THREADS];
	RGBApixel** NewPixels;
	BMP Input;
	void *status;
	Input.ReadFromFile("Fronalpstock_big.bmp");
	BMP Output;
	Output.SetBitDepth(24);
	Output.SetDPI(Input.TellHorizontalDPI(), Input.TellVerticalDPI());
	Output.SetSize(Input.TellWidth(), Input.TellHeight());
	unsigned int start_time = clock();
	for (int i = 0; i < NUM_THREADS; i++){
		int step = Input.TellWidth() / NUM_THREADS;
		t[i].NeWPixels = Output.Pixels;
		t[i].Pixels = Input.Pixels;
		t[i].startposx = 0 + i * step;
		t[i].startposy = 0;
		t[i].endposx = step * (i + 1);
		if (i == NUM_THREADS - 1)
			t[i].endposx = Input.TellWidth();
		t[i].endposy = Input.TellHeight();
		pthread_create(&threads[i], NULL, thread_func, (void *)&t[i]);
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], &status);
	}
	unsigned int end_time = clock();
	unsigned int solve_time = end_time - start_time;
	cout << "time to solve =" << solve_time<<"ms"<<endl;
	Output.WriteToFile("EasyBMPimg1NEW.bmp");

	return 0;
}
