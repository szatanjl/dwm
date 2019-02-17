/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>

/* appearance */
static const unsigned int borderpx  = 3;        /* border pixel of windows */
static const unsigned int gappx     = 10;       /* gap pixel between windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int showsystray        = 1;        /* 0 means no systray */
static const int statusonallmons    = 1;        /* 0 means only on selected monitor */
static const int focusonmove        = 0;        /* focus window on mouse move */
static const int focusonwheel       = 0;        /* focus window on mouse wheel */
static const char *fonts[]          = {
	"DejaVuSansMono Nerd Font:pixelsize=16:antialias=true:autohint=true",
	"monospace:pixelsize=16:antialias=true:autohint=true",
};
static const char col_fgn[]         = "#7c818c";
static const char col_bgn[]         = "#404552";
static const char col_fgs[]         = "#d3dae3";
static const char col_bgs[]         = "#383c4a";
static const char col_nor[]         = "#6c717c";
static const char col_sel[]         = "#5294e2";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]  = { col_fgn, col_bgn, col_nor },
	[SchemeSel]   = { col_fgs, col_bgs, col_bgs },
	[TagSelNorm]  = { col_sel, col_bgn, col_bgn },
	[TagSelSel]   = { col_sel, col_bgs, col_bgs },
	[TagNorm]     = { col_fgn, col_bgn, col_bgn },
	[TagSel]      = { col_fgs, col_bgs, col_bgs },
	[LayoutNorm]  = { col_fgn, col_bgn, col_bgn },
	[LayoutSel]   = { col_fgs, col_bgs, col_bgs },
	[TitleNorm]   = { col_fgn, col_bgn, col_bgn },
	[TitleSel]    = { col_fgs, col_bgs, col_bgs },
	[SystrayNorm] = { col_fgn, col_bgn, col_bgn },
	[SystraySel]  = { col_fgs, col_bgs, col_bgs },
	[StatusNorm]  = { col_fgn, col_bgn, col_bgn },
	[StatusSel]   = { col_fgs, col_bgs, col_bgs },
	/* colors for status (\x01, \x02, ...) */
	{ col_sel, col_bgn, col_bgn },
	{ col_sel, col_bgs, col_bgs },
	{ col_fgn, col_bgn, col_bgn },
	{ col_fgs, col_bgs, col_bgs },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *      WM_CLASS(STRING) = instance, class
	 *      WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{  NULL,      NULL,       NULL,       0,            0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int decorhints  = 1;    /* 1 means respect decoration hints */
static const int tiledraise  = 1;    /* 1 means raise tiled windows when focused */
static const int restorefloat = 1;   /* 1 means restore window's last position after togglefloating */

typedef void (*Attach)(Client *);
static const Attach attachclient = attachbelow;

static void tile1(Monitor *m)
{
	selmon->nmaster = 1;
	tile(m);
}

static void tile2(Monitor *m)
{
	selmon->nmaster = 2;
	tile(m);
}

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]-",      tile1 },   /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[]=",      tile2 },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* commands */
#define CMD(...) { .v = (const char*[]){ "dwm-cmd", __VA_ARGS__, NULL } }
static const char *termcmd[]       = { "termite", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY|ShiftMask,             XK_q,      spawn,  CMD("quitmenu", "-m", mon_str) },
	{ MODKEY,                       XK_p,      spawn,  CMD("appsmenu", "-m", mon_str, "-i", "-F", "-s") },
	{ MODKEY,                       XK_o,      spawn,  CMD("docsmenu", "-m", mon_str, "-i", "-F", "-s") },

	{ MODKEY|ControlMask,           XK_l,      spawn,  CMD("screenlock") },
	{ MODKEY|ShiftMask,             XK_Return, spawn,  {.v = termcmd } },

	{ 0,                            XK_Print,  spawn,  CMD("screenshot") },
	{ MODKEY,                       XK_Print,  spawn,  CMD("screenshot", "monitor", mon_str) },
	{ ShiftMask,                    XK_Print,  spawn,  CMD("screenshot", "window", win_str) },
	{ ControlMask,                  XK_Print,  spawn,  CMD("screenshot", "select") },

	{ 0,             XF86XK_Sleep,             spawn,  CMD("quit", "suspend") },
	{ 0,             XF86XK_RFKill,            spawn,  CMD("rfkill", "toggle") },
	{ 0,             XF86XK_TouchpadToggle,    spawn,  CMD("touchpad", "toggle") },
	{ 0,             XF86XK_MonBrightnessDown, spawn,  CMD("backlight", "down") },
	{ 0,             XF86XK_MonBrightnessUp,   spawn,  CMD("backlight", "up") },
	{ 0,             XF86XK_AudioMute,         spawn,  CMD("volume", "toggle") },
	{ 0,             XF86XK_AudioLowerVolume,  spawn,  CMD("volume", "down") },
	{ 0,             XF86XK_AudioRaiseVolume,  spawn,  CMD("volume", "up") },

	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_Return, zoom_or_maximize, {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },

	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },

	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },

	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)

	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_n,      setlayout,      {.v = &layouts[3]} },

	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY,                       XK_b,      setcfact,       {.f =  0.00} },
	{ MODKEY|ShiftMask,             XK_b,      resetcfact,     {0} },

	{ MODKEY,                       XK_Down,   moveresize,     {.v = (int []){ 0, 50, 0, 0 }}},
	{ MODKEY,                       XK_Up,     moveresize,     {.v = (int []){ 0, -50, 0, 0 }}},
	{ MODKEY,                       XK_Right,  moveresize,     {.v = (int []){ 50, 0, 0, 0 }}},
	{ MODKEY,                       XK_Left,   moveresize,     {.v = (int []){ -50, 0, 0, 0 }}},
	{ MODKEY|ShiftMask,             XK_Down,   moveresize,     {.v = (int []){ 0, 0, 0, 50 }}},
	{ MODKEY|ShiftMask,             XK_Up,     moveresize,     {.v = (int []){ 0, 0, 0, -50 }}},
	{ MODKEY|ShiftMask,             XK_Right,  moveresize,     {.v = (int []){ 0, 0, 50, 0 }}},
	{ MODKEY|ShiftMask,             XK_Left,   moveresize,     {.v = (int []){ 0, 0, -50, 0 }}},
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
