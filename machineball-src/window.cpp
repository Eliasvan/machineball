// Machine Ball
//  By Benny Kramek
//  benny@kramekweb.com
//  http://benny.kramekweb.com
//
//
// Source Code is released under the GPL
//

#define ALLEGRO_STATICLINK
#include <allegro.h>
#ifdef ALLEGRO_WINDOWS
#include <winalleg.h>
#endif
#include <alleggl.h>

#include <aldumb.h>

#include "mbdata.h"

#include "window.h"
#include "options.h"

static int width, height;

struct resolution_entry {
	int w,h;
	char s[10];
} resolutions[] = {
	{  320,  200, " 320x200 " },
	{  320,  240, " 320x240 " },
	{  400,  300, " 400x300 " },
	{  512,  384, " 512x384 " },
	{  640,  400, " 640x400 " },
	{  640,  480, " 640x480 " },
	{  800,  600, " 800x600 " },
	{ 1024,  768, "1024x768 " },
	{ 1152,  864, "1152x864 " },
	{ 1280,  960, "1280x960 " },
	{ 1280, 1024, "1280x1024" },
	{ 1600, 1200, "1600x1200" },
	{ 1920, 1440, "1920x1440" }
};

static char *resolution_lister (int i, int *size)
{
	if (i < 0) {
		*size = sizeof resolutions / sizeof *resolutions;
		return NULL;
	}
	return resolutions[i].s;
}



struct colour_depth_entry {
	int depth;
	char s[7];
} colour_depths[] = {
	{ 15, "15 bpp" },
	{ 16, "16 bpp" },
	{ 24, "24 bpp" },
	{ 32, "32 bpp" }
};

static char *colour_depth_lister (int i, int *size)
{
	if (i < 0) {
		*size = sizeof colour_depths / sizeof *colour_depths;
		return NULL;
	}
	return colour_depths[i].s;
}

struct zbuffer_depth_entry {
	int depth;
	char s[7];
} zbuffer_depths[] = {
	{  8, " 8 bpp" },
	{ 16, "16 bpp" },
	{ 24, "24 bpp" },
	{ 32, "32 bpp" }
};

static char *zbuffer_depth_lister (int i, int *size)
{
	if (i < 0) {
		*size = sizeof zbuffer_depths / sizeof *zbuffer_depths;
		return NULL;
	}
	return zbuffer_depths[i].s;
}


static int setup (int do_configure)
{
#define RESOLUTION_LIST   4
#define COLOUR_LIST       6
#define ZBUFFER_LIST      8
#define BUTTON_OK        9
	
	DIALOG dlg[] = {
    /*	proc                 x    y    w    h  fg bg  key    flags d1 d2  dp */
    {	d_box_proc,        100,   130, 400, 220,  0, 0,   0,         0, 0, 0, NULL },
    {	d_ctext_proc,      300,  150,   0,   0,  0, 0,   0,         0, 0, 0, (char*)"____________" },
    {	d_ctext_proc,      300,  148,   0,   0,  0, 0,   0,         0, 0, 0, (char*)"OpenGL Setup" },
    {	d_text_proc,       130,  180,   0,   0,  0, 0,   0,         0, 0, 0, (char*)"Resolution" },
    {	d_list_proc,       130,  190,  96, 120,  0, 0,   0,         0, options.resolution, 0, (void*)resolution_lister },
    {	d_text_proc,       250,  180,   0,   0,  0, 0,   0,         0, 0, 0, (char*)"Color depth" },
    {	d_list_proc,       250,  190,  96,  48,  0, 0,   0,         0, options.colordepth, 0, (void*)colour_depth_lister },
    {	d_text_proc,       370,  180,  96,  48,  0, 0,   0,         0, 0, 0, (char*)"Z depth" },
    {	d_list_proc,       370,  190,  96,  48,  0, 0,   0,         0, options.zdepth, 0, (void*)zbuffer_depth_lister },
    {	d_button_proc,     370, 300,  96,  18,  0, 0,   0,    D_EXIT, 0, 0, (char*)"Ok" },
    {	d_button_proc,     250, 300,  96,  18,  0, 0,   0,    D_EXIT, 0, 0, (char*)"Exit" },
    {	NULL }
	};

	int x;

	if (do_configure)
	{
		set_dialog_color (dlg, makecol(255, 255, 255), makecol(32, 32, 64));
			
		x = do_dialog (dlg, 9);
		
		options.resolution = dlg[RESOLUTION_LIST].d1;
		options.colordepth = dlg[COLOUR_LIST].d1;
		options.zdepth = dlg[ZBUFFER_LIST].d1;
	}
	else
		x = BUTTON_OK;
	
	allegro_gl_clear_settings();
	allegro_gl_set (AGL_COLOR_DEPTH, colour_depths[options.colordepth].depth);
	allegro_gl_set (AGL_Z_DEPTH, zbuffer_depths[options.zdepth].depth);
	allegro_gl_set (AGL_DOUBLEBUFFER, TRUE);
	allegro_gl_set (AGL_RENDERMETHOD, 1);
	allegro_gl_set(AGL_FULLSCREEN, TRUE);
	allegro_gl_set(AGL_REQUIRE, AGL_RENDERMETHOD);
	allegro_gl_set (AGL_SUGGEST, AGL_COLOR_DEPTH | AGL_Z_DEPTH | AGL_DOUBLEBUFFER | AGL_RENDERMETHOD | AGL_FULLSCREEN);
	width  = resolutions[options.resolution].w;
	height = resolutions[options.resolution].h;
	
	return (x == BUTTON_OK);
}


void createWindow(int force, int do_configure)
{
	// Load configuration from disk
	// ...
	
	// No configuration file found: use default values
	// ...
	
	// Create normal Allegro Window that offers video mode options
	allegro_init();
	atexit(&dumb_exit);

	dumb_register_stdfiles();

if (do_configure) {	
	set_color_depth(16);
	if(set_gfx_mode(GFX_AUTODETECT_WINDOWED, 600, 400, 0, 0) < 0)
	{
		allegro_message("Error creating allegro window: \n%s\n", allegro_error);
		exit(-1);
	}

#ifdef ALLEGRO_WINDOWS 
	HWND wnd = win_get_window(); 
	HWND desktop = GetDesktopWindow(); 
	RECT wndRect, desktopRect; 
	int  w,h, dw, dh; 

	GetWindowRect(wnd        , &wndRect); 
	GetWindowRect(desktop, &desktopRect); 
	w  = wndRect.right      - wndRect.left; 
	h  = wndRect.bottom     - wndRect.top; 
	dw = desktopRect.right  - desktopRect.left; 
	dh = desktopRect.bottom - desktopRect.top; 

	MoveWindow(wnd, (dw-w)/2, (dh-h)/2, w, h, TRUE); 
#endif
}

	install_keyboard();
	install_mouse();
	install_timer();
	install_joystick(JOY_TYPE_AUTODETECT);
	if(force==1)
	{
		if(install_sound(DIGI_AUTODETECT, MIDI_NONE, 0)!=0)
			exit(-1);
	}
	else
	{
		install_sound(DIGI_NONE, MIDI_NONE, 0);
	}
	set_volume(options.soundvol, -1);

if (do_configure) {
	acquire_screen();
	
	rectfill(screen, 0, 0, 599, 399, makecol(64, 64, 128));
	int i;
	for(i=0; i<400; i++)
		hline(screen, 0, i, 599, makecol(0, 0, i*192/400));
	blit(&mb_machineball_bmp, screen, 0, 0, 172, 20, 256, 64);
	text_mode(-1);
	textout_centre(screen, font, "Benny Kramek", 300, 90, makecol(255, 255, 255)); 
	textout_centre(screen, font, "http://benny.kramekweb.com", 300, 110, makecol(255, 255, 255)); 
	int x, y;
	for(y=0; y<400; y++)
		for(x=0; x<600; x++)
			if(getr(getpixel(screen, x, y))==255 && getb(getpixel(screen, x, y))==255 && getg(getpixel(screen, x, y))==0)
				putpixel(screen, x, y, makecol(0, 0, y*192/400));

	release_screen();
}
	
	install_allegro_gl();

	int ok = setup(do_configure);
	if(!ok)
		exit(0);
	
	set_color_depth(32);
	if(set_gfx_mode(GFX_OPENGL, width, height, 0, 0) < 0)
	{
		allegro_message("Error creating OpenGL window: \n%s\n", allegro_error);
		exit(-1);
	}
	allegro_gl_begin();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_COLOR_MATERIAL);
	allegro_gl_end();
	set_color_depth(32);
}
